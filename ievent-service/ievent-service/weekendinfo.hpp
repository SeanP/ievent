#include <string>

#pragma once

namespace IEvent {
	namespace Service {
		class WeekendInfo
		{
		public:
			WeekendInfo();
			~WeekendInfo();

		private:
			// TrackInfo
			int seriesId;
			int seasonId;
			int sessionId;
			int subsessionId;
			bool isOfficial;
			int raceWeek;
			std::string eventType;
			std::string category;

		};
	}
}