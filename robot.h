#ifndef ROBOT_H
#define ROBOT_H

#include <avr/io.h>
#include <stdio.h>

#define VALUE 250

#define DC_DDR DDRB
#define DC_PORT PORTB
#define DC_1_A PB0
#define DC_1_B PB2
#define DC_2_A PB4
#define DC_2_B PB5
#define DC_1_FORWARD  {DC_PORT|=(1<<DC_1_A); DC_PORT&=~(1<<DC_1_B);}
#define DC_1_BACKWARD {DC_PORT|=(1<<DC_1_B); DC_PORT&=~(1<<DC_1_A);}
#define DC_2_FORWARD  {DC_PORT|=(1<<DC_2_A); DC_PORT&=~(1<<DC_2_B);}
#define DC_2_BACKWARD {DC_PORT|=(1<<DC_2_B); DC_PORT&=~(1<<DC_2_A);}
#define DC_1_STOP {DC_PORT&=~((1<<DC_1_A)|(1<<DC_1_B));}
#define DC_2_STOP {DC_PORT&=~((1<<DC_2_A)|(1<<DC_2_B));}
#define ROBOT_FORWARD {DC_1_FORWARD DC_2_FORWARD}
#define ROBOT_BACKWARD {DC_1_BACKWARD DC_2_BACKWARD}
#define ROBOT_TURN_LEFT {DC_2_FORWARD}
#define ROBOT_TURN_RIGHT {DC_1_FORWARD}
#define ROBOT_ROTATE_COUNTERCLOCKWISE {DC_1_BACKWARD DC_2_FORWARD}
#define ROBOT_ROTATE_CLOCKWISE {DC_1_FORWARD DC_2_BACKWARD}
#define ROBOT_STOP {DC_1_STOP DC_2_STOP}

#define TR_DDR DDRD
#define TR_PORT PORTD
#define TR_PIN PD2

#define L_M 0 //left measurement etc
#define M_M 1
#define R_M 2

void robot_control(uint8_t); //controlling robot via bluetooth
void line_follower(void); //follows line using CNY70 proximity sensor

volatile uint8_t bluetooth_data, bluetooth_flag;

#endif