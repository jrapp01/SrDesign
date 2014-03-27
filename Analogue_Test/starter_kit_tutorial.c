#include "starter_kit_tutorial.h"
void DelayMs(unsigned int msec);
int AcquireMaxData(int ch);

int main(void)
{
	/******************* Setup *****************************/
	mPORTBSetPinsAnalogIn(0xFFFF);             //Enable all analog
	//mPORTDSetPinsDigitalOut(BIT_0 | BIT_1 | BIT_8);
	
	SYSTEMConfig(SYS_FREQ, SYS_CFG_WAIT_STATES | SYS_CFG_PCACHE);
	CloseADC10();
	SetChanADC10(INITCH);
	OpenADC10(CONFIG1, CONFIG2, CONFIG3, CFGPORT, CFGSCAN);
	ConfigIntADC10(CFGINT);
	EnableADC10();
	
	//Initialize the DB_UTILS IO channel
	DBINIT();

	// Display the introduction
	DBPRINTF("Welcome to the PIC32 Starter Kit Tutorial.\n");
	DBPRINTF("The build date and time is (" __DATE__ "," __TIME__ ")\n");

	
	/********************* Declarations *********************/
	
	int damper = 0; //ch1 binary read
	int damperchnum = 0; //damper adc number
	int hammerchnum = 1; //hammer adc number
	int samplesize = 1000; 
	
	int hammerdata[10][samplesize]; //collection of hammer values
	int damperdata[10][samplesize]; //collection of damper values
	
	int index = 0; //index of hammerdata and damperdata arrays
	int collection[2] = {0,0}; 
	int count = 0;
	
	/******************** Initialize Arrays ******************/
	int x; 
	int y;
	for( x = 0; x < 10; x++)
	{
		for( y = 0; y < samplesize; y++)
		{
			damperdata[x][y] = 0;
		}
	}
	
	/************************ Stuff to do *********************/
	while (count < 4){  
		damper = ReadADC10(damperchnum);
		if(damper > 100)
		{			
			//store data
			collection[0] = collection[1]; //we are collecting data, falling edge detection
			collection[1] = 1;
			if(index<samplesize)
			{
				hammerdata[count][index] = ReadADC10(hammerchnum);
				damperdata[count][index] = damper;
				index++;
				//DBPRINTF("Damper: %d \n", damperdata[count][index-1]);
			}
		}else{
			collection[0] = collection[1]; 
			collection[1] = 0;
		}
		if(collection[1] < collection[0]) //rising edge detection
		{
			DBPRINTF("Count: %d \n", count);
			count++;
			index = 0;
		}
	}
	
	int i;
	
	/******************** Output Memory at End **********************/
	for( i = 0; i < 1000; i++)
	{
		DBPRINTF("Damper data: %d ", damperdata[1][i]);
		DBPRINTF("\n");
	} 	

	return 0;
}

void DelayMs(unsigned int msec)
{
	unsigned int tWait, tStart;
		
    tWait=(SYS_FREQ/2000)*msec;
    tStart=ReadCoreTimer();
    while((ReadCoreTimer()-tStart)<tWait);		// wait for the time to pass

}

int AcquireMaxData(int ch)
{
	int max = 0; //boolean
	int maxvalue = 0; //max input value
	int adcread = 0;
	while(max = 0)
	{
		adcread = ReadADC10(ch);
		if (adcread > maxvalue)
		{
			maxvalue = adcread;
		}else
		{
			max = 1;
		}
	}
	return maxvalue;
}
