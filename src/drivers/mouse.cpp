#include <drivers/mouse.h>

using namespace pattos::common;
using namespace pattos::drivers;
using namespace pattos::hardwarecommunication;


MouseDriver::MouseDriver(InterruptManager* manager)
: InterruptHandler(0x2C, manager),
dataport(0x60),
commandport(0x64)
{

}
MouseDriver::~MouseDriver()
{

}


void MouseDriver::Activate() 
{
	uint16_t* VideoMemory = (uint16_t*)0xb8000; 
	VideoMemory[12*80+40] = ((VideoMemory[12*80+40] & 0xF000) >> 4) | ((VideoMemory[12*80+40] & 0x0F00) << 4) | (VideoMemory[12*80+40] & 0x00FF);


	offset = 0;
	buttons =0;

	commandport.Write(0xA8); // Activate the Interrupts
	commandport.Write(0x20); // Get Current State
	uint8_t status = dataport.Read() | 2;
	
	commandport.Write(0x60); //Change state
	dataport.Write(status);
	
	commandport.Write(0xD4);
	dataport.Write(0xF4);
	dataport.Read();
}



void printf(char *);



uint32_t MouseDriver::HandleInterrupt(uint32_t esp)
{

	uint8_t status = commandport.Read();
	if(!(status & 0x20))
		return esp;

	
	static int8_t x=40,y=12;


	buffer[offset] = dataport.Read();
	offset = (offset+1)%3;
	
	static int8_t change_x=1, change_y=1;
	bool Change_this_run = false;

	static int React = 0;

	if(offset == 0)
	{


		if(React == 0)
		{
			Display::Flip_Background(x, y);
		


			x+= buffer[1];
			y-= buffer[2];
			if(x<0)
				x=0;
			if(x>=80)
				x=79;
			if(y<0)
				y=0;
			if(y>25)
				y=24;
				
			
			Display::Flip_Background(x, y);
			React++;
		}
		else
		{
			React++;
			React%=3;
		}


		for(uint8_t i=0; i<3; i++) 
		{
			if((buffer[0] & (0x01 << i)) != (buttons & (0x01<<i)))
			{
				Display::modify_and_get_current_xy(0,0,x,y);
			}
		}
		
		
	}



	
	return esp;
}


