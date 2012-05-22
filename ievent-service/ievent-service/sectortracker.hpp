#include <string>
#include <vector>
#include "irsdk_defines.h"
#include "scoringimpulse.hpp"

#pragma once

namespace IEvent {
	namespace Service {
		class SectorTracker
		{
		public:
			SectorTracker(int carIndex);
			~SectorTracker();

			static void setSplitTrackPcts(std::vector<float> pcts);

			bool setNewPosition(int tick, float trackPct, irsdk_TrkLoc surfaceType);

			ScoringImpulsePtr getImpulse();

		private:
			static std::vector<float> _sectors;
			static int _lastTrackSector;

			ScoringImpulsePtr _scoringImpulse;
			int _carIndex;

			int _inSector;
			float _trackPos;
			int _lastTick;

			int _crossedSFAt;
			float _crossedSFAtDecimal;

			int _crossedLastSectorAt;
			float _crossedLastSectorAtDecimal;

			bool _pitted;
			bool _suppressPit;
		};

		typedef boost::shared_ptr<IEvent::Service::SectorTracker> SectorTrackerPtr;
	}
}