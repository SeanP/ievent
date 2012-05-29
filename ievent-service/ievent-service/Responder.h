#pragma once

#include <map>
#include "RequestHandler.h"
#include "zmq.hpp"
#include "boost/thread.hpp"

namespace IEvent
{
	namespace Service
	{
		class Responder
		{
		public:
			Responder(int threads=1, int port=63733);
			~Responder(void);

			void start();

			bool registerHandler(std::string messageType, RequestHandlerPtr handler);
		private:
			zmq::context_t _context;
			zmq::socket_t _socket;
			void run();
			std::map<std::string, RequestHandlerPtr> _handlers;

			boost::thread thread;
		}; 
	}

}
