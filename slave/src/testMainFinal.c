/*-----------------------------------------------------------------------------*
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
int count = 0;

//MIDI PMP data
short txFlag; //causes interrupt in master when data ready
short txBuff[16]; //contains MIDI data for master to send over USB

long long timer;            //For velocity timing; updated by core timer

// Parallel Master Port Configuration
#define PMP_CONTROL        PMP_READ_POL_LO | PMP_WRITE_POL_LO | PMP_CS2_CS1_EN
#define PMP_MODE           PMP_MODE_SLAVE | PMP_IRQ_OFF
#define PMP_PORT          (PMP_PEN_OFF)
#define PMP_INT           (PMP_INT_OFF)

// Change Notice Config
// IOPORT bit masks can be found in ports.h
#define CONFIG          (CN_ON)
#define PINS (CN0_ENABLE | CN1_ENABLE | CN8_ENABLE | CN9_ENABLE)
#define PULLUPS (CN_PULLUP_DISABLE_ALL)
#define INTERRUPT (CHANGE_INT_ON | CHANGE_INT_PRI_1)

// Initial Values of Change Pins for Comparison
unsigned int initialC = 0;
//unsigned int initialD = 0;
//unsigned int initialF = 0;
unsigned int initialG = 0;

//Key states
typedef enum{
    GET_ON, //State for collecting note-on information
    GET_NEXT
}state;
state keyState[16]={GET_ON};

int adcread;

const int threshold = 275;       //TODO: Optimize arbitrary threshold setting
int velocity[16][2]={0};        //Used to extract note-on velocity
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
    velocity = (velocity-247)/3; //min velocity = 9
    velocity = (velocity > (0x80)) ? 127 : velocity; //hard limit at 127
    txBuff[note]=(short)(velocity);
    SendNotes(note);
}

void noteOff(int note){
	//same as note on except velocity = 0
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

    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    mPORTBSetPinsAnalogIn(0xFFFF);             //Enable all analog
    mPORTDSetPinsDigitalOut(BIT_0 | BIT_1 | BIT_8);

    /****************Change Notice Setup*******************/
    //CN STEP 1A. Disable all interrupts
    unsigned int status;
    status = INTDisableInterrupts();//mINTDisableSystemMultiVectorInt();

    //CN STEP 1B. Set pins for digital inputs
    mPORTCSetPinsDigitalIn(BIT_13 | BIT_14); //CN0,1
    mPORTGSetPinsDigitalIn(BIT_6 | BIT_7); //CN8,9
/*    mPORTDSetPinsDigitalIn(BIT_6 | BIT_7 | BIT_13 | BIT_14 | BIT_15); //CN15,16,19,20,21
    mPORTFSetPinsDigitalIn(BIT_4 | BIT_5); //CN17,18
    mPORTGSetPinsDigitalIn(BIT_6 | BIT_7 | BIT_8 | BIT_9); //CN8,9,10,11
*/

    //CN STEP 2. setup the change notice options
    mCNOpen(CONFIG, PINS, PULLUPS);

    //CN STEP 3. read port(s) to clear mismatch
    initialC = mPORTCReadBits(BIT_13 | BIT_14);
    initialG = mPORTGReadBits(BIT_6 | BIT_7);
/*    initialD = mPORTDReadBits(BIT_6 | BIT_7 | BIT_13 | BIT_14 | BIT_15);
    initialF = mPORTFReadBits(BIT_4 | BIT_5);
    initialG = mPORTGReadBits(BIT_6 | BIT_7 | BIT_8 | BIT_9);
*/
    //CN STEP 4. clear change notice interrupt flag
    ConfigIntCN(INTERRUPT);

    /************************ADC Setup************************/
    CloseADC10();
    SetChanADC10(INITCH);
    OpenADC10(CONFIG1, CONFIG2, CONFIG3, CFGPORT, CFGSCAN);
    ConfigIntADC10(CFGINT);
    EnableADC10();

    /*******************External Interrupt Setup***************/
/*    ConfigINT0(EXT_INT_PRI_6 | RISING_EDGE_INT | EXT_INT_ENABLE);
    ConfigINT1(EXT_INT_PRI_6 | RISING_EDGE_INT | EXT_INT_ENABLE);
    ConfigINT2(EXT_INT_PRI_6 | RISING_EDGE_INT | EXT_INT_ENABLE);
    ConfigINT3(EXT_INT_PRI_6 | RISING_EDGE_INT | EXT_INT_ENABLE);
*/
    mPMPOpen(PMP_CONTROL, PMP_MODE, PMP_PORT, PMP_INT);
    mPMPEnable();

    mPORTDClearBits(BIT_0 | BIT_1 | BIT_8);

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

void __ISR(_CHANGE_NOTICE_VECTOR, ipl7AUTO) ChangeNotice_Handler(void)
{
    // Step #1 - always clear the mismatch condition first
    int dummy0 = PORTReadBits(IOPORT_C, BIT_14);
    int dummy1 = PORTReadBits(IOPORT_C, BIT_13);
    int dummy8 = PORTReadBits(IOPORT_G, BIT_6);
    int dummy9 = PORTReadBits(IOPORT_G, BIT_7);

    // Step #2 - then clear the interrupt flag
    mCNClearIntFlag();

    // Step #3 - process the switches
    if(dummy0 == BIT_14){
        noteOff(2);
        mPORTDToggleBits(BIT_1);       // turn on LED2
    }else if(dummy1 == BIT_13){
        noteOff(5);
        mPORTDToggleBits(BIT_1);       // turn on LED2
    }else if(dummy8 == BIT_6){
        noteOff(7);
        mPORTDToggleBits(BIT_1);       // turn on LED2
    }else if(dummy9 == BIT_7){
        noteOff(10);
        mPORTDToggleBits(BIT_1);       // turn on LED2
    }
    else
    {
        PORTClearBits(IOPORT_D, BIT_1);     // turn off LED2
    }

 }

void __ISR(_EXTERNAL_0_VECTOR, IPL7AUTO) Note2_Off(void){
    noteOff(2);
    mPORTDToggleBits(BIT_1);
    mINT0ClearIntFlag();
}

void __ISR(_EXTERNAL_1_VECTOR, IPL7AUTO) Note5_Off(void){
    noteOff(5);
    mPORTDToggleBits(BIT_1);
    mINT1ClearIntFlag();
}

void __ISR(_EXTERNAL_2_VECTOR, IPL7AUTO) Note7_Off(void){
    noteOff(7);
    mPORTDToggleBits(BIT_1);
    mINT2ClearIntFlag();
}

void __ISR(_EXTERNAL_3_VECTOR, IPL7AUTO) Note10_Off(void){
    noteOff(10);
    mPORTDToggleBits(BIT_1);
    mINT3ClearIntFlag();
}