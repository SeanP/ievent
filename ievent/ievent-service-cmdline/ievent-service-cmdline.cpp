// ievent-service-cmdline.cpp : Defines the entry point for the console application.
//

#include <stdafx.h>
#include <Windows.h>

#include "irreader.hpp"


int main(int argc, char* argv[])
{
	SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);

	IEvent::Service::iRacingReader reader;

	reader.setIsRunning(true);
	reader.setRunOnce(true);

	reader.run();
}

