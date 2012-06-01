#include "SessionInfoRequestHandler.h"

#include "boost/foreach.hpp"

#include "SessionInfo.h"
#include "Types.hpp"

using namespace IEvent::Service;

SessionInfoRequestHandler::SessionInfoRequestHandler(void)
{
}


SessionInfoRequestHandler::~SessionInfoRequestHandler(void)
{
}


std::string SessionInfoRequestHandler::handleRequest(std::string messageType, const YAML::Node& yaml) {
	//const YAML::Node& yaml = *node;
	EmitterPtr em ( new YAML::Emitter() );
	(*em) << YAML::BeginMap << YAML::Key << "Response" << YAML::Value << "OK" ;

	if (messageType == "GetAllSessions") {
		(*em) << YAML::Key << "SessionInfo" << YAML::Value << YAML::BeginMap;
		(*em) << YAML::Key << "CurrentSession" << YAML::Value << SessionInfo::currentSession;
		(*em) << YAML::Key << "Sessions" << YAML::Value << YAML::BeginSeq;
		std::pair<int, SessionPtr> p;
		BOOST_FOREACH(p, SessionInfo::sessions) {
			SessionPtr sptr = p.second;
			(*em) << YAML::BeginMap << YAML::Key << "SessionNum" << YAML::Value << sptr->sessionNum;
			(*em) << YAML::Key << "Results" << YAML::Value <<YAML::BeginSeq;
			std::pair <int, ResultsEntryPtr> resultPair;
			BOOST_FOREACH (resultPair, sptr->resultsByPosition) {
				resultsEntryToYaml(resultPair.second, em);
			}
			(*em) << YAML::EndSeq << YAML::EndMap;
		}
		(*em) << YAML::EndSeq << YAML::EndMap << YAML::EndMap;
	}
	return em->c_str();
}