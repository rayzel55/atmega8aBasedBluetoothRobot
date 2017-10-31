#define F_CPU 8000000UL
#define BAUD 9600

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>

#include "robot.h"

//INTERRUPTS

ISR(USART_RXC_vect)
{
	bluetooth_data=UDR;  //receive data from hc-06
	bluetooth_flag=1;
}

void uc_init(void);
void pwm_init(void);
void usart_init(void);
void adc_init(void);

extern volatile uint8_t bluetooth_flag, bluetooth_data;

int main(void)
{
	uc_init();
	
	sei();
	
	while(1)
	{
		if(bluetooth_flag==1)
		{
			bluetooth_flag=0;
			
			if(bluetooth_data==65)
			{
				while(1)
				{
					if(bluetooth_flag==1)
					{
						bluetooth_flag=0;
						if(bluetooth_data==72) break;
						robot_control(bluetooth_data);
					}
				}
			}
			
			else if(bluetooth_data==66)
			{
				TR_PORT|=(1<<TR_PIN);
				adc_init();
				line_follower();
			}
		}
	}
	
	return 0;
}

//MAIN FUNCTIONS

void uc_init(void)
{
	DC_DDR|=(1<<DC_1_A)|(1<<DC_1_B)|(1<<DC_2_A)|(1<<DC_2_B);	//engine outputs
	DC_PORT=0;
	TR_DDR|=(1<<TR_PIN); //transistor output
	
	pwm_init();
	
	usart_init();
}

void pwm_init(void)
{
	DDRB|=(1<<PB1)|(1<<PB3); //PWM pins
	TCCR2|=(1<<WGM20)|(1<<WGM21)|(1<<CS21)|(1<<COM21); //timer 2 PWM fcpu/8 prescaler, clear at top, set at bottom
	OCR2=255; //DC 1
	TCCR1A|=(1<<COM1A1)|(1<<WGM10); //timer 1 PWM
	TCCR1B|=(1<<WGM12)|(1<<CS11);
	OCR1A=255; //DC 2
}

void usart_init(void)
{
	UBRRH=UBRRH_VALUE;    //usart speed configuration
	UBRRL=UBRRL_VALUE;  	   
	
	UCSRC|=(1<<URSEL)|(1<<UCSZ1)|(1<<UCSZ0); //8bit 1stop no parity
	UCSRB|=(1<<TXEN)|(1<<RXEN)|(1<<RXCIE); //enable receiver and transmitter, receive interrupt enabled
}

void adc_init(void)
{
	ADCSRA|=(1<<ADEN)|(1<<ADPS2)|(1<<ADPS1);
}


