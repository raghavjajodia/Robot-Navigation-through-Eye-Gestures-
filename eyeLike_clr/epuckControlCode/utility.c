#include "utility.h"
#include "e_epuck_ports.h"
#include "e_uart_char.h"
#include "e_motors.h"
#include "math.h"
#include <stdlib.h>//for random numbers

void wait(long num) {
	long i;
	for(i=0;i<num;i++);
}

void myWait(long milli)
{
	long i;
	for(i=0;i<(milli*1500);i++);
}

int getselector() {
	return SELECTOR0 + 2*SELECTOR1 + 4*SELECTOR2 + 8*SELECTOR3;
}

void setLED(int LEDnum, int state)
{
	if(LEDnum == 0)
		LED0 = state;
	if(LEDnum == 1)
		LED1 = state;
	if(LEDnum == 2)
		LED2 = state;
	if(LEDnum == 3)
		LED3 = state;
	if(LEDnum == 4)
		LED4 = state;
	if(LEDnum == 5)
		LED5 = state;
	if(LEDnum == 6)
		LED6 = state;
	if(LEDnum == 7)
		LED7 = state;
}

int errorPercent = 1;//problem with 0
void setErrorPercent(int input)
{
	errorPercent = input;
}

void setSpeeds(int leftSpeed, int rightSpeed)
{
	double chosenErrorLeft = ((double) ((rand() % (2*errorPercent))-errorPercent)) / 100.0;
	int actualLeftSpeed = ((int) (((double) leftSpeed) * (1.0 + chosenErrorLeft)));
	double chosenErrorRight = ((double) ((rand() % (2*errorPercent))-errorPercent)) / 100.0;
	int actualRightSpeed = ((int) (((double) rightSpeed) * (1.0 + chosenErrorRight)));
	e_set_speed_left(actualLeftSpeed);
	e_set_speed_right(actualRightSpeed);

	//e_set_speed_left(leftSpeed);
	//e_set_speed_right(rightSpeed);
}

//this will move the robot forward dist millimeters, then stop
//speed is between 0 and 1000, with 1000 being maximum speed
void moveForward(int dist, int speed)
{
	//1000 counts per revolution and diameter 41 mm
	//1000/(41*pi) = 7.7637
	int maxWheelCount = dist*7.7637;
	e_set_steps_left(0);
	e_set_steps_right(0);
	setSpeeds(speed, speed);

	do
	{
		NOP();
	}while(e_get_steps_left() < maxWheelCount);

	setSpeeds(0, 0);
}

void move(int dist, int speed)
{
	int maxWheelCount = ((int) ((double) dist)*7.7637);
	e_set_steps_left(0);
	e_set_steps_right(0);
	
	if(dist>0)
	{
		setSpeeds(speed, speed);
		do{NOP();}while(e_get_steps_left() < maxWheelCount);
	}else{
		setSpeeds(-speed, -speed);
		do{NOP();}while(e_get_steps_left() > maxWheelCount);
	}

	setSpeeds(0, 0);
}

void turn(double degrees, int speed)
{
	//this 3.45 was tweaked empirically
	int maxWheelCount = ((int) (degrees*3.45));
	e_set_steps_left(0);
	e_set_steps_right(0);
	if(degrees>0)
	{
		setSpeeds(-speed, speed);	
		do
		{
			NOP();
		}while(e_get_steps_right() < maxWheelCount);
	}else if(degrees<0){
		setSpeeds(speed, -speed);	
		do
		{
			NOP();
		}while(e_get_steps_right() > maxWheelCount);
	}

	setSpeeds(0, 0);
}

void allRedLEDsOff()
{
	LED0 = 0;
	LED1 = 0;
	LED2 = 0;
	LED3 = 0;
	LED4 = 0;
	LED5 = 0;
	LED6 = 0;
	LED7 = 0;
}

void allRedLEDsOn()
{
	LED0 = 1;
	LED1 = 1;
	LED2 = 1;
	LED3 = 1;
	LED4 = 1;
	LED5 = 1;
	LED6 = 1;
	LED7 = 1;
}

//this blocks until a character comes over the bluetooth,
//then returns that character
char btcomGetCharacter()
{
    char result;
   // do
    //{
		e_getchar_uart1(&result);
   // }
   // while(msg != trigger);
    
    // sleep a bit
    long int count;
    for(count = 0; count < 1000000; count++) 
			asm("nop");

	return result;
}