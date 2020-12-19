

#include <hardwarecommunication/interrupts.h>

using namespace pattos::common;
using namespace pattos::hardwarecommunication;


void print_num(uint8_t num) {

    uint8_t modify_num = num;
    uint8_t index1 = 0;
    uint8_t index2 = 0;
    uint8_t index3 = 1;
    char* nums = "0123456789";
    char* printing = "000\0";

    while(modify_num > 100)
    {
        index1++;
        modify_num -= 100;
    }
    while(modify_num > 10)
    {
        index2++;
        modify_num -= 10;
    }
    while(modify_num > 1)
    {
        index3++;
        modify_num --;
    }

    printing[0] = nums[ index1 ];
    printing[1] = nums[ index2 ];
    printing[2] = nums[ index3 ];

  	Display::do_nothing(printing);
    Display::do_nothing("\n");


}

InterruptHandler::InterruptHandler(uint8_t interruptNumber, InterruptManager* interruptManager)
{

	this->interruptNumber = interruptNumber;
	this->interruptManager = interruptManager;
	interruptManager->handlers[interruptNumber] = this;

}
InterruptHandler::~InterruptHandler()
{
	if(interruptManager->handlers[interruptNumber] == this) 
	{
		interruptManager->handlers[interruptNumber] = 0;
	
	}
}
		
	
uint32_t InterruptHandler::HandleInterrupt(uint32_t esp)

{
	return esp;

}








InterruptManager::GateDescriptor InterruptManager::interruptDescriptorTable[256];

InterruptManager* InterruptManager::ActiveInterruptManager = 0;



void InterruptManager::SetInterruptDescriptorTableEntry(
	uint8_t interruptNumber,
	uint16_t codeSegmentSelectorOffset,
	void (*handler)(),
	uint8_t DescriptorPrivilegeLevel,
	uint8_t DescriptorType) 
{

	const uint8_t IDT_DESC_PRESENT = 0x80;

	interruptDescriptorTable[interruptNumber].handlerAddressLowBits = ((uint32_t)handler) & 0xFFFF;
	interruptDescriptorTable[interruptNumber].handlerAddressHighBits = ((uint32_t)handler >> 16) & 0xFFFF;
	interruptDescriptorTable[interruptNumber].gdt_codeSegmentSelector = codeSegmentSelectorOffset;
	interruptDescriptorTable[interruptNumber].access = IDT_DESC_PRESENT | DescriptorType | ((DescriptorPrivilegeLevel&3) <<5);
	interruptDescriptorTable[interruptNumber].reserved = 0;
	
	
	
}

	
InterruptManager::InterruptManager(GlobalDescriptorTable *gdt)
: picMasterCommand(0x20),
  picMasterData(0x21),
  picSlaveCommand(0xA0),
  picSlaveData(0xA1)
{
	uint16_t CodeSegment = gdt->CodeSegmentSelector();
	const uint8_t IDT_INTERRUPT_GATE = 0xE;
	for(uint16_t i =0; i<256; i++) 
	{
		handlers[i] = 0;
		SetInterruptDescriptorTableEntry(i, CodeSegment, &IgnoreInterruptRequest, 0, IDT_INTERRUPT_GATE);
	}
	
		
	SetInterruptDescriptorTableEntry(0x20, CodeSegment, &HandleInterruptRequest0x00, 0, IDT_INTERRUPT_GATE);
	SetInterruptDescriptorTableEntry(0x21, CodeSegment, &HandleInterruptRequest0x01, 0, IDT_INTERRUPT_GATE);
	SetInterruptDescriptorTableEntry(0x2C, CodeSegment, &HandleInterruptRequest0x0C, 0, IDT_INTERRUPT_GATE);
 		
 		
 	picMasterCommand.Write(0x11);
 	picSlaveCommand.Write(0x11);
 	
 	picMasterData.Write(0x20);
 	picSlaveData.Write(0x28);
 	
 	picMasterData.Write(0x04);
 	picSlaveData.Write(0x02);
 	
 	picMasterData.Write(0x01);
 	picSlaveData.Write(0x01);
 	
 	picMasterData.Write(0x00);
 	picSlaveData.Write(0x00);
 		
 		
 	InterruptDescriptorTablePointer idt;
 	idt.size = 256*sizeof(GateDescriptor) -1;
 	idt.base = (uint32_t) interruptDescriptorTable;
 	asm volatile("lidt %0" : : "m"(idt));
}


InterruptManager::~InterruptManager()
{

}

void InterruptManager::Activate() 
{

	if(ActiveInterruptManager != 0)
		ActiveInterruptManager->Deactivate();
	ActiveInterruptManager = this;

	asm("sti");
}

void InterruptManager::Deactivate() 
{

	if(ActiveInterruptManager == this)
	{
	
		ActiveInterruptManager = 0;

		asm("cli");
		
	}
}




uint32_t InterruptManager::handleInterrupt(uint8_t interruptNumber, uint32_t esp)
{
	if(ActiveInterruptManager != 0) 
		return ActiveInterruptManager->DoHandleInterrupt(interruptNumber, esp);
	
	
	return esp;
}

uint32_t InterruptManager::DoHandleInterrupt(uint8_t interruptNumber, uint32_t esp)
{


	if(handlers[interruptNumber] != 0)
	{
		esp = handlers[interruptNumber]->HandleInterrupt(esp);
	}
	else if(interruptNumber != 0x20)
	{
		char* printing = "INTERRUPT NOT PROGRAMMED IN 0x00";
		char* hex = "0123456789ABCDEF";
		printing[30] = hex[(interruptNumber >> 4) & 0x0F];
		printing[31] = hex[interruptNumber & 0x0F];
		Display::do_nothing(printing);
	}
	else if(interruptNumber == 0x20) 
	{
		Display::movement_on_interrupt();

		
		Display::Flip_Background_And_Change_Prev_Back();

	}
	


	if(0x20 <= interruptNumber && interruptNumber < 0x30)
	{
		picMasterCommand.Write(0x20);
		if(0x28 <= interruptNumber)
			picSlaveCommand.Write(0x20);
	}
	
	
	return esp;
}

