#include <drivers/keyboard.h>

using namespace pattos::common;
using namespace pattos::drivers;
using namespace pattos::hardwarecommunication;


KeyboardDriver::KeyboardDriver(InterruptManager* manager)
: InterruptHandler(0x21, manager),
dataport(0x60),
commandport(0x64)
{
	
}
KeyboardDriver::~KeyboardDriver()
{

}

void printf(char *);

void KeyboardDriver::Activate()
{
	while(commandport.Read() & 0x1) {
	
		dataport.Read();
	}
	commandport.Write(0xAE); // Activate the Interrupts
	commandport.Write(0x20); // Get Current State
	uint8_t status = (dataport.Read() | 1) & ~0x10;
	
	commandport.Write(0x60); //Change state
	dataport.Write(status);
	
	dataport.Write(0xF4);

}


void KeyboardDriver::Print_KeyPresses_To_Screen(uint8_t key)
{

	static bool shift=false;
	
	if(key == 42 | key == 54) 
	{
		shift = true;
		return;

	}
	if(key == 170 | key == 54+128)
	{
		shift = false;
		return;
	}


	const char LIST[] = "??1234567890-=\x0e?qwertyuiop[]\n?asdfghjkl;'`\x2a\\zxcvbnm,./??? ?????????????789-456+1230?????????????????????????????????????????????";
	const char SHIFT_LIST[] = "??!@#$%^&*()_+\x0e?QWERTYUIOP{}\n?ASDFGHJKL:\"~\x2a|ZXCVBNM<>???? ?????????????789-456+1230?????????????????????????????????????????????";


	char temp = "?";

	if(shift)
		temp = SHIFT_LIST[key];
	else
		temp = LIST[key];
	
	if(temp == '?') 
	{
		char *printing = "0X00\0";	
		char *hex = "0123456789ABCDEF";
		printing[2] = hex[(key >> 4) & 0x0F];
		printing[3] = hex[key & 0x0F];
		Display::printstr(printing);
		
		
		
	}
	else if(key==72)
	{
		//Up arrow
		Display::modify_and_get_current_xy(0,-1,0,0);
		return;
	}
	else if(key==75)
	{
		// Left arrow
		Display::modify_and_get_current_xy(-1,0,0,0);
		return;
	}
	else if(key==77)
	{
		//Right arrow
		Display::modify_and_get_current_xy(1,0,0,0);
		return;
	}
	else if(key==80)
	{
		//Down Arrow
		Display::modify_and_get_current_xy(0,1,0,0);
		return;
	}
	else
	{
		Display::printchar(temp);
	}

	pattos::api::commands::pedit(true, 0);
}



uint32_t KeyboardDriver::HandleInterrupt(uint32_t esp)
{
	uint8_t key = dataport.Read();
	
	uint8_t less_than = 0x80;
	
	if(key == 0x45)
		return esp;

	if(key < less_than) {
	
		KeyboardDriver::Print_KeyPresses_To_Screen(key);
	//	char *printing = "KEYBOARD 0X00 \n";	
	//	char *hex = "0123456789ABCDEF";
	//	printing[11] = hex[(key >> 4) & 0x0F];
	//	printing[12] = hex[key & 0x0F];
	//	printf(printing);
		
	}
	else if(key == 170 | key == 54+128) {
		KeyboardDriver::Print_KeyPresses_To_Screen(key);
	}
	
	return esp;
}


