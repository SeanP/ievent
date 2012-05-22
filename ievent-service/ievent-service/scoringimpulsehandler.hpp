#include "updatehandler.hpp"

#include "irsdk_defines.h"
#include "sectortracker.hpp"

#pragma once

namespace IEvent {
	namespace Service {
		class ScoringImpulseHandler : public UpdateHandler {
		public:
			ScoringImpulseHandler();
			bool handleUpdate(const irsdk_header *pHeader, char *ir_data);

		private:
			static const int _maxCars = 64;
			const irsdk_varHeader* _ir_carIdxLapPct;
			const irsdk_varHeader* _ir_carIdxTrackSurface;
			//float _previousPct[_maxCars];
			std::vector<SectorTrackerPtr> _cars;
			int _lastHeaderVersion;
			
		};
	}
}