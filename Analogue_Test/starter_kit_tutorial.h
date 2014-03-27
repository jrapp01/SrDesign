/***********************************************************************************
						
						PIC32MX Starter Kit Example Tutorial

 ***********************************************************************************
 * FileName:        starter_kit_tutorial.c
 * Dependencies:    None
 * Company:         Microchip Technology, Inc.
 *
 * Copyright (c) 2008 Microchip Technology, Inc.
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the “Company”) for its PIC32 Microcontroller is intended
 * and supplied to you, the Company’s customer, for use solely and
 * exclusively on Microchip PIC32 Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN “AS IS” CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ***************************************************************************
 * Description:
 *			This tutorial introduces the PIC32 microcontroller and provides
 *			a basic	"hands-on" experience with PIC32 Starter Kit, MPLAB IDE,
 *			MPLAB C compiler for PIC32, PIC32 Peripheral Libraries and 
 *          Debug Print Library.
 *
 *			The application demonstrates a simple console IO application that
 *			turns on/off the PIC32MX Starter Kit LEDs and echos text when the
 *          user responds to a menu hosted in MPLAB IDE Starter Kit console window.
 *
 * Additional Info:
 *			Refer to the PIC32MX Starter Kit Users Guide, DS61144B or later, and
 *          the ReleaseNotes for additional details.
 *
 * Platforms:
 *			PIC32MX Starter Kit DM320001
 *			PIC32MX USB Starter Kit DM320003
 *
 * Tools:
 *			1. MPLAB IDE 8.20 or higher
 *			2. MPLAB C Compiler for PIC32 v1.04 or higher
 *			3. General Purpose Starter Kit DM320001 or USB Starter board DM320003
 *			4. USB Cable
 *
 * Debug Print Library: SUPPORTED IN DEBUG MODE ONLY
 *			The Debug Print Library adds user debugging input/output capability
 *          to the MPLAB IDE Starter Kit console window using DBPRINTF, DPUTS,
 *          DBGETC and DBGETS.
 *
 *          To use the Debug Print Library with the PIC32 Starter Kit, do the following:
 *			1. Add '#include "db_utils.h"' to the source code.
 *			   Note: you may need to copy db_utils.h into your working directory or
 *			   specify	a path to its location.
 *			   It is located in "<install dir>\PIC32 Starter Kits\Microchip\Include".
 *			2. Add 'db_utils.a' to the library branch of MPLAB IDE project tree.
 *			   It is located in "<install dir>\PIC32 Starter Kits\Microchip\Common"
 *			3. Add "PIC32_STARTER_KIT" macro definition to the project build options
 *			   for the C compiler.
 *			4. Be sure to call DBINIT() first before any other Debug Print Library macros.
 *
 * Starter Board Resources:
 *			LED1 (RED)		= PORTD.RD0
 *			LED2 (YELLOW)	= PORTD.RD1
 *			LED3 (GREEN)	= PORTD.RD2
 *			SW1				= PORTD.RD6  (change notice pin CN15)
 *			SW2				= PORTD.RD7	 (change notice pin CN16)
 *			SW3				= PORTD.RD13 (change notice pin CN19)
 *			JTAG.TMS		= PORTA.RA0  (used by debugger)
 *			JTAG.TCK		= PORTA.RA1  (used by debugger)
 *			JTAG.TDO		= PORTA.RA5  (used by debugger)
 *			JTAG.TDI		= PORTA.RA4  (used by debugger)
 *			PGC2			= PORTB.RB6	 (used by debugger)
 *			PGD2			= PORTB.RB7  (used by debugger)
 *
 * Starter Board Notes:
 *			1.	There are no pullup resistors on SW1, SW2 or SW3.
 *				To enable the PORTD pullups for SW1, SW2 and SW3, use the following:
 *				CNPUE = (CN15_PULLUP_ENABLE | CN16_PULLUP_ENABLE | CN19_PULLUP_ENABLE);
 *			2.  Do not disable the PIC32MX JTAG.  This will prevent the PIC32MX Starter Kit
 *				debugger(PIC18F4550) from communicating with the PIC32MX device.
 *			3.  Do not configure the SYSTEM CLOCK to operate faster than 80MHz.
  *****************************************************************************
 * Change History:
 * ID			Date            Changes
 * C11842		12/01/2008		Modified for 80MHz operation, Updated comments
 *****************************************************************************/

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
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select
#pragma config DEBUG    = OFF           // Debugger Disabled for Starter Kit
            
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
