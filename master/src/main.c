/*------------------------------------------------------------------------------
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
 *   Description: Contains primary functions of the master MCU, including
 *   handling of PMP communication with slaves and USB communication with host
------------------------------------------------------------------------------*/

#include <usb/usb.h>
#include <usb/usb_function_midi.h>
#include <plib.h>

//------------------------------Configuration-----------------------------------

#pragma config UPLLEN   = ON            // USB PLL Enabled
#pragma config FPLLMUL  = MUL_20        // PLL Multiplier
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_1         // PLL Output Divider
#pragma config FPBDIV   = DIV_8         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS1           // Watchdog Timer Postscale
#pragma config OSCIOFNC = OFF           // CLKO Enable
#pragma config POSCMOD  = HS            // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable
#pragma config FNOSC    = PRIPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select

#define PMP_CONTROL        (PMP_ON | PMP_IDLE_CON | PMP_READ_WRITE_EN | \
                            PMP_MUX_OFF | PMP_WRITE_POL_LO | \
                            PMP_READ_POL_LO | PMP_LATCH_POL_HI | \
                            PMP_CS2_CS1_EN | PMP_CS2_POL_LO | PMP_CS1_POL_LO)

#define PMP_MODE           (PMP_IRQ_OFF | PMP_AUTO_ADDR_OFF | \
                            PMP_DATA_BUS_8 | PMP_MODE_MASTER2 | PMP_WAIT_BEG_4|\
                            PMP_WAIT_MID_15 | PMP_WAIT_END_4)

#define PMP_PORT           (PMP_PEN_ALL)

#define PMP_INT            (PMP_INT_OFF)


//------------------------------USB Globals-------------------------------------

#define RX_BUFFER_ADDRESS_TAG
#define TX_BUFFER_ADDRESS_TAG
#define MIDI_EVENT_ADDRESS_TAG


unsigned char ReceivedDataBuffer[64] RX_BUFFER_ADDRESS_TAG;
unsigned char ToSendDataBuffer[64] TX_BUFFER_ADDRESS_TAG;
USB_AUDIO_MIDI_EVENT_PACKET midiData MIDI_EVENT_ADDRESS_TAG;

USB_HANDLE USBTxHandle = 0;
USB_HANDLE USBRxHandle = 0;


USB_VOLATILE BYTE msCounter;

static void InitializeSystem(void);
void UserInit(void);
void USBCBSendResume(void);

//------------------------------PMP Handler-------------------------------------
void __ISR(_EXTERNAL_0_VECTOR, IPL6) GetSlaveData(void){
    unsigned char pitch, vel;
    PMPMasterRead();
    pitch = mPMPMasterReadByte();
    
    vel = mPMPMasterReadByte();

    if(!USBHandleBusy(USBTxHandle))
    {
        midiData.Val = 0;   //must set all unused values to 0

        midiData.CableNumber = 0;
        midiData.CodeIndexNumber = MIDI_CIN_NOTE_ON;
        midiData.DATA_0 = 0x90;                     //Note on
        midiData.DATA_1 = pitch + 60;               //pitch
        midiData.DATA_2 = vel;                      //velocity

        USBTxHandle = USBTxOnePacket(MIDI_EP,(BYTE*)&midiData,4);
    }

    mINT0ClearIntFlag();
}

int main(void)
{
    InitializeSystem();
    USBDeviceAttach();
    while(1)
    {
    }
}

static void InitializeSystem(void)
{
    AD1PCFG = 0xFFFF;   
    SYSTEMConfigPerformance(80000000);
    UserInit();
    USBDeviceInit();	// Initializes USB module SFRs and firmware
    			// variables to known states.
    ConfigINT0(EXT_INT_PRI_6 | RISING_EDGE_INT | EXT_INT_ENABLE);
    
    mPMPOpen(PMP_CONTROL, PMP_MODE, PMP_PORT, PMP_INT);
    PMPSetAddress(0x4000);
}

void UserInit(void)
{
    // Initialize the handle for the last transmission
    USBTxHandle = NULL;
    USBRxHandle = NULL;
}

//------------------------------USB Callback Functions -------------------------

void USBCBSuspend(void)
{
}

void USBCB_SOF_Handler(void)
{
    if(msCounter != 0)
    {
        msCounter--;
    }
}

void USBCBErrorHandler(void)
{
}

void USBCBCheckOtherReq(void)
{
}

void USBCBStdSetDscHandler(void)
{
}

void USBCBInitEP(void)
{
    // enable the MIDI endpoint
    USBEnableEndpoint(MIDI_EP, 
            USB_OUT_ENABLED | USB_IN_ENABLED | USB_HANDSHAKE_ENABLED |\
            USB_DISALLOW_SETUP);

    // Re-arm the endpoint for the next packet
    USBRxHandle = USBRxOnePacket(MIDI_EP,(BYTE*)&ReceivedDataBuffer,64);
}

void USBCBSendResume(void)
{
    static WORD delay_count;

    if(USBGetRemoteWakeupStatus() == TRUE) 
    {
        // Verify that the USB bus is in fact suspended, before we send
        // remote wakeup signalling.
        if(USBIsBusSuspended() == TRUE)
        {
            USBMaskInterrupts();
            
            // Clock switch to settings consistent with normal USB operation.
            // USBCBWakeFromSuspend();
            USBSuspendControl = 0; 
            USBBusIsSuspended = FALSE;  // So we don't execute this code again,
                                        // until a new suspend condition is
                                        // detected.

            // Section 7.1.7.7 of the USB 2.0 specifications indicates a USB
            // device must continuously see 5ms+ of idle on the bus, before it
            // sends remote wakeup signalling.  One way to be certain that this
            // parameter gets met, is to add a 2ms+ blocking delay here
            // (2ms plus at least 3ms from bus idle to
            // USBIsBusSuspended() == TRUE, yeilds 5ms+ total delay since start
            // of idle).
            delay_count = 3600U;        
            do
            {
                delay_count--;
            }while(delay_count);
            
            //Now drive the resume K-state signalling onto the USB bus.
            USBResumeControl = 1;       // Start RESUME signaling
            delay_count = 1800U;        // Set RESUME line for 1-13 ms
            do
            {
                delay_count--;
            }while(delay_count);
            USBResumeControl = 0;       //Finished driving resume signalling

            USBUnmaskInterrupts();
        }
    }
}

BOOL USER_USB_CALLBACK_EVENT_HANDLER(int event, void *pdata, WORD size)
{
    switch( event )
    {
        case EVENT_TRANSFER:
            //Add application specific callback task or callback function here if desired.
            break;
        case EVENT_SOF:
            USBCB_SOF_Handler();
            break;
        case EVENT_SUSPEND:
            USBCBSuspend();
            break;
        case EVENT_RESUME:
            //USBCBWakeFromSuspend();
            break;
        case EVENT_CONFIGURED:
            USBCBInitEP();
            break;
        case EVENT_SET_DESCRIPTOR:
            USBCBStdSetDscHandler();
            break;
        case EVENT_EP0_REQUEST:
            USBCBCheckOtherReq();
            break;
        case EVENT_BUS_ERROR:
            USBCBErrorHandler();
            break;
        case EVENT_TRANSFER_TERMINATED:
            break;
        default:
            break;
    }
    return TRUE;
}
