#pragma once
#include "requesthandler.h"

namespace IEvent
{
	namespace Service
	{
		class SessionInfoRequestHandler :
			public RequestHandler
		{
		public:
			SessionInfoRequestHandler(void);
			~SessionInfoRequestHandler(void);

			std::string handleRequest(std::string messageType, const YAML::Node& node);
		}; 
	}
}
