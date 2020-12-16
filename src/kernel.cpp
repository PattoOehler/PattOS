#include <common/types.h>
#include <gdt.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/keyboard.h>
#include <drivers/mouse.h>
#include <drivers/driver.h>
#include <hardwarecommunication/display.h>
#include <drivers/ata.h>
#include <api/commandline.h>
#include <hardwarecommunication/filesystem.h>

using namespace pattos;
using namespace pattos::common;
using namespace pattos::drivers;
using namespace pattos::hardwarecommunication;

void printf(char* str)
{
    static uint16_t* VideoMemory = (uint16_t*)0xb8000;

    static uint16_t x=0, y=0;
    
    static uint8_t Shift=0;
    
    /*if(str[0] == 42) 
    {
    	Shift =1;
    	return;
    }
    else if(str[0] == '\xAA')
    {
    	
    	Shift = 0;
    	return;
    }
    if(Shift == 1) {
    	str[0] -= 32;
    }
    
    if((str[0] == 14) & x!=0) 
    {
    	if(x != 0)
    	{
    		VideoMemory[80*y+x-1] = (VideoMemory[80*y+x-1] & 0xFF00);
    		x--;
    		return;
    	}
    
    }
    */
    

    /*for(int i = 0; str[i] != '\0'; ++i)
     {
        switch(str[i])
        {
            case '\n':
                x = 0;
                y++;
                break;
            default:
                VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | str[i];
                x++;
                break;
        }

        if(x >= 80)
        {
            x = 0;
            y++;
        }

        if(y >= 25)
        {
            for(y = 0; y < 25; y++)
                for(x = 0; x < 80; x++)
                    VideoMemory[80*y+x] = (VideoMemory[80*y+x] & 0xFF00) | ' ';
            x = 0;
            y = 0;
        }
    }*/
}


typedef void (*constructor)();
extern "C" constructor start_ctors;
extern "C" constructor end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i = &start_ctors; i != &end_ctors; i++)
        (*i)();
}



extern "C" void kernelMain(const void* multiboot_structure, uint32_t /*multiboot_magic*/)
{

    Display::clearscreen();
    //Display::printstr("Hello baby\n");
    GlobalDescriptorTable gdt;

    InterruptManager interrupts(&gdt);
    
    //printf("Initializing Hardware\n");
    
    DriverManager drvManager;
    
    
    KeyboardDriver keyboard(&interrupts);
    drvManager.AddDriver(&keyboard);
    
    
    MouseDriver mouse(&interrupts);
    drvManager.AddDriver(&mouse);
    
    drvManager.ActivateAll();
    
    //printf("All Drivers Initilized!\n");
    
    interrupts.Activate();

    
    Display::setupDisplay();
    

    // Interrupt 14
    //AdvancedTechnologyAttachment ata0m(0x1F0, true);
    //Display::printstr("ATA Primary Master: ");
    //ata0m.Identify();
    //Display::printchar('\n');

    //char buffer[] = "I aaaaa aaaa aa read and write this to the hard drive plz...";
    //ata0m.Write28(0, (uint8_t*)buffer, 62);
    //ata0m.Flush();
   // ata0m.Read28(0, (uint8_t*)buffer, 62);
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


    Filesystem filesys;

    filesys.Check_File_System();


    Display::printstr("Type \"help\" after CMD: to get a list of commands\n");



    api::commandline::StartCommandLine();
    //Display::printchar(Display::CharAt(4,3));


    while(1);
}
