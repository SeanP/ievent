#pragma once

#include "boost/shared_ptr.hpp"

#include "ResultsEntry.h"

#include <vector>

namespace IEvent
{
	namespace Service
	{
		struct Session
		{
			float avgLapTime;
			//std::map<int, ResultsEntryPtr> fastestLapByClass;
			int lapsComplete;
			int numCautions;
			int numCautionLaps;
			int numLeadChanges;
			bool isOfficial;
			int sessionLapLimit;
			float sessionTimeLimit;
			int sessionNum;
			std::string sessionType;

			std::map<int, ResultsEntryPtr> resultsByPosition;
			std::map<int, ResultsEntryPtr> resultsByCarIdx;
			std::map<int, ResultsEntryPtr> positionByClass;
		};

		typedef boost::shared_ptr<Session> SessionPtr;
	}
}

