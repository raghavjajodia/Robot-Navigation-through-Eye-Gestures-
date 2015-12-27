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
#include "e_init_port.h"
#include "e_remote_control.h"
#include "e_agenda.h"

int sel;//the position of the selector switch
char debugMessage[80];//this is some data to store screen-bound debug messages
int seeSomething;//boolean for forward facing prox sensors

int main(void)
	
	//init uart for randb comm

	//init randb
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
	}
	//in case it gets here, it shouldn't fall off the edge
	while(1) NOP();
}