/**
 * Author: Sean Pivek
 * Date: 2012-05-06
 */

#include "stdafx.h"
#include <string>
#include "zmq.hpp"
#include <iostream>
#include <sstream>


namespace IEvent {
	namespace Service {
		class Publisher {
		public:
			Publisher(int threads, int port);
			~Publisher();

			bool publish(std::string subject, std::string message);	
			
		private:
			zmq::context_t _context;
			zmq::socket_t _publisher;
		};

		
	}
}
