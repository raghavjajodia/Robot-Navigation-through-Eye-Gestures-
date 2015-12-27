//////////////////////////////////////////////////////
// CS266                                            //
// Lab1 - LED blinking, proximity and simple motion //
// Christian Ahler - 2012                           //
//////////////////////////////////////////////////////


#include <p30f6014A.h>
#include <stdlib.h>//for random numbers
#include "stdio.h"
#include "string.h"
#include "math.h"
#include "e_epuck_ports.h"
#include "e_init_port.h"#include "e_motors.h"#include "utility.h"#include "e_led.h"#include "e_prox.h"#include "e_ad_conv.h"#include "e_uart_char.h"#include "e_randb.h"#include "btcom.h"
#include "e_remote_control.h"
#include "e_agenda.h"

int sel;//the position of the selector switch
char debugMessage[80];//this is some data to store screen-bound debug messages
int seeSomething;//boolean for forward facing prox sensors

int main(void){	myWait(500);//safety wait period to prevent UART clogging	e_init_port();//set up the Epuck port pins	sel = getselector();//read the position of the selector switch	if(sel == 0) while(1) NOP();//setting the switch to 0 will make the robot do nothing
	
	//init uart for randb comm	e_init_uart1();//	e_init_randb(I2C);

	//init randb//	e_randb_set_range(50);//0 is full range, 255 is zero range, calibrated for 128//	e_randb_set_calculation(ON_BOARD);//	e_randb_store_light_conditions();
	myWait(50);
if(sel==2)
{
	while(1)
	{
		myWait(50);

		char in=btcomGetCharacter();
	//	sprintf(debugMessage,"link active\r\n");
	//	btcomSendString(debugMessage);
		if(in=='F'){
			moveForward(200,900);
		}
		else if(in=='R')
			turn(-90,400);
		else if(in=='L')
			turn(90,400);
		else if (in=='B')
			move(-200,900);				
	}}
	//in case it gets here, it shouldn't fall off the edge
	while(1) NOP();
}
