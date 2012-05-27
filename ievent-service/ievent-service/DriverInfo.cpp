#include "DriverInfo.h"

using namespace IEvent::Service;

int DriverInfo::driverCarIndex= -1;
float DriverInfo::carRedline = -1;
float DriverInfo::fuelKilosPerLiter = -1;
std::map<int, DriverPtr> DriverInfo::driversByIndex;
std::map<std::string, DriverPtr> DriverInfo::driversByCarNumber;

void DriverInfo::addDriver(DriverPtr driver) {
	driversByIndex.insert(std::pair<int, DriverPtr>(driver->carIndex, driver));
	driversByCarNumber.insert(std::pair<std::string, DriverPtr>(driver->carNumber, driver));
}