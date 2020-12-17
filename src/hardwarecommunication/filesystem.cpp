#include <hardwarecommunication/filesystem.h>

using namespace pattos::hardwarecommunication;
using namespace pattos::api;
using namespace pattos::common;


Filesystem::Filesystem()
 : ata0m(0x1F0, true)
{


}
Filesystem::~Filesystem()
{

}

bool Filesystem::Check_File_System()
{

	char PATTOS_MAGIC[] = "PATTOS\0";
	char checking[] = "aaaaaa\0";

	ata0m.Read28(0, (uint8_t*)checking, 6);

	if(commandline::strcmp(PATTOS_MAGIC, checking))
	{
		Display::printstr("Using Pattos file system!\n");
		return true;
	}
	else
	{
		Display::printstr("Not using PATTOS filesys use command \"makePATTOSfilesys\" to change it\n");
		return false;
	}


}
void Filesystem::Make_File_System_PATTOS()
{

	char PATTOS_MAGIC[] = "PATTOS\0";
	ata0m.Write28(0, PATTOS_MAGIC, 6);
	ata0m.Flush();

}


void Filesystem::Print_First_Sector()
{
	Display::clearscreen();

	char buffer[512];

	ata0m.Read28(0, buffer, 512);

	int Printlocation = 0;


	for(int i=0; i< 512; i++)
	{

		if(Printlocation <= 0)
		{
			if(buffer[i] != '\n')
			{
				Display::printchar(buffer[i]);
			}
			else
			{
				Display::printchar('\\');
				Display::printchar('n');
			}

			if((buffer[i] == '\\') | (buffer[i] == '/'))
			{
				Printlocation = 4;

			}
		}
		else
		{
			Display::printint(buffer[i]);
			Printlocation --;
		}

	}

	Display::printchar('\n');
	

}

void Filesystem::print_sector(uint32_t sector)
{

	Display::clearscreen();

	char buffer[512];

	ata0m.Read28(sector, buffer, 512);

	int Printlocation = 0;




	for(int i=1; i< 512; i++)
	{

		if(buffer[i] != '\n')
			Display::printchar(buffer[i]);
		else
			Display::printchar('\0');
		
	}

}





void Filesystem::ls(uint32_t sector)
{

	if(!Filesystem::Check_File_System())
		return;

	Display::clearscreen();

	char buffer[512];

	ata0m.Read28(sector, buffer, 512);


	int i;
	if(sector == 0)
	{
		i = 6;
	}
	else
	{
		i = 1;
	}


	for(;i<512;i++)
	{
		if(buffer[i] != '\0')
		{
			//File/folder start

			while((buffer[i] != '\\') & (buffer[i] != '/'))
			{
				Display::printchar(buffer[i]);
				i++;	
			}
			Display::printchar(buffer[i]);
			//Accounting for the 4 bytes after the name
			i+=4;
			Display::printchar('\n');
		}


	}

	Display::printchar('\n');

}


uint32_t Filesystem::find_next_open_sector()
{
	if(!Filesystem::Check_File_System())
		return;


	char check[6];



	Display::clearscreen();

	uint32_t i = 0;
	

	ata0m.Read28(i, check, 6);

	while(check[0] != '\0')
	{
		i++;
		ata0m.Read28(i, check, 6);


		if(i > 1000)
		{
			Display::printstr("Error with finding an open sector!!");
			return 1;
		}

	}


	Display::printint(i & 0x000000FF);
	Display::printchar('\n');
	return i;


}


void Filesystem::addDirectory(char name[], uint8_t length)
{

	if(!Filesystem::Check_File_System())
		return;


	uint32_t sector = Filesystem::Change_and_get_current_dir_sector(0, false);

	char buffer[512];
	ata0m.Read28(sector, buffer, 512);


	uint16_t a;
	if(sector == 0)
		a = 6;
	else
		a = 1;

	int count = 0;

	while(a < 512)
	{
		if(buffer[a] == '\0')
			count++;
		else
			count = 0;
		//Length + 5 to account for the ending byte + location
		if(count == length+5)
		{
			a++;
			break;
		}
		a++;
	}

	//On success 
	if(count == length+5)
	{
		int i=0;

		while( i < length )
		{
			buffer[a-count+i] = name[i];
			i++;
		}
		buffer[a-count+i] = '/';

		uint32_t Local= Filesystem::find_next_open_sector();

		buffer[a-count+i+1] = (Local >> 24) & 0x000000FF;
		buffer[a-count+i+2] = (Local >> 16) & 0x000000FF;
		buffer[a-count+i+3] = (Local >> 8)  & 0x000000FF;
		buffer[a-count+i+4] = (Local & 0x000000FF);
		claimSector(Local, true);

		ata0m.Write28(sector, (uint8_t*)buffer, 512);
		ata0m.Flush();


	}
	else
	{
		Display::printstr("Ran out of space or error occured\n");

	}

}


void Filesystem::addFile(char name[], pattos::common::uint8_t length)
{
	if(!Filesystem::Check_File_System())
		return;

	uint32_t sector = Filesystem::Change_and_get_current_dir_sector(0, false);

	char buffer[512];
	ata0m.Read28(sector, buffer, 512);


	uint16_t a;
	if(sector == 0)
		a = 6;
	else
		a = 1;

	int count = 0;

	while(a < 512)
	{
		if(buffer[a] == '\0')
			count++;
		else
			count = 0;
		//Length + 5 to account for the ending byte + location
		if(count == length+5)
		{
			a++;
			break;
		}
		a++;
	}

	if(count == length+5)
	{
		int i=0;

		while( i < length )
		{
			buffer[a-count+i] = name[i];
			i++;
		}
		buffer[a-count+i] = '\\';

		uint32_t Local= Filesystem::find_next_open_sector();

		buffer[a-count+i+1] = (Local >> 24) & 0x000000FF;
		buffer[a-count+i+2] = (Local >> 16) & 0x000000FF;
		buffer[a-count+i+3] = (Local >> 8)  & 0x000000FF;
		buffer[a-count+i+4] = (Local & 0x000000FF);
		claimSector(Local, true);

		ata0m.Write28(sector, (uint8_t*)buffer, 512);
		ata0m.Flush();


	}
	else
	{
		Display::printstr("Ran out of space or error occured\n");

	}


}


void Filesystem::claimSector(uint32_t sector, bool folder)
{

	if(!Filesystem::Check_File_System())
		return;

	char as[1] = "\0";
	as[0] +=1;

	char ab[1] = "\0";
	ab[0] += 2;

	if(folder)
	{
		ata0m.Write28(sector, as, 1);

	}
	else
	{

		ata0m.Write28(sector, ab, 1);
	}
	ata0m.Flush();

}

void Filesystem::clearSector(uint32_t sector)
{
	if(!Filesystem::Check_File_System())
		return;

	char as[1] = "\0";
	ata0m.Write28(sector, as, 1);
	ata0m.Flush();

}



void Filesystem::clear1ksectors()
{

	char zeros[] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";

	for(int i=0; i<100; i++)
	{
		ata0m.Write28(i, zeros, 512);
		ata0m.Flush();
	}

	Display::clearscreen();


}


uint32_t Filesystem::Change_and_get_current_dir_sector(uint32_t new_location, bool change)
{

	static uint32_t current_location = 0;

	if(change)
		current_location = new_location;

	return current_location;



}

void Filesystem::cd(char dirname[], uint8_t len)
{

	uint32_t current = Filesystem::Change_and_get_current_dir_sector(0,0);


	char buffer[512];
	ata0m.Read28(current, buffer, 512);


	int i;
	bool found = false;
	if(current == 0)
	{
		i = 6;
	}
	else
	{
		i = 1;
	}


	int lencheck=0;
	for(;i<512;i++)
	{
		//Possibly Found first char of string
		if(buffer[i] == dirname[0])
		{
			lencheck++;
			i++;
			int x=1;
			while(x<len)
			{
				if(buffer[i] == dirname[x])
				{

					lencheck++;
				}
				else
				{
					lencheck = 0;
					break;
				}
				x++;
				i++;
			}

		}

		//If it found the string
		if(lencheck == len)
		{
			if(buffer[i] == '/')
			{
				found = true;
				break;
			}
			else
			{
				Display::printstr("Found string but not with the /\n");

			}

		}



	}


	uint32_t current_sector;
	if(found)
	{
		//Locations of the location of the dir we are changing to

		current_sector += buffer[i+1] * 0x1000000;
		current_sector += buffer[i+2] * 0x10000;
		current_sector += buffer[i+3] * 0x100; 
		current_sector += buffer[i+4];

		Filesystem::Change_and_get_current_dir_sector(current_sector, true);

	}
	else
	{
		Display::printstr("Error: Could not find the directory\n");
	}


}


void Filesystem::pedit(char name[], pattos::common::uint8_t len)
{

	if(!Filesystem::Check_File_System())
		return;

	uint32_t current = Filesystem::Change_and_get_current_dir_sector(0,0);


	char buffer[512];
	ata0m.Read28(current, buffer, 512);


	int i;
	bool found = false;
	if(current == 0)
	{
		i = 6;
	}
	else
	{
		i = 1;
	}


	int lencheck=0;
	for(;i<512;i++)
	{
		//Possibly Found first char of string
		if(buffer[i] == name[0])
		{
			lencheck++;
			i++;
			int x=1;
			while(x<len)
			{
				if(buffer[i] == name[x])
				{

					lencheck++;
				}
				else
				{
					lencheck = 0;
					break;
				}
				x++;
				i++;
			}

		}
		else
		{
			lencheck = 0;
		}

		//If it found the string
		if(lencheck == len)
		{
			if(buffer[i] == '\\')
			{
				found = true;
				break;
			}
			else
			{
				Display::printstr("Found string but not with the \\\n");

			}

		}



	}


	uint32_t sector_of_file;
	if(found)
	{
		//Locations of the location of the dir we are changing to

		sector_of_file += buffer[i+1] * 0x1000000;
		sector_of_file += buffer[i+2] * 0x10000;
		sector_of_file += buffer[i+3] * 0x100; 
		sector_of_file += buffer[i+4];

		//Filesystem::Change_and_get_current_dir_sector(sector_of_file, true);
		Filesystem::print_sector(sector_of_file);
		Display::printstr("|EOF  ^^ WRITE STUFF UP HERE it will save!\n");
		Display::printstr("(To exit/save edit the | before EOF)\n");

		SaveFile(true, sector_of_file);
	}
	else
	{
		Display::printstr("Error: Could not find the file\n");
	}



}



void Filesystem::SaveFile(bool isfile, uint32_t sector)
{
	static bool run=false;
	static uint32_t filesec;

	if(sector != 0)
		filesec = sector;

	if(isfile)
		run = true;

	if((Display::CharAt(3,10) != '|') & run)
	{

		char buffer[512];
		ata0m.Read28(filesec, buffer, 512);




		Display::x_and_y Current = Display::modify_and_get_current_xy(0,0,1,1);


		for(int i=1;i<512;i++)
		{
			buffer[i] = Display::CharAt(Current.x,Current.y);
			Current = Display::modify_and_get_current_xy(1,0,0,0);

		}
		ata0m.Write28(filesec, (uint8_t*)buffer, 512);
		ata0m.Flush();
		Display::clearscreen();
		Display::printstr("CMD:");
		run = false;
	}


}


void Filesystem::rm(char dirname[], uint8_t len)
{

	if(!Filesystem::Check_File_System())
		return;


	uint32_t current = Filesystem::Change_and_get_current_dir_sector(0,0);


	char buffer[512];
	ata0m.Read28(current, buffer, 512);


	int i;
	bool found = false;
	if(current == 0)
	{
		i = 6;
	}
	else
	{
		i = 1;
	}


	int lencheck=0;
	for(;i<512;i++)
	{
		//Possibly Found first char of string
		if(buffer[i] == dirname[0])
		{
			lencheck++;
			i++;
			int x=1;
			while(x<len)
			{
				if(buffer[i] == dirname[x])
				{

					lencheck++;
				}
				else
				{
					lencheck = 0;
					break;
				}
				x++;
				i++;
			}

		}

		//If it found the string
		if(lencheck == len)
		{
			
			found = true;
			break;
		}



	}

	if(found)
	{

		uint32_t sector_of_file;

		sector_of_file += buffer[i+1] * 0x1000000;
		sector_of_file += buffer[i+2] * 0x10000;
		sector_of_file += buffer[i+3] * 0x100; 
		sector_of_file += buffer[i+4];
		clearSector(sector_of_file);


		buffer[i] = '\0';
		buffer[i + 1] = '\0';
		buffer[i + 2] = '\0';
		buffer[i + 3] = '\0';
		buffer[i + 4] = '\0';

		for(;len>0;len--)
		{
			buffer[i-len] = '\0';

		}
		ata0m.Write28(current, (uint8_t*)buffer, 512);
		ata0m.Flush();


	}
	else
	{
		Display::printstr("File/Directory not found\n");
	}






}