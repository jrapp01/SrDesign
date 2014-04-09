#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device__Mouse.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device__Mouse.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED="../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware/usb_descriptors.c" "../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware/mouse.c" "../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/USB/HID Device Driver/usb_function_hid.c" "../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/USB/usb_device.c"

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/73642316/usb_descriptors.o ${OBJECTDIR}/_ext/73642316/mouse.o ${OBJECTDIR}/_ext/1435207005/usb_function_hid.o ${OBJECTDIR}/_ext/1137633533/usb_device.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/73642316/usb_descriptors.o.d ${OBJECTDIR}/_ext/73642316/mouse.o.d ${OBJECTDIR}/_ext/1435207005/usb_function_hid.o.d ${OBJECTDIR}/_ext/1137633533/usb_device.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/73642316/usb_descriptors.o ${OBJECTDIR}/_ext/73642316/mouse.o ${OBJECTDIR}/_ext/1435207005/usb_function_hid.o ${OBJECTDIR}/_ext/1137633533/usb_device.o

# Source Files
SOURCEFILES=../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware/usb_descriptors.c ../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware/mouse.c ../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/USB/HID Device Driver/usb_function_hid.c ../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/USB/usb_device.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device__Mouse.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=32MX795F512L
MP_LINKER_FILE_OPTION=
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/73642316/usb_descriptors.o: ../../../../../../../Microchip\ Starter\ Kits/PIC32\ Starter\ Kits/USB\ Device\ -\ HID\ -\ Mouse/HID\ -\ Mouse\ -\ Firmware/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/73642316 
	@${RM} ${OBJECTDIR}/_ext/73642316/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/73642316/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/73642316/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include/USB" -I"." -MMD -MF "${OBJECTDIR}/_ext/73642316/usb_descriptors.o.d" -o ${OBJECTDIR}/_ext/73642316/usb_descriptors.o "../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware/usb_descriptors.c"   
	
${OBJECTDIR}/_ext/73642316/mouse.o: ../../../../../../../Microchip\ Starter\ Kits/PIC32\ Starter\ Kits/USB\ Device\ -\ HID\ -\ Mouse/HID\ -\ Mouse\ -\ Firmware/mouse.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/73642316 
	@${RM} ${OBJECTDIR}/_ext/73642316/mouse.o.d 
	@${RM} ${OBJECTDIR}/_ext/73642316/mouse.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/73642316/mouse.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include/USB" -I"." -MMD -MF "${OBJECTDIR}/_ext/73642316/mouse.o.d" -o ${OBJECTDIR}/_ext/73642316/mouse.o "../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware/mouse.c"   
	
${OBJECTDIR}/_ext/1435207005/usb_function_hid.o: ../../../../../../../Microchip\ Starter\ Kits/PIC32\ Starter\ Kits/Microchip/USB/HID\ Device\ Driver/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1435207005 
	@${RM} ${OBJECTDIR}/_ext/1435207005/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1435207005/usb_function_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1435207005/usb_function_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include/USB" -I"." -MMD -MF "${OBJECTDIR}/_ext/1435207005/usb_function_hid.o.d" -o ${OBJECTDIR}/_ext/1435207005/usb_function_hid.o "../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/USB/HID Device Driver/usb_function_hid.c"   
	
${OBJECTDIR}/_ext/1137633533/usb_device.o: ../../../../../../../Microchip\ Starter\ Kits/PIC32\ Starter\ Kits/Microchip/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1137633533 
	@${RM} ${OBJECTDIR}/_ext/1137633533/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1137633533/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1137633533/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include/USB" -I"." -MMD -MF "${OBJECTDIR}/_ext/1137633533/usb_device.o.d" -o ${OBJECTDIR}/_ext/1137633533/usb_device.o "../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/USB/usb_device.c"   
	
else
${OBJECTDIR}/_ext/73642316/usb_descriptors.o: ../../../../../../../Microchip\ Starter\ Kits/PIC32\ Starter\ Kits/USB\ Device\ -\ HID\ -\ Mouse/HID\ -\ Mouse\ -\ Firmware/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/73642316 
	@${RM} ${OBJECTDIR}/_ext/73642316/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/73642316/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/73642316/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include/USB" -I"." -MMD -MF "${OBJECTDIR}/_ext/73642316/usb_descriptors.o.d" -o ${OBJECTDIR}/_ext/73642316/usb_descriptors.o "../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware/usb_descriptors.c"   
	
${OBJECTDIR}/_ext/73642316/mouse.o: ../../../../../../../Microchip\ Starter\ Kits/PIC32\ Starter\ Kits/USB\ Device\ -\ HID\ -\ Mouse/HID\ -\ Mouse\ -\ Firmware/mouse.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/73642316 
	@${RM} ${OBJECTDIR}/_ext/73642316/mouse.o.d 
	@${RM} ${OBJECTDIR}/_ext/73642316/mouse.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/73642316/mouse.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include/USB" -I"." -MMD -MF "${OBJECTDIR}/_ext/73642316/mouse.o.d" -o ${OBJECTDIR}/_ext/73642316/mouse.o "../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware/mouse.c"   
	
${OBJECTDIR}/_ext/1435207005/usb_function_hid.o: ../../../../../../../Microchip\ Starter\ Kits/PIC32\ Starter\ Kits/Microchip/USB/HID\ Device\ Driver/usb_function_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1435207005 
	@${RM} ${OBJECTDIR}/_ext/1435207005/usb_function_hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1435207005/usb_function_hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1435207005/usb_function_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include/USB" -I"." -MMD -MF "${OBJECTDIR}/_ext/1435207005/usb_function_hid.o.d" -o ${OBJECTDIR}/_ext/1435207005/usb_function_hid.o "../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/USB/HID Device Driver/usb_function_hid.c"   
	
${OBJECTDIR}/_ext/1137633533/usb_device.o: ../../../../../../../Microchip\ Starter\ Kits/PIC32\ Starter\ Kits/Microchip/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1137633533 
	@${RM} ${OBJECTDIR}/_ext/1137633533/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1137633533/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1137633533/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/USB Device - HID - Mouse/HID - Mouse - Firmware" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include" -I"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/Include/USB" -I"." -MMD -MF "${OBJECTDIR}/_ext/1137633533/usb_device.o.d" -o ${OBJECTDIR}/_ext/1137633533/usb_device.o "../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/Microchip/USB/usb_device.c"   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device__Mouse.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PIC32MXSK=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device__Mouse.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PIC32MXSK=1,-L"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/MPLAB C32 Suite/lib",-L"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/MPLAB C32 Suite/pic32mx/lib",-L".",-Map="${DISTDIR}/USB_Device__Mouse.X.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device__Mouse.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device__Mouse.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-L"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/MPLAB C32 Suite/lib",-L"../../../../../../../Microchip Starter Kits/PIC32 Starter Kits/MPLAB C32 Suite/pic32mx/lib",-L".",-Map="${DISTDIR}/USB_Device__Mouse.X.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/USB_Device__Mouse.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif