#include "CameraHandler.h"

#include "yaml-cpp/yaml.h"
#include "DriverInfo.h"

#include <iostream>

using namespace IEvent::Service;

CameraHandler::CameraHandler(PublisherPtr publisher) :
	_ir_camCarIdx(irsdk_getVarHeaderEntry(irsdk_varNameToIndex("CamCarIdx"))),
	_ir_camGroupNumber(irsdk_getVarHeaderEntry(irsdk_varNameToIndex("CamGroupNumber"))),
	_ir_camNumber(irsdk_getVarHeaderEntry(irsdk_varNameToIndex("CamCameraNumber"))),
	_ir_cameraState(irsdk_getVarHeaderEntry(irsdk_varNameToIndex("CamCameraState"))),
	_ir_replaySpeed(irsdk_getVarHeaderEntry(irsdk_varNameToIndex("ReplayPlaySpeed"))),
	_ir_isSlowMotion(irsdk_getVarHeaderEntry(irsdk_varNameToIndex("ReplayPlaySlowMotion"))),
	_publisher(publisher)
{
}


CameraHandler::~CameraHandler(void)
{
}

bool CameraHandler::handleUpdate(const irsdk_header *pHeader, char *ir_data) {
	if (ir_data == NULL) {
		return false;
	}

	YAML::Emitter em;

	em << YAML::BeginMap << YAML::Key << "CameraUpdate" << YAML::Value << YAML::BeginMap;

	int *focusedCar =  (int *) (ir_data + _ir_camCarIdx->offset);
	int *cameraGroup =  (int *) (ir_data + _ir_camGroupNumber->offset);
	int *cameraNumber =  (int *) (ir_data + _ir_camNumber->offset);
	int *cameraState =  (int *) (ir_data + _ir_cameraState->offset);
	int *replaySpeed =  (int *) (ir_data + _ir_replaySpeed->offset);
	bool *isSlowMotion =  (bool *) (ir_data + _ir_isSlowMotion->offset);

	if ( (*focusedCar) != _camera.getFocusCarIdx() ) {
		_camera.setFocusCarIdx(*focusedCar);
		assert(DriverInfo::driversByIndex.count(*focusedCar) != 0);
		em << YAML::Key << "FocusCar" << YAML::Value << DriverInfo::driversByIndex[*focusedCar]->carNumber;
	}

	if ( (*cameraGroup) != _camera.getCameraGroup() ) {
		_camera.setCameraGroup(*cameraGroup);
		//std::cerr << "New camera group is " << (*cameraGroup) << std::endl;
	}

	if ( (*cameraNumber) != _camera.getCameraNumber() ) {
		_camera.setCameraNumber(*cameraNumber);
		//std::cerr << "New camera number is " << (*cameraNumber) << std::endl;
	}

	if ( (*replaySpeed) != _camera.getPlaybackSpeed() ) {
		_camera.setPlaybackSpeed(*replaySpeed);
		em << YAML::Key << "CameraSpeed" << YAML::Value << *replaySpeed;
	}

	if ( (*isSlowMotion) != _camera.isSlowMotionEnabled() ) {
		_camera.setSlowMotionEnabled(*isSlowMotion);
		em << YAML::Key << "SlowMotion" << YAML::Value << *isSlowMotion/* == true ? "True" : "False")*/;
	}

	if ( (*cameraState) != _camera.getCameraStateBitfield() ) {
		int diff = (*cameraState) ^ _camera.getCameraStateBitfield();
		_camera.setCameraStateBitfield(*cameraState);

		//if (diff & irsdk_IsSessionScreen) {
		//	std::cerr << "The session screen is currently " << ( (*cameraState) & irsdk_IsSessionScreen ? "active" : "inactive") << std::endl;
		//}
		//if (diff & irsdk_IsScenicActive) {
		//	std::cerr << "The scenic camera is currently " << ( (*cameraState) & irsdk_IsScenicActive ? "active" : "inactive") << std::endl;
		//}
		//if (diff & irsdk_CamToolActive) {
		//	std::cerr << "The camera tool is currently " << ( (*cameraState) & irsdk_CamToolActive ? "active" : "inactive") << std::endl;
		//}
		//if (diff & irsdk_UIHidden) {
		//	std::cerr << "The UI is currently " << ( (*cameraState) & irsdk_UIHidden ? "hidden" : "visible") << std::endl;
		//}
		//if (diff & irsdk_UseAutoShotSelection) {
		//	std::cerr << "Auto shot selection is currently " << ( (*cameraState) & irsdk_UseAutoShotSelection ? "active" : "inactive") << std::endl;
		//}
		//if (diff & irsdk_UseTemporaryEdits) {
		//	std::cerr << "Temporary edits are currently " << ( (*cameraState) & irsdk_UseTemporaryEdits ? "active" : "inactive") << std::endl;
		//}
		//if (diff & irsdk_UseKeyAcceleration) {
		//	std::cerr << "Key acceleration is currently " << ( (*cameraState) & irsdk_UseKeyAcceleration ? "active" : "inactive") << std::endl;
		//}
		//if (diff & irsdk_UseKey10xAcceleration) {
		//	std::cerr << "Fast key acceleration is currently " << ( (*cameraState) & irsdk_UseKey10xAcceleration ? "active" : "inactive") << std::endl;
		//}
		//if (diff & irsdk_UseMouseAimMode) {
		//	std::cerr << "Mouse aim mode is currently " << ( (*cameraState) & irsdk_UseMouseAimMode ? "active" : "inactive") << std::endl;
		//}

	}

	em << YAML::EndMap << YAML::EndMap;

	if (em.size() > 18 ) { // Magic value
		_publisher->publish(em.c_str());
		//std::cerr << em.c_str() << std::endl;
	}

	return true;	
}