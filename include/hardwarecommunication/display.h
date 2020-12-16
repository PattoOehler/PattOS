
#ifndef __PATTOS__HARDWARECOMMUNICATION__DISPLAY_H
#define __PATTOS__HARDWARECOMMUNICATION__DISPLAY_H

#include <common/types.h>
#include <api/commandline.h>

namespace pattos
{

	namespace hardwarecommunication
	{

		class Display
		{
		public:

			struct x_and_y
			{
				pattos::common::uint8_t x;
				pattos::common::uint8_t y;


			};

			static x_and_y modify_and_get_current_xy(pattos::common::int8_t add_x, pattos::common::int8_t add_y, pattos::common::uint8_t set_x, pattos::common::uint8_t set_y);

			static void Flip_Background(int x, int y);
			static void Flip_Background_And_Change_Prev_Back();
			static void setupDisplay();
			static void movement_on_interrupt();
			static void print_char_at(char character, int x, int y);
			static void printchar(char printit);
			static void printstr(const char printing[]);
			static void printint(pattos::common::uint8_t inta);
			static void clearscreen();
			static void go_to_last_typed_char_in_line(pattos::common::uint16_t line);
			static void ChangeBackground(bool background, pattos::common::uint8_t Color, int x, int y);
			static char CharAt(pattos::common::int8_t x, pattos::common::int8_t y);


		};




	};


};




#endif