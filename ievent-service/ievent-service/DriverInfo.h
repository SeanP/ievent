#pragma once

#include <map>
#include "Driver.h"

namespace IEvent
{
	namespace Service
	{
		namespace DriverInfo
		{
			extern int driverCarIndex;
			extern float carRedline;
			extern float fuelKilosPerLiter;
			extern std::map<int, DriverPtr> driversByIndex;
			extern std::map<std::string, DriverPtr> driversByCarNumber;

			void addDriver(DriverPtr driver);
		};  
	}
}

