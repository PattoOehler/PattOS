#include <api/commandline.h>

using namespace pattos::common;
using namespace pattos::api;
using namespace pattos::hardwarecommunication;

void commandline::StartCommandLine()
{

	commandline::CommandlineRunning(1);

	Display::printstr("CMD:");

}

void commandline::GetCommand()
{
	if(!commandline::CommandlineRunning)
		return;

	Display::x_and_y currentlocation;

	currentlocation = Display::modify_and_get_current_xy(0,0,0,0);

	char cmd[] = "CMD:\0";
	char check[5];
	for(int i=3; i<7; i++)
		check[i-3] = Display::CharAt(i,currentlocation.y-1);
	check[4] = '\0';

	if(commandline::strcmp(cmd, check))
	{

		commandline::Get_Command(currentlocation.y -1);

		commandline::StartCommandLine();
		//Display::printstr("STRCMP is working!");

	}



	//Display::CharAt(x,y);


}


bool commandline::CommandlineRunning(int8_t Running)
{
	//0 is return value, 1 is change to true 2 is change to false

	static bool CommandLineRunnin = false;

	if(Running == 0)
	{
		return CommandLineRunnin;
	}

	if(Running == 1)
	{
		CommandLineRunnin = true;
	}
	else if(Running == 2)
	{
		CommandLineRunnin = false;
	}

	return CommandLineRunnin;
}


bool commandline::strcmp(char a[], char b[])
{
	uint8_t i=0;
	while(a[i] != '\0')
	{
		if(a[i] != b[i])
			return false;
		i++;
	}

	if(a[i] != b[i])
		return false;

	return true;
}


void commandline::Get_Command(uint8_t line)
{
	//Max characters for the command is 25

	char command[25];
	uint8_t command_counter=0;

	char check='a';

	while((check != '\0') & (check != ' '))
	{

		check = Display::CharAt(command_counter+7,line);

		if(check == ' ')
			break;
		else if(check == '\0')
			break;


		command[command_counter] = check;
		command_counter++;



		if(command_counter > 23)
		{
			
			Display::printstr("Too long of a command!\n");
			return;
		}

	}

	command[command_counter] = '\0';



	commands Commands_Class;


	if(commandline::strcmp(command, "clear\0"))
	{
		Display::clearscreen();
	}
	else if(commandline::strcmp(command, "help\0"))
	{
		Commands_Class.help();
	}
	else if(commandline::strcmp(command, "checkPATTOSfilesys\0"))
	{
		Commands_Class.checkPATTOSfilesys();
	}
	else if(commandline::strcmp(command, "makePATTOSfilesys\0"))
	{
		Commands_Class.makePATTOSfilesys();
	}
	else if(commandline::strcmp(command, "PrintFirstSector\0"))
	{
		Commands_Class.Print_First_Sector();
	}
	else if(commandline::strcmp(command, "ls\0"))
	{
		Commands_Class.ls();
	}
	else if(commandline::strcmp(command, "clear1ksectors\0"))
	{
		Commands_Class.clear1ksectors();
	}
	else if(commandline::strcmp(command, "test\0"))
	{
		Commands_Class.test();
	}
	else if(commandline::strcmp(command, "mkdir\0"))
	{
		Commands_Class.mkdir(line);
	}
	else if(commandline::strcmp(command, "mkfile\0"))
	{
		Commands_Class.mkfile(line);
	}
	else if(commandline::strcmp(command, "cd\0"))
	{
		Commands_Class.cd(line);
	}
	else if(commandline::strcmp(command, "pedit\0"))
	{
		commands::pedit(false, line);
	}
	else if(commandline::strcmp(command, "rm\0"))
	{
		Commands_Class.rm(line);
	}
	else
	{
		Display::printstr("Unknown command!\n");
	}
}