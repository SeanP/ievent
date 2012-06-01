#pragma once

#include "boost/shared_ptr.hpp"

#include "Driver.h"

namespace IEvent
{
	namespace Service
	{
		struct ResultsEntry
		{
			DriverPtr driver;
			int fastestLapNumber;
			float fastestLapTime;
			int incidents; // WORTHLESS!
			int lap;
			int lapsComplete;
			float lapsDriven;
			int lapsLed;
			float lastLapTime;
			int position;
			int positionInClass;
			std::string reasonOut;
			float time;
		};

		typedef boost::shared_ptr<ResultsEntry> ResultsEntryPtr;

		void resultsEntryToYaml(ResultsEntryPtr ptr, EmitterPtr em);
	}

}
