#ifndef __PATTOS__HARDWARECOMMUNICATION__INTERRUPTS_H
#define __PATTOS__HARDWARECOMMUNICATION__INTERRUPTS_H

#include <common/types.h>
#include <hardwarecommunication/port.h>
#include <gdt.h>
#include <hardwarecommunication/display.h>


namespace pattos
{
	namespace hardwarecommunication
	{

		class InterruptManager;

		class InterruptHandler
		{
		protected:
			pattos::common::uint8_t interruptNumber;
			InterruptManager* interruptManager;
			
			InterruptHandler(pattos::common::uint8_t interruptNumber, InterruptManager* interruptManager);
			~InterruptHandler();
			
		public:
			virtual pattos::common::uint32_t HandleInterrupt(pattos::common::uint32_t esp);
			
		};
			class InterruptManager
		{
		friend class InterruptHandler;
		
		protected:
		
			static InterruptManager* ActiveInterruptManager;
			InterruptHandler* handlers[256];
		
			struct GateDescriptor
			{
				pattos::common::uint16_t handlerAddressLowBits;
				pattos::common::uint16_t gdt_codeSegmentSelector;
				pattos::common::uint8_t reserved;					
				pattos::common::uint8_t access;
				pattos::common::uint16_t handlerAddressHighBits;
				
				
			} __attribute__((packed));
		
			static GateDescriptor interruptDescriptorTable[256];
				
				
			struct InterruptDescriptorTablePointer
			{
				pattos::common::uint16_t size;
				pattos::common::uint32_t base;
					
			} __attribute__((packed));
				
				
				
			static void SetInterruptDescriptorTableEntry(
				pattos::common::uint8_t interruptNumber,
				pattos::common::uint16_t codeSegmentSelectorOffset,
				void (*handler)(),
				pattos::common::uint8_t DescriptorPrivilegeLevel,
				pattos::common::uint8_t DescriptorType
			
			);
			
			Port8BitSlow picMasterCommand;
			Port8BitSlow picMasterData;
			Port8BitSlow picSlaveCommand;
			Port8BitSlow picSlaveData;
			
			public:
		
			InterruptManager(pattos::GlobalDescriptorTable *gdt);
			~InterruptManager();
		
			void Activate();
			void Deactivate();
			
			static pattos::common::uint32_t handleInterrupt(pattos::common::uint8_t interruptNumber, pattos::common::uint32_t est);
			pattos::common::uint32_t DoHandleInterrupt(pattos::common::uint8_t interruptNumber, pattos::common::uint32_t est);
			
			static void IgnoreInterruptRequest();
			static void HandleInterruptRequest0x00();
			static void HandleInterruptRequest0x01();	
			static void HandleInterruptRequest0x0C();	
			
		};
	}
}


#endif
