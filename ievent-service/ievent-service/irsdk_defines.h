#ifndef IRSDK_DEFINES_H
#define IRSDK_DEFINES_H

#include <time.h>

// Copyright (c) 2011 iRacing.com Motorsport Simulations, LLC.
// All rights reserved.

/*
 The IRSDK is a simple api that lets clients access telemetry data from the 
 iRacing simulator. It is broken down into several parts:

 - Live data
   Live data is output from the sim into a shared memory mapped file.  Any
   application can open this memory mapped file and read the telemetry data
   out.  The format of this data was laid out in such a way that it should be
   possible to access from any language that can open a windows memory mapped
   file, without needing an external api.

   There are two different types of data that the telemetry outputs,
   sessionInfo and variables: 
   
   Session info is for data that only needs to be updated every once in a
   while.  This data is output as a YAML formatted string.

   Variables, on the other hand, are output at a rate of 60 times a second.
   The varHeader struct defines each variable that the sim will output, while
   the varData struct gives details about the current line buffer that the vars
   are being written into.  Each variable is packed into a binary array with 
   an offset and length stored in the varHeader.  The number of variables 
   available can change depending on the car or session loaded.  But once the
   sim is running the variable list is locked down and will not change during a
   session.

   The sim writes a new line of variables every 16 ms, and then signals any
   listeners in order to wake them up to read the data.  Because the sim has no
   way of knowing when a listener is done reading the data, we triple buffer
   it in order to give all the clients enough time to read the data out.  This
   gives you a minimum of 16 ms to read the data out and process it.  So it is
   best to copy the data out before processing it.  You can use the function
   irsdk_waitForDataReady() to both wait for new data and copy the data to a
   local buffer.

 - Logged data
   Detailed information about the local drivers car can be logged to disk in
   the form of an ibt binary file.  This logging is enabled in the sim by
   typing alt-L at any time.  The ibt file format directly mirrors the format
   of the live data.

   It is stored as an irsdk_header followed immediately by an irsdk_diskSubHeader.
   After that the offsets in the irsdk_header point to the sessionInfo string, 
   the varHeader, and the varBuffer.

 - Remote Conrol
   You can control the camera selections and playback of a replay tape, from
   any external application by sending a windows message with the 
   irsdk_broadcastMsg() function.
*/

// Constant Definitions

static const char IRSDK_DATAVALIDEVENTNAME[] = "Local\\IRSDKDataValidEvent";
static const char IRSDK_MEMMAPFILENAME[] = "Local\\IRSDKMemMapFileName";
static const char IRSDK_BROADCASTMSGNAME[] = "IRSDK_BROADCASTMSG";

static const int IRSDK_MAX_BUFS = 4;
static const int IRSDK_MAX_STRING = 32;
// descriptions can be longer than max_string!
static const int IRSDK_MAX_DESC = 64; 

enum irsdk_StatusField
{
	irsdk_stConnected   = 1
};

enum irsdk_VarType
{
	// 1 byte
	irsdk_char = 0,
	irsdk_bool,

	// 4 bytes
	irsdk_int,
	irsdk_bitField,
	irsdk_float,

	// 8 bytes
	irsdk_double,

	//index, don't use
	irsdk_ETCount
};

static const int irsdk_VarTypeBytes[irsdk_ETCount] =
{
	1,		// irsdk_char
	1,		// irsdk_bool

	4,		// irsdk_int
	4,		// irsdk_bitField
	4,		// irsdk_float

	8		// irsdk_double
};

// bit fields
enum irsdk_EngineWarnings 
{
	irsdk_waterTempWarning		= 0x01,
	irsdk_fuelPressureWarning	= 0x02,
	irsdk_oilPressureWarning	= 0x04,
	irsdk_engineStalled			= 0x08,
	irsdk_pitSpeedLimiter		= 0x10,
	irsdk_revLimiterActive		= 0x20,
};

// global flags
enum irsdk_Flags
{
	// global flags
	irsdk_checkered				= 0x00000001,
	irsdk_white					= 0x00000002,
	irsdk_green					= 0x00000004,
	irsdk_yellow				= 0x00000008,
	irsdk_red					= 0x00000010,
	irsdk_blue					= 0x00000020,
	irsdk_debris				= 0x00000040,
	irsdk_crossed				= 0x00000080,
	irsdk_yellowWaving			= 0x00000100,
	irsdk_oneLapToGreen			= 0x00000200,
	irsdk_greenHeld				= 0x00000400,
	irsdk_tenToGo				= 0x00000800,
	irsdk_fiveToGo				= 0x00001000,
	irsdk_randomWaving			= 0x00002000,
	irsdk_caution				= 0x00004000,
	irsdk_cautionWaving			= 0x00008000,

	// drivers black flags
	irsdk_black					= 0x00010000,
	irsdk_disqualify			= 0x00020000,
	irsdk_servicible			= 0x00040000, // car is allowed service (not a flag)
	irsdk_furled				= 0x00080000,
	irsdk_repair				= 0x00100000,

	// start lights
	irsdk_startHidden			= 0x10000000,
	irsdk_startReady			= 0x20000000,
	irsdk_startSet				= 0x40000000,
	irsdk_startGo				= 0x80000000,
};


// status 
enum irsdk_TrkLoc
{
	irsdk_NotInWorld = -1,
	irsdk_OffTrack,
	irsdk_InPitStall,
	irsdk_AproachingPits,
	irsdk_OnTrack
};

enum irsdk_SessionState
{
	irsdk_StateInvalid,
	irsdk_StateGetInCar,
	irsdk_StateWarmup,
	irsdk_StateParadeLaps,
	irsdk_StateRacing,
	irsdk_StateCheckered,
	irsdk_StateCoolDown
};

enum irsdk_CameraState
{
	irsdk_IsSessionScreen          = 0x0001, // the camera tool can only be activated if viewing the session screen (out of car)
	irsdk_IsScenicActive           = 0x0002, // the scenic camera is active (no focus car)

	//these can be changed with a broadcast message
	irsdk_CamToolActive            = 0x0004,
	irsdk_UIHidden                 = 0x0008,
	irsdk_UseAutoShotSelection     = 0x0010,
	irsdk_UseTemporaryEdits        = 0x0020,
	irsdk_UseKeyAcceleration       = 0x0040,
	irsdk_UseKey10xAcceleration    = 0x0080,
	irsdk_UseMouseAimMode          = 0x0100
};


//----
// Remote controll the sim by sending these windows messages
enum irsdk_BroadcastMsg 
{
	irsdk_BroadcastCamSwitchPos = 0,      // car position, group, camera
	irsdk_BroadcastCamSwitchNum,	      // driver #, group, camera
	irsdk_BroadcastCamSetState,           // irsdk_CameraState, unused, unused 
	irsdk_BroadcastReplaySetPlaySpeed,    // speed, slowMotion, unused
	irskd_BroadcastReplaySetPlayPosition, // irsdk_RpyPosMode, Frame Number (high, low)
	irsdk_BroadcastReplaySearch,          // irsdk_RpySrchMode, unused, unused
	irsdk_BroadcastLast                   // unused placeholder
};

// Search replay tape for events
enum irsdk_RpySrchMode
{
	irsdk_RpySrch_ToStart = 0,
	irsdk_RpySrch_ToEnd,
	irsdk_RpySrch_PrevSession,
	irsdk_RpySrch_NextSession,
	irsdk_RpySrch_PrevLap,
	irsdk_RpySrch_NextLap,
	irsdk_RpySrch_PrevFrame,
	irsdk_RpySrch_NextFrame,
	irsdk_RpySrch_PrevIncident,
	irsdk_RpySrch_NextIncident,
	irsdk_RpySrch_Last                   // unused placeholder
};

enum irsdk_RpyPosMode
{
	irsdk_RpyPos_Begin = 0,
	irsdk_RpyPos_Current,
	irsdk_RpyPos_End,
	irsdk_RpyPos_Last                   // unused placeholder
};

//----
//

struct irsdk_varHeader
{
	int type;			// irsdk_VarType
	int offset;			// offset fron start of buffer row
	int count;			// number of entrys (array)
						// so length in bytes would be irsdk_VarTypeBytes[type] * count
	int pad[1];			// (16 byte align)

	char name[IRSDK_MAX_STRING];
	char desc[IRSDK_MAX_DESC];
	char unit[IRSDK_MAX_STRING];	// something like "kg/m^2"
};

struct irsdk_varBuf
{
	int tickCount;		// used to detect changes in data
	int bufOffset;		// offset from header
	int pad[2];			// (16 byte align)
};

struct irsdk_header
{
	int ver;				// api version 1 for now
	int status;				// bitfield using irsdk_StatusField
	int tickRate;			// ticks per second (60 or 360 etc)

	// session information, updated periodicaly
	int sessionInfoUpdate;	// Incremented when session info changes
	int sessionInfoLen;		// Length in bytes of session info string
	int sessionInfoOffset;	// Session info, encoded in YAML format

	// State data, output at tickRate

	int numVars;			// length of arra pointed to by varHeaderOffset
	int varHeaderOffset;	// offset to irsdk_varHeader[numVars] array, Describes the variables recieved in varBuf

	int numBuf;				// <= IRSDK_MAX_BUFS (3 for now)
	int bufLen;				// length in bytes for one line
	int pad1[2];			// (16 byte align)
	irsdk_varBuf varBuf[IRSDK_MAX_BUFS]; // buffers of data being written to
};

// sub header used when writing telemetry to disk
struct irsdk_diskSubHeader
{
	time_t sessionStartDate;
	double sessionStartTime;
	double sessionEndTime;
	int sessionLapCount;
	int sessionRecordCount;
};

//----
// Client function definitions

bool irsdk_startup();
void irsdk_shutdown();

bool irsdk_getNewData(char *data);
bool irsdk_waitForDataReady(int timeOut, char *data);
bool irsdk_isConnected();

const irsdk_header *irsdk_getHeader();
const char *irsdk_getData(int index);
const char *irsdk_getSessionInfoStr();

const irsdk_varHeader *irsdk_getVarHeaderPtr();
const irsdk_varHeader *irsdk_getVarHeaderEntry(int index);

int irsdk_varNameToIndex(const char *name);
int irsdk_varNameToOffset(const char *name);

unsigned int irsdk_getBroadcastMsgID();

//send a remote controll message to the sim
// var1, var2, and var3 are all 16 bits signed
void irsdk_broadcastMsg(irsdk_BroadcastMsg msg, int var1, int var2, int var3);
// var2 can be a full 32 bits
void irsdk_broadcastMsg(irsdk_BroadcastMsg msg, int var1, int var2);

int padCarNum(int num, int zero);

#endif //IRSDK_DEFINES_H
