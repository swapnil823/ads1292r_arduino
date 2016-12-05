#include "ads1292r.h"
#include <SPI.h>

ads1292r ADS1292;

volatile byte SPI_RX_Buff[150] ;
volatile static int SPI_RX_Buff_Count = 0;
volatile char *SPI_RX_Buff_Ptr;
volatile int Responsebyte = false;
volatile unsigned int pckt =0 , buff=0,t=0 , j1=0,j2=0;
volatile unsigned long int EEG_Ch1_Data[150],EEG_Ch2_Data[150];
volatile unsigned char datac[150];
unsigned long ueegtemp = 0,Pkt_Counter=0;
signed long seegtemp=0;
volatile int i;

void setup() 
{
	// initalize the  data ready and chip select pins:
	pinMode(ADS1292_DRDY_PIN, INPUT);  //6
	pinMode(ADS1292_CS_PIN, OUTPUT);    //7
	pinMode(ADS1292_START_PIN, OUTPUT);  //5
	pinMode(ADS1292_PWDN_PIN, OUTPUT);  //4
  
	
	ADS1292.ads1292_TestInit();
}

void loop() 
{
	if((digitalRead(ADS1292_DRDY_PIN)) == LOW)
	{  
		SPI_RX_Buff_Ptr = ADS1292.ads1292_Read_Data();
		Responsebyte = true; 
	}

	if(Responsebyte == true)
	{
		for(i = 0; i < 9; i++)
		{
			SPI_RX_Buff[SPI_RX_Buff_Count++] = *(SPI_RX_Buff_Ptr + i);
		}
		Responsebyte = false;
	}
  
	if(SPI_RX_Buff_Count >= 9)
	{     
		pckt = 0;	j1=0;		j2=0;
		for(i=3;i<9;i+=9)
		{
			//udi_cdc_putc(SPI_RX_Buff[i]);
			//EEG_Ch1_Data[j1++]=  SPI_RX_Buff[i+0];
			//EEG_Ch1_Data[j1++]= SPI_RX_Buff[i+1];
			//EEG_Ch1_Data[j1++]= SPI_RX_Buff[i+2];
		


			EEG_Ch2_Data[j2++]= (unsigned char)SPI_RX_Buff[i+3];
			EEG_Ch2_Data[j2++]= (unsigned char)SPI_RX_Buff[i+4];
			EEG_Ch2_Data[j2++]= (unsigned char)SPI_RX_Buff[i+5];

		}
		
		for(t=0; t< 1 ; t++)
		{	
			buff = 0;
					
											
			
			
			ueegtemp = (unsigned long) ((EEG_Ch2_Data[pckt]<<16)|(EEG_Ch2_Data[pckt+1]<<8)|EEG_Ch2_Data[pckt+2]);
			Serial.print(ueegtemp,HEX);        
			//Serial.print("  ");                
			ueegtemp = (unsigned long) (ueegtemp<<8);
			seegtemp = (signed long) (ueegtemp);
			seegtemp = (signed long) (seegtemp>>8);	
            
			Serial.println(seegtemp,HEX);
			//delay(100);
			//  seegtemp = 1000;

			pckt+= 3;
							
			datac[buff++] = (unsigned char) (seegtemp);
			datac[buff++] = (unsigned char) (seegtemp>>8);   //>>8
			datac[buff++] = ( unsigned char) (seegtemp >> 16); //>>16
			datac[buff++] = (unsigned char) (seegtemp >>24);

							
						
			
			//Serial.println(seegtemp);	

			for(i=0; i<39; i++) // transmit the data
			{
			//	Serial.write(datac[i]);
				//udi_cdc_putc(datac[i]);
				//cpu_delay_us(90, 48000000);		
			}					
			//	cpu_delay_us(100, 48000000);	
		}
		
		SPI_RX_Buff_Count = 0;
					
	}						
} 

    


