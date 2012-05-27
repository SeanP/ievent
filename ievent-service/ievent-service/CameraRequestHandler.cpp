#include "CameraRequestHandler.h"

using namespace IEvent::Service;

CameraRequestHandler::CameraRequestHandler(void)
{
}


CameraRequestHandler::~CameraRequestHandler(void)
{
}

std::string CameraRequestHandler::handleRequest(std::string messageType, NodePtr node) {
	const YAML::Node& yaml = *node;
	if (messageType == "CameraSetCar") {
		std::string carNumber;
		yaml["CarNumber"] >> carNumber;
		// TODO: Get car index from CarNumber, then set the camera
	}

	YAML::Emitter em;
	em << YAML::BeginMap << YAML::Key << "Response" << YAML::Value << "OK" << YAML::EndMap;
	return em.c_str();
}
