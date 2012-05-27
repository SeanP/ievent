#pragma once

#include "boost/shared_ptr.hpp"
#include "yaml-cpp/yaml.h"

typedef boost::shared_ptr<YAML::Node> NodePtr;

namespace IEvent
{
	namespace Service
	{
		

		class RequestHandler
		{
		public:
			RequestHandler(void);
			virtual ~RequestHandler(void);

			virtual std::string handleRequest(std::string messageType, const YAML::Node& node) = 0;

		}; 
		typedef boost::shared_ptr<RequestHandler> RequestHandlerPtr;
	}


}