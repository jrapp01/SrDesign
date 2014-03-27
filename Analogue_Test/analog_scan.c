#include "starter_kit_tutorial.h"

void DelayMs(unsigned int msec);
int AcquireMaxData(int ch);

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

//Find max by comparing to previous max values

int AcquireMaxData(int ch)
{
	int maxValue = 0; 	//max input value
	int previous = 0;	//saves sample f(n-1) to ensure that 
						//max value is not value of next sample after reseting to zero
	int threshold = 40;	//somewhat arbitrary threshold based on values of floating pins

	int adcRead = ReadADC10(ch);	//get next sample 

	//Make sure that peak detection only starts for signal 
	// above threshold and with an upward slope
	if (adcRead >= threshold && adcRead > previous){
		while(adcRead >= maxValue){
			maxValue = adcRead; //continuously update max as amplitude increases
			adcRead = ReadADC10(ch);
		}
		previous = adcRead;
		maxValue = 0; //should not set to 0 before returning value
	}
	return maxValue;
}
