#include "stdafx.h"

#include "boost/foreach.hpp"
#include "boost/thread.hpp"
#include "irreader.hpp"
#include "irsdk_defines.h"

#include "dummyhandler.hpp"
#include "scoringimpulsehandler.hpp"
#include "CameraHandler.h"
#include "SessionInfoHandler.h"
#include "CameraRequestHandler.h"
#include "DriverInfoRequestHandler.h"


IEvent::Service::iRacingReader::iRacingReader ():
	_running(false),
	_connected(false),
	_timeout(60000),
	_g_data(NULL),
	_g_nData(0),
	_resp(),
	_pub()
{
	RequestHandlerPtr camera ( new CameraRequestHandler() );
	_resp.registerHandler("CameraSetFocus", camera);

	RequestHandlerPtr drivers ( new DriverInfoRequestHandler() );
	_resp.registerHandler("GetAllDrivers", drivers);

	_resp.start();
}

IEvent::Service::iRacingReader::~iRacingReader () {
}

void IEvent::Service::iRacingReader::setIsRunning(bool isRunning) {
	_running = isRunning;
}

bool IEvent::Service::iRacingReader::init() {
	// Initialize network comms
	_pub = PublisherPtr( new Publisher() );

	// Set header size
	_g_nData = pHeader->bufLen;
	_g_data = new char[_g_nData];
	// Add handlers
	_handlers.clear();

	// Must be first -- populates SessionInfo namespaces other handlers read
	UpdatePtr sessionInfo ( new IEvent::Service::SessionInfoHandler(_pub) );
	_handlers.push_back(sessionInfo);

	UpdatePtr scoringImpulse ( new IEvent::Service::ScoringImpulseHandler(_pub) );
	_handlers.push_back(scoringImpulse);

	UpdatePtr camera ( new IEvent::Service::CameraHandler(_pub) );
	_handlers.push_back(camera);

	// End handlers

	_timeout = 1000;
	_connected = true;

	return true;
}

bool IEvent::Service::iRacingReader::shutdown() {
	_handlers.clear();
	_connected = false;
	_running = false; // For testing only!

	_pub = PublisherPtr();

	_timeout = 1000;

	if (_g_data) {
		delete _g_data;
		_g_data = NULL;
	}

	return true;
}


void IEvent::Service::iRacingReader::run() {

	int count = 0;
	while (_running) {

		// Check for data
		if ( irsdk_waitForDataReady(_timeout, _g_data) ) {
			pHeader = irsdk_getHeader();

			if ( !_g_data || _g_nData != pHeader->bufLen ) {
				// It's a new session, rerun init
				init();
			} else if (_g_data) {

				BOOST_FOREACH(UpdatePtr handler, _handlers) {
					handler->handleUpdate(pHeader, _g_data);
				}
				/*std::cerr<< irsdk_getSessionInfoStr() << std::endl;*/
				/*
				if (pHeader->numVars > 0) {
				for(int i=0; i<pHeader->numVars; i++) {
				const irsdk_varHeader *rec = irsdk_getVarHeaderEntry(i);

				std::cerr << "Name: " << rec->name << std::endl;
				}
				} else {
				std::cerr<< "No vars in header!" << std::endl;
				}				
				*/

			}
		} else if (_connected && irsdk_isConnected()) {
			// Lost the session, shut everything down
			std::cerr << "Lost connection" << std::endl;
			shutdown();
		} else {
			std::cerr << "Not connected" << std::endl;
		}
	}
}