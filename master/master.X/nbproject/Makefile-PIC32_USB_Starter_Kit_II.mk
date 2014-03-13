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
ifeq "$(wildcard nbproject/Makefile-local-PIC32_USB_Starter_Kit_II.mk)" "nbproject/Makefile-local-PIC32_USB_Starter_Kit_II.mk"
include nbproject/Makefile-local-PIC32_USB_Starter_Kit_II.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=PIC32_USB_Starter_Kit_II
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/master.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/master.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=C:/microchip_solutions_v2013-06-15/Microchip/USB/usb_device.c ../src/delay.c ../src/main.c ../src/usb_descriptors.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1961590870/usb_device.o ${OBJECTDIR}/_ext/1360937237/delay.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1961590870/usb_device.o.d ${OBJECTDIR}/_ext/1360937237/delay.o.d ${OBJECTDIR}/_ext/1360937237/main.o.d ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1961590870/usb_device.o ${OBJECTDIR}/_ext/1360937237/delay.o ${OBJECTDIR}/_ext/1360937237/main.o ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o

# Source Files
SOURCEFILES=C:/microchip_solutions_v2013-06-15/Microchip/USB/usb_device.c ../src/delay.c ../src/main.c ../src/usb_descriptors.c


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
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-PIC32_USB_Starter_Kit_II.mk dist/${CND_CONF}/${IMAGE_TYPE}/master.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

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
${OBJECTDIR}/_ext/1961590870/usb_device.o: C:/microchip_solutions_v2013-06-15/Microchip/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1961590870 
	@${RM} ${OBJECTDIR}/_ext/1961590870/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1961590870/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1961590870/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I".." -I"C:/Program Files (x86)/Microchip/xc32/v1.31/pic32-libs/include" -I"C:/Microchip MLA/Microchip/Include" -I"../include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15" -I"../../../../../../../microchip_solutions_v2013-06-15/USB/Device - Audio - MIDI/Firmware" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -MMD -MF "${OBJECTDIR}/_ext/1961590870/usb_device.o.d" -o ${OBJECTDIR}/_ext/1961590870/usb_device.o C:/microchip_solutions_v2013-06-15/Microchip/USB/usb_device.c   
	
${OBJECTDIR}/_ext/1360937237/delay.o: ../src/delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/delay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I".." -I"C:/Program Files (x86)/Microchip/xc32/v1.31/pic32-libs/include" -I"C:/Microchip MLA/Microchip/Include" -I"../include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15" -I"../../../../../../../microchip_solutions_v2013-06-15/USB/Device - Audio - MIDI/Firmware" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -MMD -MF "${OBJECTDIR}/_ext/1360937237/delay.o.d" -o ${OBJECTDIR}/_ext/1360937237/delay.o ../src/delay.c   
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I".." -I"C:/Program Files (x86)/Microchip/xc32/v1.31/pic32-libs/include" -I"C:/Microchip MLA/Microchip/Include" -I"../include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15" -I"../../../../../../../microchip_solutions_v2013-06-15/USB/Device - Audio - MIDI/Firmware" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c   
	
${OBJECTDIR}/_ext/1360937237/usb_descriptors.o: ../src/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PIC32MXSK=1 -fframe-base-loclist  -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I".." -I"C:/Program Files (x86)/Microchip/xc32/v1.31/pic32-libs/include" -I"C:/Microchip MLA/Microchip/Include" -I"../include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15" -I"../../../../../../../microchip_solutions_v2013-06-15/USB/Device - Audio - MIDI/Firmware" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -MMD -MF "${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d" -o ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o ../src/usb_descriptors.c   
	
else
${OBJECTDIR}/_ext/1961590870/usb_device.o: C:/microchip_solutions_v2013-06-15/Microchip/USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1961590870 
	@${RM} ${OBJECTDIR}/_ext/1961590870/usb_device.o.d 
	@${RM} ${OBJECTDIR}/_ext/1961590870/usb_device.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1961590870/usb_device.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I".." -I"C:/Program Files (x86)/Microchip/xc32/v1.31/pic32-libs/include" -I"C:/Microchip MLA/Microchip/Include" -I"../include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15" -I"../../../../../../../microchip_solutions_v2013-06-15/USB/Device - Audio - MIDI/Firmware" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -MMD -MF "${OBJECTDIR}/_ext/1961590870/usb_device.o.d" -o ${OBJECTDIR}/_ext/1961590870/usb_device.o C:/microchip_solutions_v2013-06-15/Microchip/USB/usb_device.c   
	
${OBJECTDIR}/_ext/1360937237/delay.o: ../src/delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/delay.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/delay.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I".." -I"C:/Program Files (x86)/Microchip/xc32/v1.31/pic32-libs/include" -I"C:/Microchip MLA/Microchip/Include" -I"../include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15" -I"../../../../../../../microchip_solutions_v2013-06-15/USB/Device - Audio - MIDI/Firmware" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -MMD -MF "${OBJECTDIR}/_ext/1360937237/delay.o.d" -o ${OBJECTDIR}/_ext/1360937237/delay.o ../src/delay.c   
	
${OBJECTDIR}/_ext/1360937237/main.o: ../src/main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I".." -I"C:/Program Files (x86)/Microchip/xc32/v1.31/pic32-libs/include" -I"C:/Microchip MLA/Microchip/Include" -I"../include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15" -I"../../../../../../../microchip_solutions_v2013-06-15/USB/Device - Audio - MIDI/Firmware" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -MMD -MF "${OBJECTDIR}/_ext/1360937237/main.o.d" -o ${OBJECTDIR}/_ext/1360937237/main.o ../src/main.c   
	
${OBJECTDIR}/_ext/1360937237/usb_descriptors.o: ../src/usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1360937237 
	@${RM} ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d 
	@${RM} ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d" $(SILENT) -rsi ${MP_CC_DIR}../  -c ${MP_CC}  $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DPIC32_USB_STARTER_KIT -I".." -I"C:/Program Files (x86)/Microchip/xc32/v1.31/pic32-libs/include" -I"C:/Microchip MLA/Microchip/Include" -I"../include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15" -I"../../../../../../../microchip_solutions_v2013-06-15/USB/Device - Audio - MIDI/Firmware" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -MMD -MF "${OBJECTDIR}/_ext/1360937237/usb_descriptors.o.d" -o ${OBJECTDIR}/_ext/1360937237/usb_descriptors.o ../src/usb_descriptors.c   
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compileCPP
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/master.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PIC32MXSK=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/master.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PIC32MXSK=1,-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	
else
dist/${CND_CONF}/${IMAGE_TYPE}/master.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/master.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}          -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),-Map="${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"
	${MP_CC_DIR}\\xc32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/master.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} 
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/PIC32_USB_Starter_Kit_II
	${RM} -r dist/PIC32_USB_Starter_Kit_II

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
