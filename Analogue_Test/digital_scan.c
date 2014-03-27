#include "starter_kit_tutorial.h"

void DelayMs(unsigned int msec);

int main(void){
        mPORTASetPinsDigitalIn(0xFFFF);
	
   	//Initialize the DB_UTILS IO channel
	DBINIT();
	
	// Display the introduction
	DBPRINTF("Welcome to the Analog Input Test.\n");
	DBPRINTF("The build date and time is (" __DATE__ "," __TIME__ ")\n");

	while (1){
            DBPRINTF("%X \n", mPORTARead() );
            DelayMs(1000);
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
