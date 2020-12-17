#include <api/commands.h>
#include <hardwarecommunication/filesystem.h>

using namespace pattos::api;
using namespace pattos::hardwarecommunication;
using namespace pattos::common;

void commands::help() 
{

	Display::clearscreen();

	Display::printstr("help - prints this help screen\n");
	Display::printstr("clear - clears the screen(same as typing to the bottom of the screen)\n");
	Display::printstr("checkPATTOSfilesys - Checks if you are using PATTOS filesystem\n");
	Display::printstr("makePATTOSfilesys - Makes the drive use PATTOS filesys WILL DAMAGE DRIVE\n");
	Display::printstr("PrintFirstSector - clear screen and print the first sector\n");
	Display::printstr("clear1ksectors - delete the first 1000 sectors\n");
	Display::printstr("ls - print all files and directorys\n");
	Display::printstr("mkdir name - make a directory named name\n");
	Display::printstr("mkfile name - make a file named name\n");
	Display::printstr("cd name - enter the directory named name(use cd / to return to root dir)\n");
	Display::printstr("pedit name - edit the file named name\n");
	Display::printstr("rm name - unlinks and clears the sector of the file/dir named name\n");


	Display::printchar('\n');
}

void commands::makePATTOSfilesys()
{

	Filesystem a;

	a.Make_File_System_PATTOS();



}

void commands::checkPATTOSfilesys()
{

	Filesystem a;

	a.Check_File_System();

}

void commands::Print_First_Sector()
{
	Filesystem a;

	a.Print_First_Sector();



}


void commands::ls()
{
	Filesystem a;
	a.ls(Filesystem::Change_and_get_current_dir_sector(0, false));

}

void commands::test()
{
	Filesystem a;
	a.addDirectory("asdf", 4);

}

void commands::mkdir(uint8_t line)
{

	char Filename[25];
	int i = 0;



	while((Display::CharAt(13+i, line) != '\0') & (Display::CharAt(13+i, line) != ' ') & (i < 23))
	{

		Filename[i] = Display::CharAt(13+i, line);

		i++;

	}


	Filesystem filesys;
	filesys.addDirectory(Filename, i);
}

void commands::mkfile(uint8_t line)
{

	char Filename[25];
	uint8_t i = 0;



	while((Display::CharAt(14+i, line) != '\0') & (Display::CharAt(14+i, line) != ' ') & (i < 23))
	{

		Filename[i] = Display::CharAt(14+i, line);

		i++;

	}


	Filesystem filesys;
	filesys.addFile(Filename, i);
}




void commands::cd(uint8_t line)
{
	char Filename[25];
	int i = 0;



	while((Display::CharAt(10+i, line) != '\0') & (Display::CharAt(10+i, line) != ' ') & (i < 23))
	{

		Filename[i] = Display::CharAt(10+i, line);

		i++;

	}


	Filesystem filesys;

	if((i == 1) & Filename[0] == '/')
	{
		Filesystem::Change_and_get_current_dir_sector(0, true);
	}
	else
	{
		filesys.cd(Filename, i);
	}
}

void commands::clear1ksectors()
{
	Filesystem a;
	a.clear1ksectors();


}

void commands::pedit(bool keyboard_calling, uint8_t line)
{

	Filesystem filesys;

	if(!keyboard_calling)
	{
		char Filename[25];
		int i = 0;



		while((Display::CharAt(13+i, line) != '\0') & (Display::CharAt(13+i, line) != ' ') & (i < 23))
		{

			Filename[i] = Display::CharAt(13+i, line);

			i++;

		}

		filesys.pedit(Filename, i);
	

	}

	if(keyboard_calling)
	{
		filesys.SaveFile(false, 0);
	}
	

}

void commands::rm(pattos::common::uint8_t line)
{


	char Filename[25];
	int i = 0;



	while((Display::CharAt(10+i, line) != '\0') & (Display::CharAt(10+i, line) != ' ') & (i < 23))
	{

		Filename[i] = Display::CharAt(10+i, line);

		i++;

	}


	Filesystem filesys;

	if((i == 1) & Filename[0] == '/')
	{
		Filesystem::Change_and_get_current_dir_sector(0, true);
	}
	else
	{
		filesys.rm(Filename, i);
	}


}