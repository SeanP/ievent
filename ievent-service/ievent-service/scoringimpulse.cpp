#include "scoringimpulse.hpp"

IEvent::Service::ScoringImpulse::ScoringImpulse(int carIndex) :
	_carIndex(carIndex), _didCrossSector(false), _didCrossSF(false),
	_didPitThisLap(false), _currentSector(-1), _oldSector(-1),
	_crossingTimeTick(-1)
{
}

IEvent::Service::ScoringImpulse::~ScoringImpulse()
{
}

void IEvent::Service::ScoringImpulse::setPitted(bool didPit) {_didPitThisLap = didPit;}
void IEvent::Service::ScoringImpulse::setSectorChange(int oldSector, int newSector) {
	_currentSector = newSector;
	_oldSector = oldSector;
	_didCrossSector = true;
}
void IEvent::Service::ScoringImpulse::setCrossedSF(bool didCrossSF) {
	_didCrossSF = true;
}
void IEvent::Service::ScoringImpulse::setTime(float time) {
	_crossingTimeTick = time;
}

std::string IEvent::Service::ScoringImpulse::toString() {
	std::stringstream ss;
	ss << "Car " << _carIndex << " moved from sector " << _oldSector << " to "
		<< _currentSector << " at " << _crossingTimeTick << "tick.";
	if (_didCrossSF) {
		ss << "  The driver did ";
		if (!_didPitThisLap) {
			ss << "not ";
		}
		ss << "pit on this lap.";
	}
	return ss.str();
}

void IEvent::Service::ScoringImpulse::reset() {
	_didCrossSector = false;
	_didCrossSF = false;
	_didPitThisLap = false;
	_currentSector = -1;
	_oldSector = -1;
	_crossingTimeTick = -1;
}