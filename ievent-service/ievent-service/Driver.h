#pragma once

#include <string>
#include "boost/shared_ptr.hpp"

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
			string carPath;
			int carClassID;
			int carId;
		}; 

		typedef boost::shared_ptr<Driver> DriverPtr;
	}

}
