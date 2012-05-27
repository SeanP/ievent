#pragma once
#include "requesthandler.h"


namespace IEvent
{
	namespace Service
	{
		class CameraRequestHandler :
			public RequestHandler
		{
		public:
			CameraRequestHandler(void);
			~CameraRequestHandler(void);

			std::string handleRequest(std::string messageType, NodePtr node);
		}; 
	}


}