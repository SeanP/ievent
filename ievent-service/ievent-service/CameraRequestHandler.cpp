#include "CameraRequestHandler.h"

#include "irsdk_defines.h"

#include "DriverInfo.h"
#include "Driver.h"

using namespace IEvent::Service;

CameraRequestHandler::CameraRequestHandler(void)
{
}


CameraRequestHandler::~CameraRequestHandler(void)
{
}

std::string CameraRequestHandler::handleRequest(std::string messageType, const YAML::Node& yaml) {
	//const YAML::Node& yaml = *node;
	if (messageType == "CameraSetCar") {
		std::string carNumber;
		yaml["CarNumber"] >> carNumber;

		if (DriverInfo::driversByCarNumber.count(carNumber) > 0) {

			std::cerr << "Attempting to change camera to " << carNumber << ", encoded number " << DriverInfo::driversByCarNumber[carNumber]->encodedCarNumber << std::endl;
			irsdk_broadcastMsg(irsdk_BroadcastCamSwitchNum, DriverInfo::driversByCarNumber[carNumber]->encodedCarNumber, 0, 0);
			std::cerr << "Attempted to change." << std::endl;
		}
	} else if (messageType == "CameraSetSpeed") {
		int speed = 1;
		bool slowMo = false;
		
		try {
			yaml["CameraSpeed"] >> speed;
			
		} catch (...) {}

		try {
			yaml["IsSlowMotion"] >> slowMo;
		} catch (...) {}

		std::cerr << "Attempting to change camera speed to " << speed << "x, slow mo " << (slowMo ? "enabled" : "disabled") << std::endl;
		irsdk_broadcastMsg(irsdk_BroadcastReplaySetPlaySpeed, speed, slowMo, 0);
		std::cerr << "Set camera speed." << std::endl;
	}

	YAML::Emitter em;
	em << YAML::BeginMap << YAML::Key << "Response" << YAML::Value << "OK" << YAML::EndMap;
	return em.c_str();
}
