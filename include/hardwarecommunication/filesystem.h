#ifndef __PATTOS__HARDWARECOMMUNICATION__FILESYSTEM_H
#define __PATTOS__HARDWARECOMMUNICATION__FILESYSTEM_H


#include <common/types.h>
#include <hardwarecommunication/display.h>
#include <api/commandline.h>
#include <drivers/ata.h>


namespace pattos
{
	namespace hardwarecommunication
	{

		class Filesystem
		{
		protected:

			//COMMENTS ARE FOR AN EXAMPLE TO USE AND EXPANSION 
				// Interrupt 14
			pattos::drivers::AdvancedTechnologyAttachment ata0m;
			    //pattos::drivers::AdvancedTechnologyAttachment ata0m(0x1F0, true);
			    //Display::printstr("ATA Primary Master: ");
			    //ata0m.Identify();
			    //Display::printchar('\n');

			    //char buffer[] = "I aaaaa aaaa aa read and write this to the hard drive plz...";
			    //ata0m.Write28(0, (uint8_t*)buffer, 62);
			    //ata0m.Flush();
			    //ata0m.Read28(0, (uint8_t*)buffer, 62);
			    //Display::printchar('\n');
			    //Display::printstr(buffer);
			    
			    //AdvancedTechnologyAttachment ata0s(0x1F0, false);
			    //Display::printstr("ATA Primary Slave: ");
			    //ata0s.Identify();
			    

			    // Interrupt 15
			    //AdvancedTechnologyAttachment ata1m(0x170, true);
			    //AdvancedTechnologyAttachment ata1s(0x170, false);
			    // Next inline                     0x1E8  Interrupt ?
			    // Next inline                     0x168  Interrupt ?	


		public:
			Filesystem();
			~Filesystem();
			bool Check_File_System();
			void Make_File_System_PATTOS();

			void Print_First_Sector();

			void ls(pattos::common::uint32_t sector);

			pattos::common::uint32_t find_next_open_sector();
			void addDirectory(char name[], pattos::common::uint8_t length);
			void addFile(char name[], pattos::common::uint8_t length);


			void claimSector(pattos::common::uint32_t sector, bool folder);

			void clear1ksectors();

			void cd(char dirname[], pattos::common::uint8_t len);

			static pattos::common::uint32_t Change_and_get_current_dir_sector(pattos::common::uint32_t new_location, bool change);

			void pedit(char name[], pattos::common::uint8_t length);

			void print_sector(pattos::common::uint32_t sector);

			void SaveFile(bool isfile, pattos::common::uint32_t sector);

			void rm(char dirname[], pattos::common::uint8_t len);

			void clearSector(pattos::common::uint32_t sector);

		};


	}


}



#endif