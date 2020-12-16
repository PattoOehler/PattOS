
#ifndef __PATTOS__DRIVERS__MOUSE_H
#define __PATTOS__DRIVERS__MOUSE_H

#include <common/types.h>
#include <hardwarecommunication/interrupts.h>
#include <drivers/driver.h>
#include <hardwarecommunication/port.h>
#include <hardwarecommunication/display.h>

namespace pattos
{

	namespace drivers
	{

		class MouseDriver : public pattos::hardwarecommunication::InterruptHandler, public Driver
		{
			pattos::hardwarecommunication::Port8Bit dataport;
			pattos::hardwarecommunication::Port8Bit commandport;
			
			pattos::common::uint8_t buffer[3];
			pattos::common::uint8_t offset;
			pattos::common::uint8_t buttons;
			
		public: 
			MouseDriver(pattos::hardwarecommunication::InterruptManager* manager);
			~MouseDriver();
			virtual pattos::common::uint32_t HandleInterrupt(pattos::common::uint32_t esp);
			virtual void Activate();
			
			
		};

	}
}

#endif
