#include "SessionInfoHandler.h"

#include <sstream>
#include "yaml-cpp/yaml.h"
#include "boost/foreach.hpp"

#include "DriverInfo.h"
#include "Driver.h"

#include "SessionInfo.h"

using namespace IEvent::Service;

SessionInfoHandler::SessionInfoHandler(PublisherPtr publisher) :
	_lastSessionInfoStringVersion(-1),
	_publisher(publisher),
	_ir_currentSession(irsdk_getVarHeaderEntry(irsdk_varNameToIndex("SessionNum")))
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
		handleDriverInfo(yamlDriverInfo);

		const YAML::Node& yamlSessionInfo = doc["SessionInfo"];
		handleSessionInfo(yamlSessionInfo, ir_data);

	}
	return true;
}


void SessionInfoHandler::handleSessionInfo(const YAML::Node& yamlSessionInfo, char* ir_data) {
	if (ir_data == NULL) {
		return;
	}
	int *currentSession = (int *) (ir_data + _ir_currentSession->offset);

	boost::shared_ptr<YAML::Emitter> em (new YAML::Emitter);
	(*em) << YAML::BeginMap << YAML::Key << "SessionInfoUpdate" << YAML::Value << YAML::BeginMap;

	if (*currentSession != SessionInfo::currentSession) {
		(*em) << YAML::Key << "CurrentSession" << YAML::Value << *currentSession;
		SessionInfo::currentSession = *currentSession;
	}

	(*em) << YAML::Key << "Sessions" << YAML::Value << YAML::BeginSeq;



	const YAML::Node& yamlSessions = yamlSessionInfo["Sessions"];
	for (YAML::Iterator itr = yamlSessions.begin(); itr != yamlSessions.end(); ++itr) {

		std::vector<pair<std::string, int>> changedInts;
		std::vector<pair<std::string, float>> changedFloats;
		std::vector<pair<std::string, bool>> changedBools;
		std::vector<pair<std::string, std::string>> changedStrings;
		std::vector<ResultsEntryPtr> changedEntries;

		const YAML::Node& session = *itr;
		int sessionId;
		session["SessionNum"] >> sessionId;

		SessionPtr sptr;
		if (SessionInfo::sessions.count(sessionId) == 0) {
			sptr = SessionPtr ( new Session() );
			SessionInfo::sessions.insert( std::pair<int, SessionPtr>(sessionId, sptr) );
			sptr->sessionNum = sessionId;
		} else {
			sptr = SessionInfo::sessions[sessionId];
		}


		for (YAML::Iterator sitr = session.begin(); sitr != session.end(); ++sitr) {

			std::string k;
			sitr.first() >> k;

			/*			if (k == "SessionLaps" ) {
			std::string sv;
			int v;
			sitr.second() >> sv;
			if (sv == "unlimited") {
			v = -1;
			} else {
			v = atoi(sv.c_str());
			}

			if ( v != sptr->sessionLapLimit ) {
			sptr->sessionLapLimit = v;
			changedInts.push_back(std::pair<std::string, int>("SessionLaps", v));
			}
			} else if (k == "SessionTime" ) {
			float v;
			sitr.second() >> v;
			if (v != sptr->sessionTimeLimit) {
			sptr->sessionTimeLimit = v;
			changedFloats.push_back(std::pair<std::string, float>("SessionTime", v));
			}
			} else if (k == "SessionType") {
			std::string v;
			sitr.second() >> v;
			if (v != sptr->sessionType) {
			sptr->sessionType = v;
			changedStrings.push_back(std::pair<std::string, std::string>("SessionType", v));
			}
			} else if (k == "ResultsAverageLapTime") {
			float v;
			sitr.second() >> v;
			if (v != sptr->avgLapTime) {
			sptr->avgLapTime = v;
			changedFloats.push_back(std::pair<std::string, float>("AverageLapTime", v));
			}
			} else if (k == "ResultsNumCautionFlags") {
			int v;
			sitr.second() >> v;
			if (v != sptr->numCautions) {
			sptr->numCautions = v;
			changedInts.push_back(std::pair<std::string, int>("NumCautions", v));
			}
			} else if (k == "ResultsNumCautionLaps") {
			int v;
			sitr.second() >> v;
			if (v != sptr->numCautionLaps) {
			sptr->numCautionLaps = v;
			changedInts.push_back(std::pair<std::string, int>("CautionLaps", v));
			}
			} else if (k == "ResultsNumLeadChanges") {
			int v;
			sitr.second() >> v;
			if (v != sptr->numLeadChanges) {
			sptr->numLeadChanges = v;
			changedInts.push_back(std::pair<std::string, int>("LeadChanges", v));
			}
			} else if (k == "ResultsLapsComplete") {
			int v;
			sitr.second() >> v;
			if (v != sptr->lapsComplete) {
			sptr->lapsComplete = v;
			changedInts.push_back(std::pair<std::string, int>("LapsComplete", v));
			}
			} else if (k == "ResultsOfficial") {
			bool v;
			sitr.second() >> v;
			if (v != sptr->isOfficial) {
			sptr->isOfficial = v;
			changedBools.push_back(std::pair<std::string, bool>("IsOfficial", v));
			}
			} else*/ if (k == "ResultsPositions") {
				const YAML::Node& results = sitr.second();

				for (YAML::Iterator ritr = results.begin(); ritr != results.end(); ++ritr) {
					const YAML::Node& entry = *ritr;

					int position;
					int carIdx;
					bool changed = false;
					bool positionChanged = false;
					DriverPtr driver;
					entry["Position"] >> position;
					entry["CarIdx"] >> carIdx;
					driver = DriverInfo::driversByIndex[carIdx];

					ResultsEntryPtr ptr;
					if (SessionInfo::sessions[sessionId]->resultsByCarIdx.count(carIdx) > 0) {
						ptr = SessionInfo::sessions[sessionId]->resultsByCarIdx[carIdx];
					} else {
						ptr = ResultsEntryPtr ( new ResultsEntry() );
						ptr->position = position;
						ptr->driver = DriverInfo::driversByIndex[carIdx];
						assert(ptr->driver);
						SessionInfo::sessions[sessionId]->resultsByCarIdx[carIdx] = ptr;
						changed = true;
						positionChanged = true;
					}


					int fastestLapNumber;
					float fastestLapTime;
					int lap;
					int lapsComplete;
					float lapsDriven;
					int lapsLed;
					float lastLapTime;
					std::string reasonOut;
					float time;



					entry["FastestTime"] >> fastestLapTime;
					entry["FastestLap"] >> fastestLapNumber;
					entry["Lap"] >> lap;
					entry["LapsComplete"] >> lapsComplete;
					entry["LapsDriven"] >> lapsDriven;
					entry["LapsLed"] >> lapsLed;
					entry["LastTime"] >> lastLapTime;
					entry["ReasonOutStr"] >> reasonOut;
					entry["Time"] >> time;

					if (position != ptr->position) {
						ptr->position = position;
						changed = true;
						positionChanged = true;
					}

					if (fastestLapTime != ptr->fastestLapTime) {
						ptr->fastestLapTime = fastestLapTime;
						changed = true;
					}

					if (fastestLapNumber != ptr->fastestLapNumber) {
						ptr->fastestLapNumber = fastestLapNumber;
						changed = true;
					}

					if (lap != ptr->lap) {
						ptr->lap = lap;
						changed = true;
					}

					if (lapsComplete != ptr->lapsComplete) {
						ptr->lapsComplete = lapsComplete;
						changed = true;
					}

					if (lapsDriven != ptr->lapsDriven) {
						ptr->lapsDriven = lapsDriven;
						changed = true;
					}

					if (lapsLed != ptr->lapsLed) {
						ptr->lapsLed = lapsLed;
						changed = true;
					}

					if (lastLapTime != ptr->lastLapTime) {
						ptr->lastLapTime = lastLapTime;
						changed = true;
					}

					if (reasonOut != ptr->reasonOut) {
						ptr->reasonOut = reasonOut;
						changed = true;
					}

					if (time != ptr->time) {
						ptr->time = time;
						changed = true;
					}

					if (changed) {
						changedEntries.push_back(ptr);
					}

					if (positionChanged) {
						SessionInfo::sessions[sessionId]->resultsByPosition.erase(position);
						SessionInfo::sessions[sessionId]->resultsByPosition[position] = ptr;
					}

				} // for each result in session
			}
		} // for each record in session
		if (changedEntries.size() > 0) {
			(*em) << YAML::BeginMap << YAML::Key << "SessionNum" << YAML::Value << sessionId;
			(*em) << YAML::Key << "Results" << YAML::Value <<YAML::BeginSeq;
			BOOST_FOREACH (ResultsEntryPtr ptr, changedEntries) {
				resultsEntryToYaml(ptr, em);
			}
			(*em) << YAML::EndSeq << YAML::EndMap;
		}
	} // for each session
	(*em) << YAML::EndSeq << YAML::EndMap << YAML::EndMap;
}

void SessionInfoHandler::handleDriverInfo(const YAML::Node& yamlDriverInfo) {

	boost::shared_ptr<YAML::Emitter> em (new YAML::Emitter);

	(*em) << YAML::BeginMap << YAML::Key << "DriverInfoUpdate" << YAML::Value << YAML::BeginMap;

	for (YAML::Iterator itr = yamlDriverInfo.begin(); itr != yamlDriverInfo.end(); ++itr) {

		std::string k;
		itr.first() >> k;
		if (k == "DriverCarFuelKgPerLtr") {
			float v;
			itr.second() >> v;
			if (v != DriverInfo::fuelKilosPerLiter) {
				DriverInfo::fuelKilosPerLiter = v;
				(*em) << YAML::Key << "FuelKilosPerLiter" << YAML::Value << v;
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
				(*em) << YAML::Key << "RPMRedline" << YAML::Value << v;
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
				(*em) << YAML::Key << "Drivers" << YAML::Value << YAML::BeginSeq;
				BOOST_FOREACH( DriverPtr ptr, changedDrivers) {
					IEvent::Service::driverToYaml(ptr, em);
				}
				(*em) << YAML::EndSeq;
			}
		}
	}

	if ((*em).size() > 22 ) { // Magic value
		_publisher->publish((*em).c_str());
	}
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