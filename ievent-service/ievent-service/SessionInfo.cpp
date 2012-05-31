#include "SessionInfo.h"

using namespace IEvent::Service;


std::map<int, SessionPtr> IEvent::Service::SessionInfo::sessions;
int IEvent::Service::SessionInfo::currentSession = -1;