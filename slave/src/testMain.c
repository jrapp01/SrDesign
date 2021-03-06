/*-----------------------------------------------------------------------------*
 *
 *   Copyright 2013 Tom Cahill
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 *
 *   File: main.c
 *
 *   Description: Includes hardware configuration, functions to send note data
 *   over parallel port (PMP), and ISR to continuously sample analog inputs
 *   and extract note on/off and velocity info from the inputs.
 *----------------------------------------------------------------------------*/

#include <plib.h>
#include <math.h>
#include "delay.h"

//System Configuration

#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (must be DIV_2)
#pragma config FPLLMUL  = MUL_20        // PLL Multiplier
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = XT            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = OFF            // Debugger Disabled for Starter Kit

#define SYS_FREQ         (80000000)
#define TICKS_SEC         10000
#define CORE_RATE        (SYS_FREQ/2)/TICKS_SEC

//ADC Configuration

#define CONFIG1 (ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON | \
                 ADC_IDLE_CONTINUE | ADC_MODULE_ON)
#define CONFIG2 (ADC_SAMPLES_PER_INT_16 | ADC_BUF_16 | ADC_ALT_INPUT_OFF | \
                 ADC_SCAN_ON | ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE)
#define CONFIG3 (ADC_CONV_CLK_PB | ADC_SAMPLE_TIME_2 | ADC_CONV_CLK_3Tcy)
#define CFGPORT (ENABLE_ALL_ANA)
#define CFGINT  (ADC_INT_PRI_7 | ADC_INT_SUB_PRI_2 | ADC_INT_ON)
#define CFGSCAN  0x0000 //enable all inputs for scan
#define INITCH  (ADC_CH0_POS_SAMPLEA_AN0 | ADC_CH0_NEG_SAMPLEA_NVREF)

//ADC Data

int max[16]={0};            //Value of each analog input at maximum displacement
int min[16]={0};            //Value of each analog input at rest
int samp1[16];              //20% from rest
int samp2[16];              //80% from rest

short txFlag;
short txBuff[16];

long long timer;            //For velocity timing; updated by core timer
#define VEL_SCALE 8         //TODO: figure out optimal value for this

// Parallel Master Port Configuration
#define PMP_CONTROL        PMP_READ_POL_LO | PMP_WRITE_POL_LO | PMP_CS2_CS1_EN
#define PMP_MODE           PMP_MODE_SLAVE | PMP_IRQ_OFF
#define PMP_PORT          (PMP_PEN_OFF)
#define PMP_INT           (PMP_INT_OFF)

//Key states
typedef enum{
    GET_ON, //State for collecting note-on information
    GET_OFF
}state;
state keyState[16]={GET_ON};

int adcread;
char noteOffBit;                //variable to hold value of bit for note off determination
unsigned int bitMap[16]={BIT_0,BIT_1,BIT_2,BIT_3,BIT_6,BIT_7,BIT_8,BIT_9,BIT_12,BIT_13,BIT_14,BIT_15};    //Maps digital pin to corresponding ADC channel

//int testSequence[25] = {0,1,2,3,4,5,14,19,30,48,59,65,81,104,233,450,999,781,617,23,12,16,10,0};

const int threshold = 250;       //TODO: Optimize arbitrary threshold setting
int velocity[16][2]={0};        //Record timestamps to extract note-on velocity
int forceData[16][2]={0};       //Collect FSR hammer force data
int slopeData[16][2]={0};       //Contrain calculated slope data

void SendNotes(int note){
    mPORTDSetBits(BIT_8);
    if((txFlag & (1<<note))>>note){
        while(!mIsPMPSlaveBufferEmpty());
        mPMPSlaveWrite(note);
        while(!mIsPMPSlaveBufferEmpty());
        mPMPSlaveWrite(txBuff[note]);
        txFlag -= (1 << note);
    }
    mPORTDClearBits(BIT_8);
}

void noteOn(int note, int velocity){
    txFlag |= 1 << note;
    velocity /= VEL_SCALE;
    //velocity = (velocity > (0x80)) ? 0 : velocity;
    //txBuff[note]=(short)(127-velocity); //not an inverse relationship in FSR system
    txBuff[note]=(short)(velocity);
    SendNotes(note);
}

void noteOff(int note){
    txFlag |= 1 << note;
    txBuff[note] = 0;
    SendNotes(note);
}

void __ISR(_ADC_VECTOR, IPL7AUTO) ADCHandle(void){
    mAD1ClearIntFlag();
    int i=0;
    for(i; i<16; i++){
        adcread = ReadADC10(i);

        switch(keyState[i]){
        case GET_ON:
            // Previous force value moved into bin 0
            // Bin 1 gets new value from ADC
            forceData[i][0] = forceData[i][1];
            forceData[i][1] = adcread;

            // Previous slope bit moved into bin 0
            // New slope bit determined from backward differentiator
            // if y[n] - y[n-1] is positive --> slope in bin 1 is '1'
            // else, negative slope gets '0'
            slopeData[i][0] = slopeData[i][1];
            if(forceData[i][1]>forceData[i][0]){
                slopeData[i][1]=1;
            }else{
                slopeData[i][1]=0;
            }

            // Slope in bin 1 will be less than bin 0 if the slope changes from
            // Positive to negative, meaning a peak has been found
            // If so, send out noteOn signal
            // Else keep searching for noteOn conditions
            if (slopeData[i][1]<slopeData[i][0] && forceData[i][0] > threshold){
                noteOn(i, forceData[i][0]);
                keyState[i] = GET_OFF;
            }else{
                keyState[i] = GET_ON;
            }
            break;

        case GET_OFF:
            //TODO: Change this to digital input from comparator
            // bitMap is an array that correlates the index i of the ADC to the
            // corresponding digital NoteOff pin
            //If the pin is high, note-off has been triggered
            noteOffBit = mPORTGReadBits(bitMap[i]);
            if(noteOffBit == 1){
                noteOff(i);
                keyState[i] = GET_ON;
            }
            break;

        }
    }
}

void __ISR(_TIMER_2_VECTOR, IPL5) T2Handle(void){
    mT2ClearIntFlag();
    timer += 1;
}

int main(void){
    mPORTASetPinsDigitalIn(0xFFFF); //Enable all port A pins as digital inputs
    mPORTBSetPinsAnalogIn(0xFFFF);             //Enable all analog
    mPORTDSetPinsDigitalOut(BIT_0 | BIT_1 | BIT_8);

    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
    CloseADC10();
    SetChanADC10(INITCH);
    OpenADC10(CONFIG1, CONFIG2, CONFIG3, CFGPORT, CFGSCAN);
    ConfigIntADC10(CFGINT);
    EnableADC10();

    mPMPOpen(PMP_CONTROL, PMP_MODE, PMP_PORT, PMP_INT);
    mPMPEnable();

    mPORTDClearBits(BIT_8);
    INTEnableSystemMultiVectoredInt();

    OpenTimer2(T2_ON | T2_PS_1_64, 256);
    ConfigIntTimer2(T2_INT_ON | T2_INT_PRIOR_5 | T2_INT_SUB_PRIOR_2);
    EnableIntT2;

   while(1){
        //Blink so we know we're still running
        DelayMs(1000);
        mPORTDToggleBits(BIT_0);
    }
    return 0;
}