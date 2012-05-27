#include "scoringimpulsehandler.hpp"

#include <sstream>
#include <vector>
#include "irsdk_defines.h"
#include "boost/foreach.hpp"
#include "yaml-cpp/yaml.h"

IEvent::Service::ScoringImpulseHandler::ScoringImpulseHandler() :
	_ir_carIdxLapPct(irsdk_getVarHeaderEntry(irsdk_varNameToIndex("CarIdxLapDistPct"))),
	_ir_carIdxTrackSurface(irsdk_getVarHeaderEntry(irsdk_varNameToIndex("CarIdxTrackSurface"))),
	_lastHeaderVersion(-1),
	_cars()
{
	std::vector<float> sectors;
	sectors.push_back(0.0f);
	sectors.push_back(0.33f);
	sectors.push_back(0.67f);
	SectorTracker::setSplitTrackPcts(sectors);
	for (int i=0; i < _maxCars; i++) {
		SectorTrackerPtr stp (new SectorTracker(i));
		_cars.push_back(stp);
	}
}


bool IEvent::Service::ScoringImpulseHandler::handleUpdate(const irsdk_header *pHeader, char *ir_data) {
	static int tick=0;
	float *lapDistPct = (float *) (ir_data + _ir_carIdxLapPct->offset);
	irsdk_TrkLoc *surfaceType = (irsdk_TrkLoc*) (ir_data + _ir_carIdxTrackSurface->offset);

	std::vector<ScoringImpulsePtr> impulses;
	for (int i=0; i < _ir_carIdxLapPct->count; i++) {
		SectorTrackerPtr stp = _cars.at(i);
		bool impulseExists = stp->setNewPosition(tick++, lapDistPct[i], surfaceType[i]);
		if (impulseExists) {
			impulses.push_back(_cars[i]->getImpulse());
		}

		/*
		if (_previousPct[i] - lapDistPct[i] > 0.5) {
			completedLap.push_back(i);
		}
		_previousPct[i] = lapDistPct[i];
		*/
	}
	
	//BOOST_FOREACH(ScoringImpulsePtr sip, impulses) {
	//	std::cerr << sip->toString() << std::endl;
	//}
	return true;
}