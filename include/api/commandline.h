#ifndef __PATTOS__API__COMMANDLINE_H
#define __PATTOS__API__COMMANDLINE_H

#include <common/types.h>
#include <hardwarecommunication/display.h>
#include <api/commands.h>


namespace pattos
{
	namespace api
	{

		class commandline
		{
		private:
			static bool CommandlineRunning(pattos::common::int8_t Running);
			static void Get_Command(pattos::common::uint8_t line);
		public:
			static void StartCommandLine();
			static void GetCommand();
			static bool strcmp(char a[], char b[]);

		};

	}


}



#endif