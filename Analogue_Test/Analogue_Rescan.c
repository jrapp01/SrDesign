#include "starter_kit_tutorial.h"

void DelayMs(unsigned int msec);

int main(void){
        mPORTASetPinsDigitalIn(0xFFFF);
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
		int i = 0;
		for(i;i<16;i++){
			int damper = ReadADC10(i);
			DBPRINTF("Damper %d = %d", i, damper);
			DBPRINTF("\n");
		}

            //DBPRINTF("%d \n", LATA );
	}
	return 0;
}

//Simple delay function, no operation
void DelayMs(unsigned int msec)
{
    unsigned int tWait, tStart;
    tWait=(SYS_FREQ/2000)*msec;
    tStart=ReadCoreTimer();
    while((ReadCoreTimer()-tStart)<tWait);		// wait for the time to pass
}
