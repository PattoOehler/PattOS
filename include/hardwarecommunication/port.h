
#ifndef __PATTOS__HARDWARECOMMUNICATION__PORT_H
#define __PATTOS__HARDWARECOMMUNICATION__PORT_H

#include <common/types.h>

namespace pattos
{
	namespace hardwarecommunication
	{

		class Port
		{
		protected:
			pattos::common::uint16_t portnumber;
			Port(pattos::common::uint16_t portnumber);	
			~Port();
		};
		class Port8Bit : public Port
		{
		
		public:
			Port8Bit(pattos::common::uint16_t portnumber);
			~Port8Bit();
			virtual void Write(pattos::common::uint8_t data);
			virtual pattos::common::uint8_t Read();
		
		};
		
		class Port8BitSlow : public Port8Bit
		{
		
		public:
			Port8BitSlow(pattos::common::uint16_t portnumber);
			~Port8BitSlow();
			virtual void Write(pattos::common::uint8_t data);
			
		};
		
			
		class Port16Bit : public Port
		{
		
		public:
			Port16Bit(pattos::common::uint16_t portnumber);
			~Port16Bit();
			virtual void Write(pattos::common::uint16_t data);
			virtual pattos::common::uint16_t Read();
		
		};
		
			
		class Port32Bit : public Port
		{
		
		public:
			Port32Bit(pattos::common::uint16_t portnumber);
			~Port32Bit();
			virtual void Write(pattos::common::uint32_t data);
			virtual pattos::common::uint32_t Read();
		};

	}
}

#endif
