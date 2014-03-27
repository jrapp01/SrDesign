#include "starter_kit_tutorial.h"

void DelayMs(unsigned int msec);

int main(void){
	mPORTBSetPinsAnalogIn(0xFFFF);             //Enable all analog
	
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
	
		DBPRINTF("Damper");
		DBPRINTF("\n");
	}
	return 0;
}

