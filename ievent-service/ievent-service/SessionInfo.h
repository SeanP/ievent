#pragma once

#include <map>
#include "Session.h"


namespace IEvent
{
	namespace Service
	{
		namespace SessionInfo
		{
			extern std::map<int, SessionPtr> sessions;
			extern int currentSession;
		} 
	}

}
