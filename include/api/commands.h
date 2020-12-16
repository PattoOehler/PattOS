#ifndef __PATTOS__API__COMMANDS_H
#define __PATTOS__API__COMMANDS_H

#include <common/types.h>
#include <hardwarecommunication/display.h>


namespace pattos
{
	namespace api
	{
		class commands
		{
		public:
			void help();
			void makePATTOSfilesys();
			void checkPATTOSfilesys();
			void Print_First_Sector();
			void ls();
			void test();
			void mkdir(pattos::common::uint8_t line);
			void clear1ksectors();
			void cd(pattos::common::uint8_t line);
			void mkfile(pattos::common::uint8_t line);
			static void pedit(bool keyboard_calling, pattos::common::uint8_t line);
			void rm(pattos::common::uint8_t line);
		};


	}
}

#endif