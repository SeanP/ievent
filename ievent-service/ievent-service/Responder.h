#pragma once

#include <map>
#include "RequestHandler.h"
#include "zmq.hpp"

namespace IEvent
{
	namespace Service
	{
		class Responder
		{
		public:
			Responder(int threads=1, int port=63733);
			~Responder(void);

			void run();

			bool registerHandler(std::string messageType, RequestHandlerPtr handler);
		private:
			zmq::context_t _context;
			zmq::socket_t _socket;

			std::map<std::string, RequestHandlerPtr> _handlers;
		}; 
	}

}
