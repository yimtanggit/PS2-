#ifndef __CAR_H
#define __CAR_H


extern int up_level[4];
extern int down_level[4];
extern int direction[4];


void car_control(int num,int directions,int percent);
void delay_car_ms(int num);
#endif
