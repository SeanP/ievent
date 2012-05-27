#pragma once
namespace IEvent {
	namespace Service {
		class CameraState
		{
		public:
			CameraState(void);
			~CameraState(void);

			int getFocusCarIdx();
			int getCameraGroup();
			int getCameraNumber();
			int getPlaybackSpeed();
			int getCameraStateBitfield();

			void setFocusCarIdx(int value);
			void setCameraGroup(int value);
			void setCameraNumber(int value);
			void setPlaybackSpeed(int value);
			void setCameraStateBitfield(int value);
			void setSlowMotionEnabled(bool value);


			bool isSlowMotionEnabled();
			bool sessionScreen();
			bool scenic(); // the scenic camera is active (no focus car)

			//these can be changed with a broadcast message
			bool camToolActive();
			bool UIHidden();
			bool useAutoShotSelection();
			bool useTemporaryEdits();
			bool useKeyAcceleration();
			bool useFastKeyAcceleration();
			bool useMouseAimMode();

		private:
			int _focusCarIdx;
			int _cameraGroup;
			int _cameraNumber;

			int _playbackSpeed;

			unsigned int _cameraStateBitfield;
			bool _slowMotionEnabled;

		};
	}
}
