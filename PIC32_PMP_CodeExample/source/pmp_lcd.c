/*********************************************************************
 *
 *      PIC32MX PMP API Example
 *
 *********************************************************************
 * FileName:        pmp_lcd.c
 * Dependencies:    plib.h
 *
 * Processor:       PIC32MX
 *
 * Complier:        MPLAB C32
 *                  MPLAB IDE
 * Company:         Microchip Technology Inc.
 *
 * Software License Agreement
 *
 * The software supplied herewith by Microchip Technology Incorporated
 * (the ?Company?) for its PIC32MX Microcontroller is intended
 * and supplied to you, the Company?s customer, for use solely and
 * exclusively on Microchip Microcontroller products.
 * The software is owned by the Company and/or its supplier, and is
 * protected under applicable copyright laws. All rights are reserved.
 * Any use in violation of the foregoing restrictions may subject the
 * user to criminal sanctions under applicable laws, as well as to
 * civil liability for the breach of the terms and conditions of this
 * license.
 *
 * THIS SOFTWARE IS PROVIDED IN AN ?AS IS? CONDITION. NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING, BUT NOT LIMITED
 * TO, IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 * PARTICULAR PURPOSE APPLY TO THIS SOFTWARE. THE COMPANY SHALL NOT,
 * IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *********************************************************************
 * The purpose of this example code is to demonstrate the PIC32MX
 * peripheral library macros and functions supporting the PMP
 * module and its various features.
 *
 * Platform: Explorer-16 with PIC32MX PIM
 *
 * Features demonstrated:
 *    - Timer configuration
 *    - PMP functions
 *    - LCD control
 *
 * Description:
 *      This program writes text to the Explorer-16 LCD display using
 *      the PMP library.
 *
 * Notes:
 *    - The Explorer-16 LCD is compatible with the standard HD44780
 *      instruction set. The specific delays and instructions needed
 *      for proper LCD functionality can be found online.
 ********************************************************************/
#include <plib.h>
#include <xc.h>

#if defined (__32MX360F512L__) || (__32MX460F512L__) || (__32MX795F512L__) || (__32MX430F064L__) || (__32MX450F256L__) || (__32MX470F512L__)
// Configuration Bit settings
// SYSCLK = 80 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 80 MHz (SYSCLK / FPBDIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
#pragma config FPLLMUL = MUL_20, FPLLIDIV = DIV_2, FPLLODIV = DIV_1, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1
#define SYS_FREQ (80000000L)
// Declaring timer settings to use for delays
// -Frequency is 1/256 of the peripheral bus clock
// -(80 MHz * 1/256) = 312KHz = 3.2us per cycle
#define CONFIG      (T1_ON | T1_PS_1_256 | T1_SOURCE_INT)
#define PERIOD      0xFFFF

#elif defined (__32MX220F032D__) || (__32MX250F128D__)
// Configuration Bit settings
// SYSCLK = 48 MHz (8MHz Crystal / FPLLIDIV * FPLLMUL / FPLLODIV)
// PBCLK = 48 MHz (SYSCLK / FPBDIV)
// Primary Osc w/PLL (XT+,HS+,EC+PLL)
// WDT OFF
// Other options are don't care
#pragma config FPLLMUL = MUL_24, FPLLIDIV = DIV_2, FPLLODIV = DIV_2, FWDTEN = OFF
#pragma config POSCMOD = HS, FNOSC = PRIPLL, FPBDIV = DIV_1
#define SYS_FREQ (48000000L)
// Declaring timer settings to use for delays
// -Frequency is 1/256 of the peripheral bus clock
// -(48 MHz * 1/256) = 187KHz = 5.3us per cycle
#define CONFIG      (T1_ON | T1_PS_1_256 | T1_SOURCE_INT)
#define PERIOD      0xFFFF
#endif

// Enable PMP, master mode 1, long wait times to accommodate the LCD
#define CONTROL     (PMP_ON | PMP_READ_WRITE_EN | PMP_WRITE_POL_HI | PMP_READ_POL_HI)
#define MODE        (PMP_MODE_MASTER1 | PMP_WAIT_BEG_4 | PMP_WAIT_MID_15 | PMP_WAIT_END_4)
#define PORT        (PMP_PEN_0)
#define INTERRUPT   (PMP_INT_OFF)

#if defined (__32MX360F512L__) || (__32MX460F512L__) || (__32MX795F512L__) || (__32MX430F064L__) || (__32MX450F256L__) || (__32MX470F512L__)
    // Delay needed for LCD to process data
    void Delay_ms(unsigned long x) {
        WriteTimer1(0); // Clear the timer
        while (TMR1 < (315 * x)); // 3.2us x 315 = (~1ms delay * desired value)
    }

#elif defined (__32MX220F032D__) || (__32MX250F128D__)
    // Delay needed for LCD to process data
    void Delay_ms(unsigned long x) {
        WriteTimer1(0); // Clear the timer
        while (TMR1 < (190 * x)); // 5.3us x 190 = (~1ms delay * desired value)
    }
#endif

// Define aliases for the LCD data and command registers
#define DATAREG 1 // data register
#define CMDREG 0  // command register

// LCD initialization sequence
void initializeLCD(void) {
    mPMPOpen(CONTROL, MODE, PORT, INTERRUPT); // Initialize PMP using above settings
    
    OpenTimer1(CONFIG, PERIOD); // Enable Timer 1
    Delay_ms(30); // LCD needs 30ms to power on, perform startup functions, etc

    PMPSetAddress(CMDREG); // Access the LCD command register
    PMPMasterWrite(0x38); // LCD Function Set - 8-bit interface, 2 lines, 5*7 Pixels
    Delay_ms(1); // Needs a 40us delay to complete

    PMPMasterWrite(0x0C); // Turn on display (with cursor hidden)
    Delay_ms(1); // Needs a 40us delay to complete

    PMPMasterWrite(0x01); // Clear the display
    Delay_ms(2); // Needs a 1.64ms delay to complete

    PMPMasterWrite(0x06); // Set text entry mode - auto increment, no shift
    Delay_ms(1); // Needs a 40us delay to complete
}

// Write a byte of data to either of the two LCD registers (DATAREG, CMDREG)
void writeToLCD(int reg, char c) {
    Delay_ms(1); // 40us needed in between each character write
    PMPSetAddress(reg); // Select either 'DATAREG' or 'CMDREG'
    PMPMasterWrite(c); // Write the byte to selected register
}

// Used to write strings to the LCD
void writeString(unsigned char *string) {
    while (*string) { // Keep going until string ends
        writeToLCD(DATAREG, *string++); //Send characters one by one
    }
}

// Set the LCD cursor position to line two
void newLine(void) {
    writeToLCD(CMDREG, 0xC0); // Cursor address 0x80 + 0x40 = 0xC0
}

main(void) {

    // Configure cache, wait states and peripheral bus clock
    // Configure the device for maximum performance but do not change the PBDIV
    // Given the options, this function will change the flash wait states, RAM
    // wait state and enable prefetch cache but will not change the PBDIV.
    // The PBDIV value is already set via the pragma FPBDIV option above..
    SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);

    // Execute LCD initialization sequence
    initializeLCD();

    writeToLCD(CMDREG, 0x0F); // Turn on blinking cursor
    writeString("HELLO!"); // Write string to the LCD
    newLine(); // Set cursor to line two
    writeString("TESTING..."); // Print another string

    while(1);
}
