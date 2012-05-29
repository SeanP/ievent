#include "stdafx.h"
#include "publisher.hpp"
#include <stdio.h>

// TODO This needs to be YAMLized.
bool IEvent::Service::Publisher::publish(std::string subject, std::string message) {
	int length = subject.length() + message.length() + 3;
	zmq::message_t zmqMessage(50);
	char *data = (char*) zmqMessage.data();
	sprintf_s(data, length, "%s: %s", subject.c_str(), message.c_str());
	return _publisher.send(zmqMessage);
	/*zmq::message_t zmessage(20);
	sprintf_s ((char *) message.data(), 20 ,
		"%05d %d %d", 98109, 55, 40);
	assert (_publisher);
	return _publisher.send(zmessage);*/
	return true;
}

bool IEvent::Service::Publisher::publish(std::string message) {
	zmq::message_t zmsg(message.length());
	memcpy(zmsg.data(), message.c_str(), message.length());

	return _publisher.send(zmsg);
}

IEvent::Service::Publisher::Publisher(int threads, int port):
	_context(threads),
	_publisher(_context, ZMQ_PUB)
{
	std::stringstream ss;
	ss << "tcp://*:" << port;
	_publisher.bind(ss.str().c_str());
}

IEvent::Service::Publisher::~Publisher() {
	_publisher.close();
}