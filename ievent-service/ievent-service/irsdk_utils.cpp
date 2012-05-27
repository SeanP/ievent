// Copyright (c) 2011 iRacing.com Motorsport Simulations, LLC.
// All rights reserved.

#include <windows.h>
#include <stdio.h>
#include <crtdbg.h>
#include <time.h>
#include <limits.h>

#include "irsdk_defines.h"

// for timeBeginPeriod()
#pragma comment(lib, "Winmm")
// for RegisterWindowMessageA() and SendMessage()
#pragma comment(lib, "User32")

// Local memory

static HANDLE hDataValidEvent = NULL;
static HANDLE hMemMapFile = NULL;

static const char *pSharedMem = NULL;
static const irsdk_header *pHeader = NULL;

static int lastTickCount = INT_MAX;
static bool isInitialized = false;

static const double timeout = 30.0; // timeout after 30 seconds with no communication
static time_t lastValidTime = 0;

// Function Implementations

bool irsdk_startup()
{
	if(!hMemMapFile)
	{
		hMemMapFile = OpenFileMappingA( FILE_MAP_READ, FALSE, IRSDK_MEMMAPFILENAME);
		lastTickCount = INT_MAX;
	}

	if(hMemMapFile)
	{
		if(!pSharedMem)
		{
			pSharedMem = (const char *)MapViewOfFile(hMemMapFile, FILE_MAP_READ, 0, 0, 0);
			pHeader = (irsdk_header *)pSharedMem;
			lastTickCount = INT_MAX;
		}

		if(pSharedMem)
		{
			if(!hDataValidEvent)
			{
				hDataValidEvent = OpenEventA(SYNCHRONIZE, false, IRSDK_DATAVALIDEVENTNAME);
				lastTickCount = INT_MAX;
			}

			if(hDataValidEvent)
			{
				isInitialized = true;
				return isInitialized;
			}
			//else printf("Error opening event: %d\n", GetLastError()); 
		}
		//else printf("Error mapping file: %d\n", GetLastError()); 
	}
	//else printf("Error opening file: %d\n", GetLastError()); 

	isInitialized = false;
	return isInitialized;
}

void irsdk_shutdown()
{
	if(hDataValidEvent)
		CloseHandle(hDataValidEvent);

	if(pSharedMem)
		UnmapViewOfFile(pSharedMem);

	if(hMemMapFile)
		CloseHandle(hMemMapFile);

	hDataValidEvent = NULL;
	pSharedMem = NULL;
	pHeader = NULL;
	hMemMapFile = NULL;

	isInitialized = false;
	lastTickCount = INT_MAX;
}

bool irsdk_getNewData(char *data)
{
	if(isInitialized || irsdk_startup())
	{
		_ASSERTE(NULL != pHeader);

		// if sim is not active, then no new data
		if(!(pHeader->status & irsdk_stConnected))
		{
			lastTickCount = INT_MAX;
			return false;
		}

		int latest = 0;
		for(int i=1; i<pHeader->numBuf; i++)
			if(pHeader->varBuf[latest].tickCount < pHeader->varBuf[i].tickCount)
			   latest = i;	

		// if newer than last recieved, than report new data
		if(lastTickCount < pHeader->varBuf[latest].tickCount)
		{
			// if asked to retrieve the data
			if(data)
			{
				// try twice to get the data out
				for(int count = 0; count < 2; count++)
				{
					int curTickCount =  pHeader->varBuf[latest].tickCount;
					memcpy(data, pSharedMem + pHeader->varBuf[latest].bufOffset, pHeader->bufLen);
					if(curTickCount ==  pHeader->varBuf[latest].tickCount)
					{
						lastTickCount = curTickCount;
						lastValidTime = time(NULL);
						return true;
					}
				}
				// if here, the data changed out from under us.
				return false;
			}
			else
			{
				lastTickCount =  pHeader->varBuf[latest].tickCount;
				lastValidTime = time(NULL);
				return true;
			}
		}
		// if older than last recieved, than reset, we probably disconnected
		else if(lastTickCount >  pHeader->varBuf[latest].tickCount)
		{
			lastTickCount =  pHeader->varBuf[latest].tickCount;
			return false;
		}
		// else the same, and nothing changed this tick
	}

	return false;
}


bool irsdk_waitForDataReady(int timeOut, char *data)
{
	_ASSERTE(timeOut >= 0);

	if(isInitialized || irsdk_startup())
	{
		// just to be sure, check before we sleep
		if(irsdk_getNewData(data))
			return true;

		// sleep till signaled
		WaitForSingleObject(hDataValidEvent, timeOut);

		// we woke up, so check for data
		if(irsdk_getNewData(data))
			return true;
		else
			return false;
	}

	// sleep if error
	if(timeOut > 0)
		Sleep(timeOut);

	return false;
}

bool irsdk_isConnected()
{
	if(isInitialized)
	{
		int elapsed = (int)difftime(time(NULL), lastValidTime);
		return (pHeader->status & irsdk_stConnected) > 0 && elapsed < timeout;
	}

	return false;
}

const irsdk_header *irsdk_getHeader()
{
	if(isInitialized)
	{
		return pHeader;
	}

	return NULL;
}

// direct access to the data buffer
// Warnign! This buffer is volitile so read it out fast!
// Use the cached copy from irsdk_waitForDataReady() or irsdk_getNewData() instead
const char *irsdk_getData(int index)
{
	if(isInitialized)
	{
		return pSharedMem + pHeader->varBuf[index].bufOffset;
	}

	return NULL;
}

const char *irsdk_getSessionInfoStr()
{
	if(isInitialized)
	{
		return pSharedMem + pHeader->sessionInfoOffset;
	}
	return NULL;
}

const irsdk_varHeader *irsdk_getVarHeaderPtr()
{
	if(isInitialized)
	{
		return ((irsdk_varHeader*)(pSharedMem + pHeader->varHeaderOffset));
	}
	return NULL;
}

const irsdk_varHeader *irsdk_getVarHeaderEntry(int index)
{
	if(isInitialized)
	{
		if(index >= 0 && index < pHeader->numVars)
		{
			return &((irsdk_varHeader*)(pSharedMem + pHeader->varHeaderOffset))[index];
		}
	}
	return NULL;
}

// Note: this is a linear search, so cache the results
int irsdk_varNameToIndex(const char *name)
{
	const irsdk_varHeader *pVar;

	if(name)
	{
		for(int index=0; index<pHeader->numVars; index++)
		{
			pVar = irsdk_getVarHeaderEntry(index);
			if(pVar && 0 == strncmp(name, pVar->name, IRSDK_MAX_STRING))
			{
				return index;
			}
		}
	}

	return -1;
}

int irsdk_varNameToOffset(const char *name)
{
	const irsdk_varHeader *pVar;

	if(name)
	{
		for(int index=0; index<pHeader->numVars; index++)
		{
			pVar = irsdk_getVarHeaderEntry(index);
			if(pVar && 0 == strncmp(name, pVar->name, IRSDK_MAX_STRING))
			{
				return pVar->offset;
			}
		}
	}

	return -1;
}

UINT irsdk_getBroadcastMsgID()
{
	static UINT msgId = RegisterWindowMessageA(IRSDK_BROADCASTMSGNAME); 

	return msgId;
}

void irsdk_broadcastMsg(irsdk_BroadcastMsg msg, int var1, int var2, int var3)
{
	irsdk_broadcastMsg(msg, var1, MAKELONG(var2, var3));
}

void irsdk_broadcastMsg(irsdk_BroadcastMsg msg, int var1, int var2)
{
	static UINT msgId = irsdk_getBroadcastMsgID();

	if(msgId && msg >= 0 && msg < irsdk_BroadcastLast)
	{
		SendNotifyMessage(HWND_BROADCAST, msgId, MAKELONG(msg, var1), var2);
	}
}

int padCarNum(int num, int zero)
{
	int retVal = num;
	int numPlace = 1;
	if(num > 99)
		numPlace = 3;
	else if(num > 9)
		numPlace = 2;
	if(zero)
	{
		numPlace += zero;
		retVal = num + 1000*numPlace;
	}

	return retVal;
}
