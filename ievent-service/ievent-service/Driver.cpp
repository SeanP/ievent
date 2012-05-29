#include "Driver.h"

using namespace IEvent::Service;

void IEvent::Service::driverToYaml(DriverPtr driver, boost::shared_ptr<YAML::Emitter> em) {
	(*em) << YAML::BeginMap;
	(*em) << YAML::Key << "DriverName" << YAML::Value << driver->driverName;
	(*em) << YAML::Key << "CarNumber" << YAML::Value << driver->carNumber;
	(*em) << YAML::Key << "CarPath" << YAML::Value << driver->carPath;
	(*em) << YAML::Key << "CarClassID" << YAML::Value << driver->carClassID;
	(*em) << YAML::Key << "CarID" << YAML::Value << driver->carId;
	(*em) << YAML::EndMap;
}