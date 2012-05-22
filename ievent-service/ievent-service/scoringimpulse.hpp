#include <string>
#include "boost/shared_ptr.hpp"
#include "yaml-cpp/yaml.h"


#pragma once

namespace IEvent {
	namespace Service {
		class ScoringImpulse
		{
		public:
			ScoringImpulse(int carIndex);
			~ScoringImpulse();

			void setSectorChange(int oldSector, int newSector);
			void setCrossedSF(bool didCrossSF);
			void setPitted(bool didPit);
			void setTime(float sectorTime);
			void reset();

			YAML::Node toYaml();
			std::string toString();
		private:
			int _carIndex;

			bool _didCrossSF;
			bool _didCrossSector;
			bool _didPitThisLap;

			int _currentSector;
			int _oldSector;

			float _crossingTimeTick;
		};

		

		typedef boost::shared_ptr<IEvent::Service::ScoringImpulse> ScoringImpulsePtr;
	}
}