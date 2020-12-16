
#include <drivers/driver.h>

using namespace pattos::common;
using namespace pattos::drivers;

Driver::Driver()
{


}
Driver::~Driver()
{


}

void Driver::Activate()
{

}
int Driver::Reset()
{

}
void Driver::Deactivate()
{

}
	




void DriverManager::AddDriver(Driver* drv)
{
	drivers[numDrivers] = drv;
	numDrivers++;

}
	
DriverManager::DriverManager()
{
	numDrivers = 0;
	

}

void DriverManager::ActivateAll()
{
	for(int i = 0; i<numDrivers; i++) 
	{
		drivers[i]->Activate();
	}

}
