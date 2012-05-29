#include "DriverInfoRequestHandler.h"

#include "boost/foreach.hpp"

#include "DriverInfo.h"

using namespace IEvent::Service;

DriverInfoRequestHandler::DriverInfoRequestHandler(void)
{
}


DriverInfoRequestHandler::~DriverInfoRequestHandler(void)
{
}

std::string DriverInfoRequestHandler::handleRequest(std::string messageType, const YAML::Node& yaml) {
	//const YAML::Node& yaml = *node;
	if (messageType == "GetAllDrivers") {
		EmitterPtr em ( new YAML::Emitter() );
		(*em) << YAML::BeginMap << YAML::Key << "Response" << YAML::Value << "OK";
		(*em) << YAML::Key << "Drivers" << YAML::Value << YAML::BeginSeq;

		std::pair<int, DriverPtr> pair;
		BOOST_FOREACH(pair, DriverInfo::driversByIndex) {
			driverToYaml(pair.second, em);
		}
		(*em) << YAML::EndSeq << YAML::EndMap;
		return em->c_str();	
	}
}