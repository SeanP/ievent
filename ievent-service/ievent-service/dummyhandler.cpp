#include "stdafx.h"

#include <iostream>
#include "dummyhandler.hpp"


IEvent::Service::DummyHandler::DummyHandler() :
	_callcount(0)
{
}

IEvent::Service::DummyHandler::~DummyHandler()
{
}

bool IEvent::Service::DummyHandler::handleUpdate(const irsdk_header *pHeader, char *ir_data) {
	_callcount++;

	return true;
}

int IEvent::Service::DummyHandler::getUpdateCount() {
	return _callcount;
}