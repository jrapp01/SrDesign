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
int count = 0;

short txFlag;
short txBuff[16];

long long timer;            //For velocity timing; updated by core timer
#define VEL_SCALE 4         //TODO: figure out optimal value for this

// Parallel Master Port Configuration
#define PMP_CONTROL        PMP_READ_POL_LO | PMP_WRITE_POL_LO | PMP_CS2_CS1_EN
#define PMP_MODE           PMP_MODE_SLAVE | PMP_IRQ_OFF
#define PMP_PORT          (PMP_PEN_OFF)
#define PMP_INT           (PMP_INT_OFF)

// Change Notice Config
// IOPORT bit masks can be found in ports.h
#define CONFIG          (CN_ON)
//#define PINS            (CN16_ENABLE)
#define PINS (CN0_ENABLE | CN1_ENABLE | CN8_ENABLE | CN9_ENABLE | \
		CN10_ENABLE | CN11_ENABLE | CN15_ENABLE | CN16_ENABLE | \
		CN17_ENABLE | CN18_ENABLE | CN19_ENABLE | CN20_ENABLE | \
		CN21_ENABLE)
//#define PULLUPS         (CN15_PULLUP_ENABLE | CN16_PULLUP_ENABLE)
#define PULLUPS (CN_PULLUP_DISABLE_ALL)
//#define INTERRUPT       (CHANGE_INT_ON | CHANGE_INT_PRI_2)
#define INTERRUPT (CHANGE_INT_ON | CHANGE_INT_PRI_1)

//Key states
typedef enum{
    GET_ON, //State for collecting note-on information
    GET_NEXT
}state;
state keyState[16]={GET_ON};

int adcread;

// Initial Values of Change Pins for Comparison
unsigned int initialC = 0;
unsigned int initialD = 0;
unsigned int initialF = 0;
unsigned int initialG = 0;

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
    //velocity /= VEL_SCALE;
    velocity = (velocity-249)/3;
    velocity = (velocity > (0x80)) ? 127 : velocity;
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
                keyState[i] = GET_NEXT;
            }else{
                keyState[i] = GET_ON;
            }
            break;

        case GET_NEXT:
            // Get_next is a state to ensure that the same impulse from the
            // FSR does not trigger multiple Note_on commands.
            // This state makes sure that the analog value has gone back down
            // below the threshold we set to ensure that the next detected
            // peak is actually a new note
            //noteOffBit = mPORTGReadBits(bitMap[i]);
            if(adcread < threshold){
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
    //CN STEP 1A. Disable all interrupts
    unsigned int status;
    status = INTDisableInterrupts();//mINTDisableSystemMultiVectoredInt();

    //CN STEP 1B. Set pins for digital inputs
    mPORTCSetPinsDigitalIn(BIT_13 | BIT_14); //CN0,1
    mPORTDSetPinsDigitalIn(BIT_6 | BIT_7 | BIT_13 | BIT_14 | BIT_15); //CN15,16,19,20,21
    mPORTFSetPinsDigitalIn(BIT_4 | BIT_5); //CN17,18
    mPORTGSetPinsDigitalIn(BIT_6 | BIT_7 | BIT_8 | BIT_9); //CN8,9,10,11

    mPORTASetPinsDigitalIn(0xFFFF); //Enable all port A pins as digital inputs
    mPORTBSetPinsAnalogIn(0xFFFF);             //Enable all analog
    mPORTDSetPinsDigitalOut(BIT_0 | BIT_1 | BIT_8);
    
    //Buttons 1 and 2 enabled
    //PORTSetPinsDigitalIn(IOPORT_D, BIT_6 | BIT_7);
    
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    //CN STEP 2. setup the change notice options
    mCNOpen(CONFIG, PINS, PULLUPS);

    //CN STEP 3. read port(s) to clear mismatch
    initialC = mPORTCReadBits(BIT_13 | BIT_14);
    initialD = mPORTDReadBits(BIT_6 | BIT_7 | BIT_13 | BIT_14 | BIT_15);
    initialF = mPORTFReadBits(BIT_4 | BIT_5);
    initialG = mPORTGReadBits(BIT_6 | BIT_7 | BIT_8 | BIT_9);
    
    //CN STEP 4. clear change notice interrupt flag
    ConfigIntCN(INTERRUPT);


    CloseADC10();
    SetChanADC10(INITCH);
    OpenADC10(CONFIG1, CONFIG2, CONFIG3, CFGPORT, CFGSCAN);
    ConfigIntADC10(CFGINT);
    EnableADC10();

    mPMPOpen(PMP_CONTROL, PMP_MODE, PMP_PORT, PMP_INT);
    mPMPEnable();

    mPORTDClearBits(BIT_0 | BIT_1 | BIT_8);

    // Enable change notice, enable discrete pins and weak pullups
    //mCNOpen(CONFIG, PINS, PULLUPS);

    // Read the port to clear any mismatch on change notice pins
    int dummy = mPORTDRead();

    // Clear change notice interrupt flag
    //ConfigIntCN(INTERRUPT);

    //CN STEP 5. enable multi-vector interrupts
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

/*void __ISR(_CHANGE_NOTICE_VECTOR, ipl2) ChangeNotice_Handler(void)
{
    // Step #1 - always clear the mismatch condition first
    int dummy = PORTReadBits(IOPORT_D, BIT_7);

    // Step #2 - then clear the interrupt flag
    mCNClearIntFlag();

    // Step #3 - process the switches
    if(dummy != BIT_7)
    {
        noteOff(0);
        keyState[0] = GET_ON;
        //count = (count > 15) ? 0 : count;
        //count++;
        PORTSetBits(IOPORT_D, BIT_1);       // turn on LED2
        DBPRINTF("Switch SW2 has been released. \n");
    }
    else
    {
        PORTClearBits(IOPORT_D, BIT_1);     // turn off LED2
        DBPRINTF("Switch SW2 has been pressed. \n");
    }

 }*/

/******************************************************************************
*	Change Notice Interrupt Service Routine
*
*   Note: Switch debouncing is not performed.
*   Code comes here if any note-off sensing circuits are triggered
*   The user must read the IOPORT to clear the IO pin change notice mismatch
*	condition first, then clear the change notice interrupt flag.
******************************************************************************/
void __ISR(_CHANGE_NOTICE_VECTOR, ipl2) ChangeNotice_Handler(void)
{
	// Step #1 - always clear the mismatch condition first
	unsigned int dummyC =  mPORTCReadBits(BIT_13 | BIT_14);
	unsigned int dummyD= mPORTDReadBits(BIT_6 | BIT_7 | BIT_13 | BIT_14 | BIT_15);
	unsigned int dummyF = mPORTFReadBits(BIT_4 | BIT_5);
	unsigned int dummyG = mPORTGReadBits(BIT_6 | BIT_7 | BIT_8 | BIT_9);

	// Step #2 - then clear the interrupt flag
	mCNClearIntFlag();

	// The current PORTx value can be compared to the PORTx read value
	// obtained at the last CN interrupt or during initialization,
	// and used to determine which pin changed

	// Code required for determining which button was pressed
	// Step #3 - process the switches
	switch (dummyC){
	case BIT_13:
		noteOff(0); //CN0
		break;
	case BIT_14:
		noteOff(1); //CN1
		break;
	default:
		break;
	}
	switch (dummyD){
	case BIT_6:
		noteOff(2);
		break;
	case BIT_7:
		noteOff(3);
		break;
	case BIT_13:
		noteOff(4);
		break;
	case BIT_14:
		noteOff(5);
		break;
	case BIT_15:
		noteOff(6);
		break;
	default:
		break;
	}
	switch (dummyF){
	case BIT_4:
		noteOff(7);
		break;
	case BIT_5:
		noteOff(8);
		break;
	default:
		break;
	}
	switch (dummyG){
	case BIT_6:
		noteOff(9);
		break;
	case BIT_7:
		noteOff(10);
		break;
	case BIT_8:
		noteOff(11);
		break;
	case BIT_9:
		noteOff(12);
		break;
	default:
		break;
	}

 }