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
*   File: usb_descriptors.c
*   Description: Contains USB descriptors for single MIDI class device
*-----------------------------------------------------------------------------*/

#include <USB/usb.h>

/* Device Descriptor */
ROM USB_DEVICE_DESCRIPTOR device_dsc=
{
    0x12,                   // Size of this descriptor in bytes
    USB_DESCRIPTOR_DEVICE,  // DEVICE descriptor type
    0x0200,                 // USB Spec Release Number in BCD format
    0x00,                   // Class Code
    0x00,                   // Subclass code
    0x00,                   // Protocol code
    USB_EP0_BUFF_SIZE,      // Max packet size for EP0, see usb_config.h
    0x04D8,                 // Vendor ID
    0x0059,                 // Product ID: Audio MIDI example
    0x0002,                 // Device release number in BCD format
    0x01,                   // Manufacturer string index
    0x02,                   // Product string index
    0x00,                   // Device serial number string index
    0x01                    // Number of possible configurations
};

/* Configuration 1 Descriptor */
/* copied from the USB Device Class Specification for MIDI Devices */
ROM BYTE configDescriptor1[]={
    /* Configuration Descriptor */
    0x09,                                   // Size of this descriptor in bytes
    USB_DESCRIPTOR_CONFIGURATION,           // CONFIGURATION descriptor type
    101,0x00,                               // Total length of data for this cfg
    2,                                      // Number of interfaces in this cfg
    1,                                      // Index value of this configuration
    0,                                      // Configuration string index
    _DEFAULT | _SELF,                       // Attributes, see usb_device.h
    50,                                     // Max power consumption (2X mA)
							
    /* Interface Descriptor */
    0x09,//sizeof(USB_INTF_DSC),             // Size of this descriptor in bytes
    USB_DESCRIPTOR_INTERFACE,                // INTERFACE descriptor type
    0,                                       // Interface Number
    0,                                       // Alternate Setting Number
    0,                                       // Number of endpoints in this intf
    1,                                       // Class code
    1,                                       // Subclass code
    0,                                       // Protocol code
    0,                                       // Interface string index

    /* MIDI Adapter Class-specific AC Interface Descriptor */
    0x09,       //bLength
    0x24,       //bDescriptorType - CS_INTERFACE
    0x01,       //bDescriptorSubtype - HEADER
    0x00,0x01,  //bcdADC
    0x09,0x00,  //wTotalLength
    0x01,       //bInCollection
    0x01,       //baInterfaceNr(1)

    /* MIDI Adapter Standard MS Interface Descriptor */
    0x09,       //bLength
    0x04,       //bDescriptorType
    0x01,       //bInterfaceNumber
    0x00,       //bAlternateSetting
    0x02,       //bNumEndpoints
    0x01,       //bInterfaceClass
    0x03,       //bInterfaceSubclass
    0x00,       //bInterfaceProtocol
    0x00,       //iInterface

    /* MIDI Adapter Class-specific MS Interface Descriptor */
    0x07,       //bLength
    0x24,       //bDescriptorType - CS_INTERFACE
    0x01,       //bDescriptorSubtype - MS_HEADER
    0x00,0x01,  //BcdADC
    0x41,0x00,  //wTotalLength

    /* MIDI Adapter MIDI IN Jack Descriptor (Embedded) */
    0x06,       //bLength
    0x24,       //bDescriptorType - CS_INTERFACE
    0x02,       //bDescriptorSubtype - MIDI_IN_JACK
    0x01,       //bJackType - EMBEDDED
    0x01,       //bJackID
    0x00,       //iJack

    /* MIDI Adapter MIDI IN Jack Descriptor (External) */
    0x06,       //bLength
    0x24,       //bDescriptorType - CS_INTERFACE
    0x02,       //bDescriptorSubtype - MIDI_IN_JACK
    0x02,       //bJackType - EXTERNAL
    0x02,       //bJackID
    0x00,       //iJack

    /* MIDI Adapter MIDI OUT Jack Descriptor (Embedded) */
    0x09,       //bLength
    0x24,       //bDescriptorType - CS_INTERFACE
    0x03,       //bDescriptorSubtype - MIDI_OUT_JACK
    0x01,       //bJackType - EMBEDDED
    0x03,       //bJackID
    0x01,       //bNrInputPins
    0x02,       //BaSourceID(1)
    0x01,       //BaSourcePin(1)
    0x00,       //iJack

    /* MIDI Adapter MIDI OUT Jack Descriptor (External) */
    0x09,       //bLength
    0x24,       //bDescriptorType - CS_INTERFACE
    0x03,       //bDescriptorSubtype - MIDI_OUT_JACK
    0x02,       //bJackType - EXTERNAL
    0x04,       //bJackID
    0x01,       //bNrInputPins
    0x01,       //BaSourceID(1)
    0x01,       //BaSourcePin(1)
    0x00,       //iJack

    /* MIDI Adapter Standard Bulk OUT Endpoint Descriptor */
    0x09,       //bLength
    0x05,       //bDescriptorType - ENDPOINT
    0x01,       //bEndpointAddress - OUT
    0x02,       //bmAttributes
    0x40,0x00,  //wMaxPacketSize
    0x00,       //bInterval
    0x00,       //bRefresh
    0x00,       //bSynchAddress

    /* MIDI Adapter Class-specific Bulk OUT Endpoint Descriptor */
    0x05,       //bLength
    0x25,       //bDescriptorType - CS_ENDPOINT
    0x01,       //bDescriptorSubtype - MS_GENERAL
    0x01,       //bNumEmbMIDIJack
    0x01,       //BaAssocJackID(1)

    /* MIDI Adapter Standard Bulk IN Endpoint Descriptor */
    0x09,       //bLength
    0x05,       //bDescriptorType
    0x81,       //bEndpointAddress
    0x02,       //bmAttributes
    0x40,0x00,  //wMaxPacketSize
    0x00,       //bInterval
    0x00,       //bRefresh
    0x00,       //bSynchAddress

    /* MIDI Adapter Class-specific Bulk IN Endpoint Descriptor */
    0x05,       //bLength
    0x25,       //bDescriptorType - CS_ENDPOINT
    0x01,       //bDescriptorSubtype - MS_GENERAL
    0x01,       //bNumEmbMIDIJack
    0x03        //BaAssocJackID(1)
};


//Language code string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[1];}sd000={
sizeof(sd000),USB_DESCRIPTOR_STRING,{0x0409
}};

//Manufacturer string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[25];}sd001={
sizeof(sd001),USB_DESCRIPTOR_STRING,
{'M','i','c','r','o','c','h','i','p',' ',
'T','e','c','h','n','o','l','o','g','y',' ','I','n','c','.'
}};

//Product string descriptor
ROM struct{BYTE bLength;BYTE bDscType;WORD string[9];}sd002={
    sizeof(sd002),USB_DESCRIPTOR_STRING,
    {'P','I','A','N','O','M','I','D','I'}
};

//Array of configuration descriptors
ROM BYTE *ROM USB_CD_Ptr[]=
{
    (ROM BYTE *ROM)&configDescriptor1
};

//Array of string descriptors
ROM BYTE *ROM USB_SD_Ptr[]=
{
    (ROM BYTE *ROM)&sd000,
    (ROM BYTE *ROM)&sd001,
    (ROM BYTE *ROM)&sd002
};
