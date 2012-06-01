#pragma once

#include <string>
#include "boost/shared_ptr.hpp"
#include "yaml-cpp/yaml.h"

#include "Types.hpp"

using namespace std;

namespace IEvent
{
	namespace Service
	{
		struct Driver
		{
			int carIndex;
			string driverName;
			int userId;
			string carNumber;
			int encodedCarNumber;
			string carPath;
			int carClassID;
			int carId;
		}; 

		typedef boost::shared_ptr<Driver> DriverPtr;

		void driverToYaml(DriverPtr driver,  EmitterPtr em);
	}

}
