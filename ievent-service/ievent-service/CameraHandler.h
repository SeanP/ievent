#pragma once
#include "updatehandler.hpp"
#include "CameraState.h"


namespace IEvent
{
	namespace Service
	{
		class CameraHandler :
			public UpdateHandler
		{
		public:
			CameraHandler(void);
			~CameraHandler(void);

			bool handleUpdate(const irsdk_header *pHeader, char *ir_data);

		private:
			CameraState _camera;
			const irsdk_varHeader* _ir_camCarIdx;
			const irsdk_varHeader* _ir_camGroupNumber;
			const irsdk_varHeader* _ir_camNumber;
			const irsdk_varHeader* _ir_cameraState;
			const irsdk_varHeader* _ir_replaySpeed;
			const irsdk_varHeader* _ir_isSlowMotion;
		};

	}

}