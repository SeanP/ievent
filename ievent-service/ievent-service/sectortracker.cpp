#include "sectortracker.hpp"

#include "irsdk_defines.h"

std::vector<float> IEvent::Service::SectorTracker::_sectors;
int IEvent::Service::SectorTracker::_lastTrackSector = -1;

IEvent::Service::SectorTracker::SectorTracker(int carIndex) :
	_inSector(-1), _trackPos(-1), _lastTick(-1), _crossedSFAt(-1),
	_crossedSFAtDecimal(0), _crossedLastSectorAt(-1),
	_crossedLastSectorAtDecimal(0), _pitted(false), _carIndex(carIndex),
	_suppressPit(false)
{
	_scoringImpulse = ScoringImpulsePtr ( new ScoringImpulse(carIndex) );
}

IEvent::Service::SectorTracker::~SectorTracker()
{
}

void IEvent::Service::SectorTracker::setSplitTrackPcts(std::vector<float> pcts) {
	_sectors = pcts;
	_lastTrackSector = _sectors.size() - 1;
}

IEvent::Service::ScoringImpulsePtr IEvent::Service::SectorTracker::getImpulse() {
	return _scoringImpulse;
}

bool IEvent::Service::SectorTracker::setNewPosition(int tick, float trackPct, irsdk_TrkLoc surfaceType) {

	/* If the trackPct is -1, we're not on track */
	if (surfaceType == irsdk_NotInWorld) {
		_suppressPit = true;
		_pitted = true;
		return false;
	}

	_scoringImpulse->reset();
	
	switch (surfaceType) {
	case irsdk_InPitStall:
	case irsdk_AproachingPits:
		if (!_suppressPit) {
			_pitted = true;
			_suppressPit = true;
		}
		break;
	case irsdk_OnTrack:
		_suppressPit = false;
		break;
	default:
		break;
	}

	int currentSector = _lastTrackSector;

	/* Locate which split we're in */
	for (unsigned int i=1; i < _sectors.size(); i++) {
		if (trackPct < _sectors.at(i)) {
			currentSector = i-1;
			break;
		}
	}


	if (_inSector != currentSector) {

		float thisSectorCrossTimeDec;
		float thisSectorMark = _sectors[currentSector];
		// If we started a new lap, we need to
		// ensure the track positions are useful.

		float prevTrackPct = _trackPos > trackPct ? _trackPos - 1: _trackPos;
		

		thisSectorCrossTimeDec = (tick - _lastTick) * ((thisSectorMark - prevTrackPct) / (trackPct - prevTrackPct));
		int thisSectorCrossTimeTick = _lastTick;

		_scoringImpulse->setTime (thisSectorCrossTimeTick + thisSectorCrossTimeDec);
		_scoringImpulse->setSectorChange(_inSector, currentSector);

		// We also started a new lap
		// (and didn't reverse into S1 from S2).
		if (currentSector == 0 && _inSector == _lastTrackSector) {
			_scoringImpulse->setCrossedSF(true);
			_scoringImpulse->setPitted(_pitted);
			_pitted = false;
		}

		// Reset the last sector info
		_inSector = currentSector;

		return true;
	}
	return false;
}