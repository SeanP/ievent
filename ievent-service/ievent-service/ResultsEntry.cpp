#include "ResultsEntry.h"

using namespace IEvent::Service;

void IEvent::Service::resultsEntryToYaml(ResultsEntryPtr ptr, EmitterPtr em) {
	(*em) << YAML::BeginMap;
	(*em) << YAML::Key << "CarNumber" << YAML::Value << ptr->driver->carNumber;
	(*em) << YAML::Key << "FastestLapNumber" << YAML::Value << ptr->fastestLapNumber;
	(*em) << YAML::Key << "FastestLapTime" << YAML::Value << ptr->fastestLapTime;
	(*em) << YAML::Key << "Lap" << YAML::Value << ptr->lap;
	(*em) << YAML::Key << "LapsComplete" << YAML::Value << ptr->lapsComplete;
	(*em) << YAML::Key << "LapsDriven" << YAML::Value << ptr->lapsDriven;
	(*em) << YAML::Key << "LapsLed" << YAML::Value << ptr->lapsLed;
	(*em) << YAML::Key << "LastLapTime" << YAML::Value << ptr->lastLapTime;
	(*em) << YAML::Key << "Position" << YAML::Value << ptr->position;
	// TODO (*em) << YAML::Key << "PositionInClass" << YAML::Value << ptr->positionInClass;
	(*em) << YAML::Key << "ReasonOut" << YAML::Value << ptr->reasonOut;
	(*em) << YAML::Key << "Time" << YAML::Value << ptr->time;
	(*em) << YAML::EndMap;
}