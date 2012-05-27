#include "SessionInfoHandler.h"

#include <sstream>
#include "yaml-cpp/yaml.h"

#include "DriverInfo.h"
#include "Driver.h"

using namespace IEvent::Service;

SessionInfoHandler::SessionInfoHandler(void) :
	_lastSessionInfoStringVersion(-1)
{
}


bool SessionInfoHandler::handleUpdate(const irsdk_header *pHeader, char *ir_data) {
	if (_lastSessionInfoStringVersion < pHeader->sessionInfoUpdate) {
		_lastSessionInfoStringVersion = pHeader->sessionInfoUpdate;

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
					//TODO Notify of change
					DriverInfo::fuelKilosPerLiter = v;
				}
			} else if (k == "DriverCarIdx") {
				int v;
				itr.second() >> v;
				DriverInfo::driverCarIndex = v;
			} else if (k == "DriverCarRedLine") {
				float v;
				itr.second() >> v;
				if (v != DriverInfo::carRedline) {
					//TODO Notify of change
					DriverInfo::carRedline = v;
				}
			} else if (k == "Drivers") {
				const YAML::Node& drivers = itr.second();
				for (YAML::Iterator ditr = drivers.begin(); ditr != drivers.end(); ++ditr) {
					const YAML::Node& dvr = *ditr;

					int carIdx;
					dvr["CarIdx"] >> carIdx;
					if (DriverInfo::driversByIndex.find(carIdx) == DriverInfo::driversByIndex.end()) {
						// TODO Notify of change.
						DriverPtr driver = generateNewDriver(dvr);
						DriverInfo::addDriver(driver);
					}
				}
			}
		}

		// TODO Deal with the changes.
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

	return driver;
}