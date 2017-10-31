#include "robot.h"

void robot_control(uint8_t data)
{
	OCR2=255;
	OCR1A=219;
	
	switch (data)
	{
		case 65:      //letter 'A' sent via bluetooth etc.
		ROBOT_FORWARD
		break;
	
		case 66:
		ROBOT_TURN_LEFT
		break;
			
		case 67:
		ROBOT_TURN_RIGHT
		break;
				
		case 68:
		ROBOT_BACKWARD
		break;
				
		case 70:
		ROBOT_STOP
		break;
				
		case 69:
		DC_2_BACKWARD
		break;
				
		case 71:
		DC_1_BACKWARD
		break;
		
		case 73:
		ROBOT_ROTATE_COUNTERCLOCKWISE
		break;
		
		case 74:
		ROBOT_ROTATE_CLOCKWISE
		break;
		
		case 75:
		OCR2=130;
		ROBOT_FORWARD
		break;
		
		case 76:
		OCR1A=130;
		ROBOT_FORWARD
		break;
	}
}

void line_follower(void)
{
	enum STATE {NONE, LEFT, MIDDLE, RIGHT};
	enum STATE state;
	state=NONE;
	
	uint8_t input;
	uint16_t data[3];
	
	while(1)
	{
		if(bluetooth_flag==1)
		{
			bluetooth_flag=0;
			if(bluetooth_data==72)
			{
				ROBOT_STOP
				break;
			}
		}
		
		for(input=3;input<6;++input)
		{
			ADMUX=0;
			ADMUX|=(1<<REFS0)|input;  //changing measurement channel (from ADC3 to ADC5)
			ADCSRA|=(1<<ADSC);
			while(ADCSRA&(1<<ADSC));
			data[input-3]=ADC;
		}
		
		OCR2=255;
		OCR1A=219;
		
		if( (data[L_M]>VALUE && data[M_M]>VALUE && data[R_M]>VALUE && state!=LEFT && state!=RIGHT)||
		    (data[L_M]>VALUE && data[M_M]<VALUE && data[R_M]>VALUE)||
			(data[L_M]<VALUE && data[M_M]<VALUE && data[R_M]>VALUE && state==LEFT)||
			(data[L_M]>VALUE && data[M_M]<VALUE && data[R_M]<VALUE && state==RIGHT)||
		    (data[L_M]<VALUE && data[M_M]<VALUE && data[R_M]<VALUE) ) 
		{
			if(state!=MIDDLE)
			{
				state=MIDDLE;
				ROBOT_STOP
				ROBOT_FORWARD
			}
		}
		
		else if((data[L_M]<VALUE && data[M_M]>VALUE) || (data[L_M]<VALUE && data[M_M]<VALUE && state==MIDDLE))
		{
			if(state!=LEFT)
			{
				state=LEFT;
				ROBOT_STOP
				ROBOT_TURN_LEFT
			}
		}
		
		else if((data[R_M]<VALUE && data[M_M]>VALUE) || (data[R_M]<VALUE && data[M_M]<VALUE && state==MIDDLE))
		{
			if(state!=RIGHT)
			{
				state=RIGHT;
				ROBOT_STOP
				ROBOT_TURN_RIGHT
			}
		}			
		
	}
}