#include "Responder.h"

#include <map>
#include <sstream>

using namespace IEvent::Service;

Responder::Responder(int threads=1, int port=63733) :
	_context(threads),
	_socket(_context, ZMQ_PUB),
	_handlers()
{
	std::stringstream ss;
	ss << "tcp://*:" << port;
	_socket.bind(ss.str().c_str());
}

Responder::~Responder(void)
{
	_socket.close();
}

bool Responder::registerHandler(std::string messageType, RequestHandlerPtr handler) {
	_handlers.insert(std::pair<std::string, RequestHandlerPtr>(messageType, handler));
	return true;
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

		RequestHandlerPtr handler = _handlers[messageType];
		if (handler != NULL) {
			NodePtr node(&doc);
			std::string response;
			response = handler->handleRequest( messageType, node);

			zmq::message_t responseMsg( response.length() );
			memcpy( (void *) responseMsg.data(), response.c_str(), response.length());

			_socket.send(responseMsg);
		}

	}
}
