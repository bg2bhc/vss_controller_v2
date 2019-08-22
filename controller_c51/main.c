#include "STC15.H"

#define MOTOR_EN	P11
#define MAG_EN		P10

#define MAG_DET		P37
#define TRIGGER		!P36
#define COUNTER		!P33
#define MODE			!P32

#define TIMEOUT_COUNTER	500
#define PREFEEDING			2000
#define JITTER_TRIGGER	3
#define JITTER_COUNTER	3
#define JITTER_MAG			300

void delayms(unsigned int ms)
{
	unsigned char j, k;
	
	for(; ms>0; ms--)
	{
		for(j=0; j<7; j++)
		{
			for(k=0; k<97; k++);
		}
	}
}

void wait4counter(unsigned int ms)
{
	for(; ms>0; ms--)
	{
		if(COUNTER)
		{
			delayms(JITTER_COUNTER);
			
			if(COUNTER)
			{
				break;
			}
			else
			{
				continue;
			}
		}
		delayms(1);
	}
}

void wait4notcounter(unsigned int ms)
{
	for(; ms>0; ms--)
	{
		if(!COUNTER)
		{
			delayms(JITTER_COUNTER);
			
			if(!COUNTER)
			{
				break;
			}
			else
			{
				continue;
			}
		}
		delayms(1);
	}
}

void main()
{
	unsigned char flag_new_mag = 0;
	
	MOTOR_EN = 0;
	MAG_EN = 0;
	
	P37 = 1; // MAG_DET	
	P36 = 1; // TRIGGER
	P33 = 1; // COUNTER
	P32 = 1; // MODE
		
	P1M0  = 0x03;
	P1M1  = 0x00;
	P3M0  = 0x00;
	P3M1  = 0x80;
	
	while(1)
	{
		if(TRIGGER)
		{
			delayms(JITTER_TRIGGER);
			
			if(TRIGGER)
			{
				MOTOR_EN = 1;
				MAG_EN = 1;
				
				wait4notcounter(TIMEOUT_COUNTER);
				
				if(MODE)
				{
					while(1)
					{
						if(!TRIGGER)
						{
							delayms(JITTER_TRIGGER);
							
							if(!TRIGGER)
							{
								break;
							}
							else
							{
								continue;
							}
						}
					}			
					
					wait4counter(TIMEOUT_COUNTER);
					
					MOTOR_EN = 0;
					MAG_EN = 0;
				}
				else
				{
					wait4counter(TIMEOUT_COUNTER);
					
					MOTOR_EN = 0;
					MAG_EN = 0;
					
					while(1)
					{
						if(!TRIGGER)
						{
							delayms(JITTER_TRIGGER);
							
							if(!TRIGGER)
							{
								break;
							}
							else
							{
								continue;
							}
						}
					}
				}
			}
			else
			{
				continue;
			}
		}
		
		if(!MAG_DET)
		{
			delayms(JITTER_MAG);
			
			if(!MAG_DET)
			{
				flag_new_mag = 1;
			}
			else
			{
				continue;
			}
		}
		
		if(MAG_DET && flag_new_mag)
		{
			delayms(JITTER_MAG);
			
			if(MAG_DET)
			{
				MAG_EN = 1;
				delayms(PREFEEDING);
				MAG_EN = 0;
				flag_new_mag = 0;
			}
			else
			{
				continue;
			}
		}
	}
}

