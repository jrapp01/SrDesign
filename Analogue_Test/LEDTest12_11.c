#include "starter_kit_tutorial.h"

void DelayMs(unsigned int msec);

int main(void){
	int damper = 0; //ch1 binary read
	int hammer = 0;
	int damperchnum = 0; //damper adc number
	int hammerchnum = 1; //hammer adc number
	int samplesize = 10; 
	mPORTBSetPinsAnalogIn(0xFFFF);             //Enable all analog
	
	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
	CloseADC10();
	SetChanADC10(INITCH);
	OpenADC10(CONFIG1, CONFIG2, CONFIG3, CFGPORT, CFGSCAN);
	ConfigIntADC10(CFGINT);
	EnableADC10(); 
	// LED setup
	// Turn off leds before configuring the IO pin as output
   	mPORTDClearBits(BIT_0 | BIT_1 | BIT_2);             // same as LATDCLR = 0x0007

	// Set RD0, RD1 and RD2 as outputs
   	mPORTDSetPinsDigitalOut(BIT_0 | BIT_1 | BIT_2 );    // same as TRISDCLR = 0x0007
 	
   	//Initialize the DB_UTILS IO channel
	DBINIT();
	
	// Display the introduction
	DBPRINTF("Welcome to the Force Sensor LED Test.\n");
	DBPRINTF("The build date and time is (" __DATE__ "," __TIME__ ")\n");
	int hammerdata[10][samplesize]; //collection of hammer values
	int damperdata[10][samplesize]; //collection of damper values
	int index = 0; //index of hammerdata and damperdata arrays
	int collection[2] = {0,0}; 
	int count = 0;

	while (1){
		//Get damper value from PIC analog input
		damper = ReadADC10(damperchnum);

	/*	DBPRINTF("Damper = %d", damper);
		DBPRINTF("\n");*/
		
		//Compare damper to threshold
		//If above threshold, damper is "released" from piano
		//Hammer hit may occur		
		if(damper > 500){
			collection[0] = collection[1]; //we are collecting data, falling edge detection
			collection[1] = 1;

			//Get analog input hammer data, find max value for displaying via LEDs
			hammer = AcquireMaxData(hammerchnum);
		//	DBPRINTF("hammer = %d \n", hammer);
		//	DBPRINTF("");
			if(hammer > 255 && hammer <= 511){
				mPORTDToggleBits(BIT_2);
			}else if(hammer > 511 && hammer <=767){
				mPORTDToggleBits(BIT_1); 
				mPORTDToggleBits(BIT_2); 
			}else if(hammer >767){
				mPORTDToggleBits(BIT_1);
				mPORTDToggleBits(BIT_2);
				mPORTDToggleBits(BIT_0);
			}	

		//Damper "on"--hammer cannot hit
		//not currently collecting data
		}else{
 			collection[0] = collection[1]; 
			collection[1] = 0;
		}

		//If damper was previously off, allowing hammer hits, but is now on, preventing hits,
		// we have a falling edge in collection
		// Print to screen peak value of hammer hit from when damper was off
		if(collection[1] < collection[0]) //edge detection
		{
			DBPRINTF("Damper = %d Hammer = %d", damper,hammer);
			DBPRINTF("\n");	
		}
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
	int max = 0; //boolean
	int maxvalue = 0; //max input value
	int adcread = 0;
	while(max == 0) //while max value has not been found
	{
		adcread = ReadADC10(ch);
		if (adcread > maxvalue || adcread <40)
		{
			maxvalue = adcread;
		}else
		{
			max = 1;
		}
	}
	return maxvalue;
}
