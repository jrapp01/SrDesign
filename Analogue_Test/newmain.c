/* 
 * File:   newmain.c
 * Author: Administrator
 *
 * Created on March 26, 2014, 10:06 PM
 */
// KEY CONCEPT - Adds support for PIC32 Peripheral library functions and macros
#include <plib.h>
#include <xc.h>

// KEY CONCEPT - Set Configuration Bits in code
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (PIC32 Starter Kit: use divide by 2 only)
#pragma config FPLLMUL  = MUL_20        // PLL Multiplier
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = XT            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx1      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = OFF           // Debugger Disabled for Starter Kit
//#pragma config JTAGEN = OFF         // Disable JTAG

// application defines
#define SYS_FREQ		(80000000)
#define TICKS_SEC         10000
#define CORE_RATE    (SYS_FREQ/2)/TICKS_SEC

//ADC Configuration

#define CONFIG1 (ADC_FORMAT_INTG | ADC_CLK_AUTO | ADC_AUTO_SAMPLING_ON | \
		  ADC_IDLE_CONTINUE | ADC_MODULE_ON)
#define CONFIG2 (ADC_SAMPLES_PER_INT_16 | ADC_BUF_16 | ADC_ALT_INPUT_OFF | \
                               ADC_SCAN_ON | ADC_VREF_AVDD_AVSS | ADC_OFFSET_CAL_DISABLE)
#define CONFIG3 (ADC_CONV_CLK_PB | ADC_SAMPLE_TIME_2 | ADC_CONV_CLK_3Tcy)
#define CFGPORT (ENABLE_ALL_ANA)
#define CFGINT  (ADC_INT_PRI_7 | ADC_INT_SUB_PRI_2 | ADC_INT_ON)
#define CFGSCAN  0xC000 //enable all inputs for scan
#define INITCH  (ADC_CH0_POS_SAMPLEA_AN0 | ADC_CH0_NEG_SAMPLEA_NVREF)

// Parallel Master Port Configuration
#define PMP_CONTROL        PMP_READ_POL_LO | PMP_WRITE_POL_LO | PMP_CS2_CS1_EN
#define PMP_MODE              PMP_MODE_SLAVE | PMP_IRQ_OFF
#define PMP_PORT               (PMP_PEN_OFF)
#define PMP_INT                  (PMP_INT_OFF)


#define SYS_FREQ		(80000000)

#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
        //DDPCONbits.JTAGEN = 0; // Disable JTAG
        mPORTGSetPinsDigitalIn(0x00FF);
	mPORTBSetPinsAnalogIn(0x00FF);             //Enable all analog

	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
	CloseADC10();
	SetChanADC10(INITCH);
	OpenADC10(CONFIG1, CONFIG2, CONFIG3, CFGPORT, CFGSCAN);
	ConfigIntADC10(CFGINT);
	EnableADC10();

   	//Initialize the DB_UTILS IO channel
	DBINIT();

	// Display the introduction
	DBPRINTF("Welcome to the Analog Input Test.\n");
	DBPRINTF("The build date and time is (" __DATE__ "," __TIME__ ")\n");

	while (1){
		 //Get damper value from PIC analog input
		int i = 0;
		for(i;i<1;i++){
			int analog = ReadADC10(i);
			DBPRINTF("Hammer %d = %d", i, analog);
			DBPRINTF("\n");
		}
                int digital = mPORTGReadBits(BIT_0);
                DBPRINTF("digital: %X \n \n", digital );
	}
	return 0;
}