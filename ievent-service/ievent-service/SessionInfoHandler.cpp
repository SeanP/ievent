#include "SessionInfoHandler.h"

#include <sstream>
#include "yaml-cpp/yaml.h"
#include "boost/foreach.hpp"

#include "DriverInfo.h"
#include "Driver.h"

using namespace IEvent::Service;

SessionInfoHandler::SessionInfoHandler(PublisherPtr publisher) :
	_lastSessionInfoStringVersion(-1),
	_publisher(publisher)
{
}


bool SessionInfoHandler::handleUpdate(const irsdk_header *pHeader, char *ir_data) {
	if (_lastSessionInfoStringVersion < pHeader->sessionInfoUpdate) {
		_lastSessionInfoStringVersion = pHeader->sessionInfoUpdate;

		YAML::Emitter em;

		em << YAML::BeginMap << YAML::Key << "SessionInfoUpdate" << YAML::Value << YAML::BeginMap;

		std::stringstream ss;
		ss << irsdk_getSessionInfoStr();
		YAML::Parser parser(ss);
		YAML::Node doc;
		parser.GetNextDocument(doc);

		const YAML::Node& yamlDriverInfo = doc["DriverInfo"];

		for (YAML::Iterator itr = yamlDriverInfo.begin(); itr != yamlDriverInfo.end(); ++itr) {

			std::string k;
			itr.first() >> k;
			if (k == "DriverCarFuelKgPerLtr") {
				float v;
				itr.second() >> v;
				if (v != DriverInfo::fuelKilosPerLiter) {
					DriverInfo::fuelKilosPerLiter = v;
					em << YAML::Key << "FuelKilosPerLiter" << YAML::Value << v;
				}
			} else if (k == "DriverCarIdx") {
				int v;
				itr.second() >> v;
				DriverInfo::driverCarIndex = v;
			} else if (k == "DriverCarRedLine") {
				float v;
				itr.second() >> v;
				if (v != DriverInfo::carRedline) {
					DriverInfo::carRedline = v;
					em << YAML::Key << "RPMRedline" << YAML::Value << v;
				}
			} else if (k == "Drivers") {
				const YAML::Node& drivers = itr.second();
				std::vector<DriverPtr> changedDrivers;

				for (YAML::Iterator ditr = drivers.begin(); ditr != drivers.end(); ++ditr) {
					const YAML::Node& dvr = *ditr;

					int carIdx;
					dvr["CarIdx"] >> carIdx;
					if (DriverInfo::driversByIndex.find(carIdx) == DriverInfo::driversByIndex.end()) {
						// TODO Notify of change.
						DriverPtr driver = generateNewDriver(dvr);
						changedDrivers.push_back(driver);
						DriverInfo::addDriver(driver);
					}
				}

				if (changedDrivers.size() > 0) {
					em << YAML::Key << "Drivers" << YAML::Value << YAML::BeginSeq;
					BOOST_FOREACH( DriverPtr ptr, changedDrivers) {
						em << YAML::BeginMap;
						em << YAML::Key << "DriverName" << YAML::Value << ptr->driverName;
						em << YAML::Key << "CarNumber" << YAML::Value << ptr->carNumber;
						em << YAML::Key << "CarPath" << YAML::Value << ptr->carPath;
						em << YAML::Key << "CarClassID" << YAML::Value << ptr->carClassID;
						em << YAML::Key << "CarID" << YAML::Value << ptr->carId;
						em << YAML::EndMap;
					}
					em << YAML::EndSeq;
				}
			}
		}

		if (em.size() > 23 ) { // Magic value
			std::cerr << em.c_str() << std::endl;
			_publisher->publish(em.c_str());
		}
	}
	return true;
}

DriverPtr SessionInfoHandler::generateNewDriver(const YAML::Node& dvr) {
	DriverPtr driver ( new Driver() );
	dvr["CarIdx"] >> driver->carIndex;
	dvr["UserName"] >> driver->driverName;
	dvr["UserID"] >> driver->userId;
	dvr["CarNumber"] >> driver->carNumber;
	dvr["CarPath"] >> driver->carPath;
	dvr["CarClassID"] >> driver->carClassID;
	dvr["CarID"] >> driver->carId;

	int intCarNum = atoi( driver->carNumber.c_str() );
	std::stringstream ss;
	ss << intCarNum;
	int lengthDiff = driver->carNumber.length() - ss.str().length();
	intCarNum = padCarNum(intCarNum, lengthDiff);
	driver->encodedCarNumber = intCarNum;

	return driver;
}