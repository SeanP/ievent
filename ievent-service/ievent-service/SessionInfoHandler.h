#pragma once

#include "updatehandler.hpp"
#include "Driver.h"
#include "yaml-cpp/yaml.h"

namespace IEvent
{
	namespace Service
	{
		class SessionInfoHandler :
			public UpdateHandler
		{
		public:
			SessionInfoHandler();
			bool handleUpdate(const irsdk_header *pHeader, char *ir_data);

		private:
			int _lastSessionInfoStringVersion;
			DriverPtr generateNewDriver(const YAML::Node& node);
		};


	}
}