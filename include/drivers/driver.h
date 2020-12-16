
#ifndef __PATTOS__DRIVERS__DRIVER_H
#define __PATTOS__DRIVERS__DRIVER_H

#include <common/types.h>

namespace pattos
{

	namespace drivers
	{


		class Driver
		{
		public:
			Driver();
			~Driver();
			
			virtual void Activate();
			virtual int Reset();
			virtual void Deactivate();
		
		};

		class DriverManager
		{
		private:
			Driver* drivers[255];
			int numDrivers;
			
		public:
			DriverManager();
			void AddDriver(Driver*);
			void ActivateAll();
			
		};

}

}

#endif
