/**
 * Author: Sean Pivek
 * Date: 2012-05-06
 */

#pragma once

#include "stdafx.h"
#include <string>
#include "zmq.hpp"
#include <iostream>
#include <sstream>

#include "boost/shared_ptr.hpp"


namespace IEvent {
	namespace Service {
		class Publisher {
		public:
			Publisher(int threads = 1, int port=63732);
			~Publisher();

			bool publish(std::string subject, std::string message);	
			bool publish(std::string message);
			
		private:
			zmq::context_t _context;
			zmq::socket_t _publisher;
		};

		typedef boost::shared_ptr<Publisher> PublisherPtr;
	}
}
