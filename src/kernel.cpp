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
    GlobalDescriptorTable gdt;

    InterruptManager interrupts(&gdt);
    
    
    DriverManager drvManager;
    
    
    KeyboardDriver keyboard(&interrupts);
    drvManager.AddDriver(&keyboard);
    
    
    MouseDriver mouse(&interrupts);
    drvManager.AddDriver(&mouse);
    
    drvManager.ActivateAll();

    
    interrupts.Activate();

    
    Display::setupDisplay();


    Filesystem filesys;

    filesys.Check_File_System();


    Display::printstr("Type \"help\" after CMD: to get a list of commands\n");



    api::commandline::StartCommandLine();


    while(1);
}
