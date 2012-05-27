#include "CameraState.h"

#include "irsdk_defines.h"

using namespace IEvent::Service;


CameraState::CameraState(void) :
	_focusCarIdx(-1),
	_cameraGroup(-1),
	_playbackSpeed(1),
	_cameraStateBitfield(0)
{
}


CameraState::~CameraState(void)
{
}

#pragma region Getters

int CameraState::getFocusCarIdx(){ return _focusCarIdx; }
int CameraState::getCameraGroup(){ return _cameraGroup; }
int CameraState::getCameraNumber(){ return _cameraNumber; }
int CameraState::getPlaybackSpeed(){ return _playbackSpeed; }
int CameraState::getCameraStateBitfield(){ return _cameraStateBitfield; }
bool CameraState::isSlowMotionEnabled(){ return _slowMotionEnabled; }

#pragma endregion



#pragma region Setters

void CameraState::setFocusCarIdx(int value){ _focusCarIdx = value; }
void CameraState::setCameraGroup(int value){ _cameraGroup = value; }
void CameraState::setCameraNumber(int value){ _cameraNumber = value; }
void CameraState::setPlaybackSpeed(int value){ _playbackSpeed = value; }
void CameraState::setCameraStateBitfield(int value){ _cameraStateBitfield = value; }
void CameraState::setSlowMotionEnabled(bool value) { _slowMotionEnabled = value; }

#pragma endregion



#pragma region Convenience

bool CameraState::sessionScreen(){ return _cameraStateBitfield & irsdk_IsSessionScreen; }
bool CameraState::scenic(){ return _cameraStateBitfield & irsdk_IsScenicActive; }
bool CameraState::camToolActive(){ return _cameraStateBitfield & irsdk_CamToolActive; }
bool CameraState::UIHidden(){ return _cameraStateBitfield & irsdk_UIHidden; }
bool CameraState::useAutoShotSelection(){ return _cameraStateBitfield & irsdk_UseAutoShotSelection; }
bool CameraState::useTemporaryEdits(){ return _cameraStateBitfield & irsdk_UseTemporaryEdits; }
bool CameraState::useKeyAcceleration(){ return _cameraStateBitfield & irsdk_UseKeyAcceleration; }
bool CameraState::useFastKeyAcceleration(){ return _cameraStateBitfield & irsdk_UseKey10xAcceleration; }
bool CameraState::useMouseAimMode(){ return _cameraStateBitfield & irsdk_UseMouseAimMode; }  

#pragma endregion
