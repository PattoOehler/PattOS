
#ifndef __PATTOS__DRIVERS__KEYBOARD_H
#define __PATTOS__DRIVERS__KEYBOARD_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>
#include <hardwarecommunication/filesystem.h>
#include <api/commands.h>


namespace pattos
{
	namespace drivers
	{

		class KeyboardDriver : public pattos::hardwarecommunication::InterruptHandler, public Driver
		{
			pattos::hardwarecommunication::Port8Bit dataport;
			pattos::hardwarecommunication::Port8Bit commandport;
			
		public: 
			KeyboardDriver(pattos::hardwarecommunication::InterruptManager* manager);
			~KeyboardDriver();
			virtual pattos::common::uint32_t HandleInterrupt(pattos::common::uint32_t esp);
			virtual void Activate();
			
			void Print_KeyPresses_To_Screen(pattos::common::uint8_t key);
			
		};
	}

}

#endif
