// ievent-service-test.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#define BOOST_TEST_MODULE iEventServiceTest
#define BOOST_TEST_MAIN
#include "boost/test/unit_test.hpp"
#include "boost/pointer_cast.hpp"
#include "zmq.hpp"
#include "publisher.hpp"
#include <iostream>
#include "updatehandler.hpp"
#include "dummyhandler.hpp"
#include "irreader.hpp"

BOOST_AUTO_TEST_CASE ( PUBLISHER__instantiate_publish_string )
{
	zmq::context_t context(1);
	zmq::socket_t sub(context, ZMQ_SUB);
	sub.connect("tcp://localhost:63732");
	sub.setsockopt(ZMQ_SUBSCRIBE,"",0);
	IEvent::Service::Publisher p(1, 63732);
	Sleep(1000);
	BOOST_CHECK( p.publish("Subject", "message") );

	zmq::message_t update;
	sub.recv(&update);
	BOOST_CHECK_EQUAL( "Subject: message", (char *) update.data() );
}

BOOST_AUTO_TEST_CASE( DUMMY_UPDATE_HANDLER__sanity_check)
{
	IEvent::Service::UpdatePtr handler( new IEvent::Service::DummyHandler() );
	handler->handleUpdate(NULL, NULL);
	boost::shared_ptr<IEvent::Service::DummyHandler> dhandler = boost::dynamic_pointer_cast<IEvent::Service::DummyHandler>(handler);
	BOOST_CHECK_EQUAL( dhandler->getUpdateCount(), 1);
}


BOOST_AUTO_TEST_CASE( CONNECT_TO_SIM ) {
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	IEvent::Service::iRacingReader reader;

	reader.setIsRunning(true);

	reader.run();
}
