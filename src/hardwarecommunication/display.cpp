#include <hardwarecommunication/display.h>

using namespace pattos::hardwarecommunication;
using namespace pattos::common;



void Display::print_char_at(char character, int x, int y)
{

	static uint16_t* VideoMemory = (uint16_t*)0xb8000;
	if(character > 127)
	{
		VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | '\0';
	}
	VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | character;

}

void Display::Flip_Background(int x, int y) 
{

	uint16_t* VideoMemory = (uint16_t*)0xb8000;
	VideoMemory[y*80+x] = ((VideoMemory[y*80+x] & 0xF000) >> 4) | ((VideoMemory[y*80+x] & 0x0F00) << 4) | (VideoMemory[y*80+x] & 0x00FF);

}




void Display::Flip_Background_And_Change_Prev_Back()
{


	static bool Flip_Back = false;
	static Display::x_and_y Current;

	uint16_t* VideoMemory = (uint16_t*)0xb8000;


	if(!Flip_Back)
	{
		Current = Display::modify_and_get_current_xy(0,0,0,0);
		Flip_Back = true;
	}
	else
	{
		Flip_Back = false;
	}


	VideoMemory[Current.y*80+Current.x] = ((VideoMemory[Current.y*80+Current.x] & 0xF000) >> 4) | ((VideoMemory[Current.y*80+Current.x] & 0x0F00) << 4) | (VideoMemory[Current.y*80+Current.x] & 0x00FF);


}



void Display::ChangeBackground(bool background, uint8_t Color, int x, int y)
{
	/*
	0 000 Black
	1 001 Blue
	2 010 Green
	3 011 Cyan
	4 100 Red
	5 101 Pink
	6 110 Orange
	7 111 Tan
	*/
	if(Color > 7)
		return;

	static uint16_t* VideoMemory = (uint16_t*)0xb8000;

	if(background)
		VideoMemory[y*80+x] = (VideoMemory[y*80+x] & 0x00FF) | Color * 0x1000;
	else
		VideoMemory[y*80+x] = (VideoMemory[y*80+x] & 0x00FF) | Color * 0x0100;

}



void Display::setupDisplay()
{
	int x[] = {0,2,77,79};
	int y[] = {0,2,22,24};	

	for(int xc=0; xc<4;xc++) 
	{
		for(int yc=0; yc<4;yc++)
		{
			Display::print_char_at('/', x[xc], y[yc]);
			Display::ChangeBackground(false, 2, x[xc], y[yc]);
		}

	}
	Display::print_char_at('\\', 1, 1);
	Display::print_char_at('\\', 1, 23);
	Display::print_char_at('\\', 78, 1);
	Display::print_char_at('\\', 78, 23);
	Display::ChangeBackground(false, 3, 1, 1);
	Display::ChangeBackground(false, 3, 1, 23);
	Display::ChangeBackground(false, 3, 78, 1);
	Display::ChangeBackground(false, 3, 78, 23);


	for(int xx=0; xx<(79-6); xx++) 
	{
		Display::print_char_at('_', xx+3, 2);
		Display::print_char_at('_', xx+3, 21);
	}

	for(int yy=0; yy<(24-5); yy++) 
	{
		Display::print_char_at('|', 2, yy+3);
		Display::print_char_at('|', 79-3, yy+3);

	}


	Display::print_char_at('P', 36, 1);
	Display::print_char_at('a', 37, 1);
	Display::print_char_at('t', 38, 1);
	Display::print_char_at('t', 39, 1);
	Display::print_char_at('O', 40, 1);
	Display::print_char_at('S', 41, 1);
	Display::ChangeBackground(false, 4, 40, 1);
	Display::ChangeBackground(false, 4, 41, 1);

}



void Display::movement_on_interrupt()
{
	static int position{ 0 };

	if(position < 4)
	{
		position++;
	}
	else
	{
		position = 0;
	}

	char chars[] = "/|\\-";
	char charsb[] = "\\|/-";

	int x[] = {0,2,77,79};
	int y[] = {0,2,22,24};	

	for(int xc=0; xc<4;xc++) 
	{
		for(int yc=0; yc<4;yc++)
		{
			Display::print_char_at(chars[position], x[xc], y[yc]);

		}

	}

	Display::print_char_at(charsb[position], 1, 1);
	Display::print_char_at(charsb[position], 1, 23);
	Display::print_char_at(charsb[position], 78, 1);
	Display::print_char_at(charsb[position], 78, 23);



}


void Display::go_to_last_typed_char_in_line(uint16_t line)
{


	if(line > 20)
		return;

	static uint16_t* VideoMemory = (uint16_t*)0xb8000;

	uint16_t x = 75;
	uint16_t current_char=0x0000;

	uint16_t allzeros = 0;

	while((x > 3) & (current_char == allzeros))
	{
		current_char = (VideoMemory[80*line+x] & 0x00FF) | 0x0000 ;


		x--;
	}
	if(x<73)
		x+=2;
	else if(x == 74)
	{
		x++;
	}
	else
	{
		//Error can't go back a line
		
	}
	
	Display::modify_and_get_current_xy(0, 0, x, line);

}





void Display::printchar(char printit)
{


	Display::x_and_y Current = Display::modify_and_get_current_xy(0,0,0,0);

	
	if(printit == 14)
	{
		//Backspace Character

		if(Current.x != 3)
		{
			Display::print_char_at('\0', Current.x, Current.y);
			Display::modify_and_get_current_xy(-1,0,0,0);
			return;
		}
		else
		{
			if(Current.y != 3)
			{
				//Go back a line
				Display::print_char_at('\0', Current.x, Current.y);
				Display::go_to_last_typed_char_in_line(Current.y-1);
				return;
			}


		}


		return;
	}

	if(printit == '\n')
	{
		Display::modify_and_get_current_xy(0,1,3,0);
		pattos::api::commandline::GetCommand();
		return;

	}

	Display::print_char_at(printit, Current.x, Current.y);
	Display::modify_and_get_current_xy(1,0,0,0);


}


void Display::printstr(const char printing[])
{

	int i=0;
	while(printing[i] != '\0')
	{
		printchar(printing[i]);
		i++;
	}

}
void Display::printint(uint8_t inta)
{

	uint8_t ones=0, tens=0, hundreds=0;
	char numbers[] = "0123456789";
	char final_Number[] = "000\0";

	while(inta > 100)
	{
		hundreds++;
		inta -= 100;
	}
	while(inta > 10)
	{
		tens++;
		inta -= 10;
	}
	while(inta >= 1)
	{
		ones++;
		inta-=1;
	}

	final_Number[0] = numbers[hundreds];
	final_Number[1] = numbers[tens];
	final_Number[2] = numbers[ones];

	Display::printstr(final_Number);
}
		

void Display::clearscreen()
{


	for(int i=3; i<76; i++)
	{
		for(int j=3; j<21;j++)
		{
			Display::print_char_at('\0', i, j);

		}
	}

	Display::modify_and_get_current_xy(0,0,3,3);

}



Display::x_and_y Display::modify_and_get_current_xy(int8_t add_x, int8_t add_y, uint8_t set_x, uint8_t set_y){


	//They only need to store values within uint8_t however uint16_t fixes overflows without extra code
	static uint16_t x = 3;
	static uint16_t y = 3;

	if(set_x != 0) 
		x = set_x;
	if(set_y != 0) 
		y=set_y;



	Display::x_and_y Returning;

	x += add_x;
	y += add_y;

	if(x<3)
		x=3;
	if(y<3)
		y=3;



	if(x > 75) 
	{
		x = 3;
		y++;

	}

	if(y > 20)
	{
		Display::clearscreen();
		y=3;
		x=3;
	}




	Returning.x = x;
	Returning.y = y;

	return Returning;

}


char Display::CharAt(int8_t x, int8_t y)
{


	static uint16_t* VideoMemory = (uint16_t*)0xb8000;

	return VideoMemory[80*y+x] & 0x00FF;


}