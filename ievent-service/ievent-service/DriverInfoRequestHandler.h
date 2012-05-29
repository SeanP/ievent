#pragma once
#include "requesthandler.h"
namespace IEvent
{
	namespace Service
	{
		class DriverInfoRequestHandler :
			public RequestHandler
		{
		public:
			DriverInfoRequestHandler(void);
			~DriverInfoRequestHandler(void);

			std::string handleRequest(std::string messageType, const YAML::Node& node);
		}; 
	}
}