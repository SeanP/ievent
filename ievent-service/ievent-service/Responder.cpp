#include "Responder.h"

#include <map>
#include <sstream>

using namespace IEvent::Service;

Responder::Responder(int threads, int port) :
	_context(threads),
	_socket(_context, ZMQ_REP),
	_handlers()
{
	std::stringstream ss;
	ss << "tcp://*:" << port;
	_socket.bind(ss.str().c_str());
	std::cerr << "Bound to " << ss.str() << std::endl;
}

Responder::~Responder(void)
{
	_socket.close();
}

bool Responder::registerHandler(std::string messageType, RequestHandlerPtr handler) {
	_handlers.insert(std::pair<std::string, RequestHandlerPtr>(messageType, handler));
	return true;
}

void Responder::start() {
	thread = boost::thread(&Responder::run, this);
}

void Responder::run() {
	while (true) {
		zmq::message_t request;

		_socket.recv (&request);
		std::stringstream ss;
		ss << (char *) request.data();

		YAML::Parser parser(ss);
		YAML::Node doc;

		/*
		* BIG FAIL WARNING!
		* We assume there is one YAML document per ZMQ message.  If there's
		* more than one, we'll ignore all but the first.
		*/
		parser.GetNextDocument(doc);
		std::string messageType;
		doc["MessageType"] >> messageType;

		std::string response;
		if (_handlers.count(messageType) > 0) {
			RequestHandlerPtr handler = _handlers[messageType];
			response = handler->handleRequest( messageType, doc);
		} else {
			YAML::Emitter em;
			em << YAML::BeginMap << YAML::Key << "Response" << YAML::Value << "ERROR";
			em << YAML::Key << "Why" << YAML::Value << "Unknown operation" << YAML::EndMap;
			response = std::string (em.c_str());
		}
		zmq::message_t responseMsg( response.length() );
		memcpy( (void *) responseMsg.data(), response.c_str(), response.length());

		_socket.send(responseMsg);
	}
}
