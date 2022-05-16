#include "car.h" 
#include "parses.h" 
#include "timer.h" 
#include "usart.h"

int up_level[4];
int down_level[4];
int direction[4];

void car_control(int num,int directions,int percent)//directions:0 反转   directions:1 正转
{
	switch(num)
	{	
		case 0:
		{
			 up_level[0]=percent; 
			 direction[0]=directions;		
			 break;
		 }
		case 1:
		{
			 up_level[1]=percent; 
			 direction[1]=directions;		
			break;
		}
		case 2:
		{
			 up_level[2]=percent; 
			 direction[2]=directions;		
			break;
		}
		case 3:
		{
			 up_level[3]=percent; 
			 direction[3]=directions;		
			break;
		}
		default: break;	
	}		 
}

void delay_car_ms(int num)
{
	// dy=1  12ms  

	// dy=0  1.2us 

	
		while(num>0)
		{
			if(Data_Parses() == 1)
				num=num-10;
			if(Data_Parses() == 0)	
				num=num;	
			num--;
		}
	
}	
