#pragma once

#include "updatehandler.hpp"
#include "Driver.h"
#include "yaml-cpp/yaml.h"
#include "publisher.hpp"

namespace IEvent
{
	namespace Service
	{
		class SessionInfoHandler :
			public UpdateHandler
		{
		public:
			SessionInfoHandler(PublisherPtr publisher);
			bool handleUpdate(const irsdk_header *pHeader, char *ir_data);

		private:
			DriverPtr generateNewDriver(const YAML::Node& node);

			int _lastSessionInfoStringVersion;
			PublisherPtr _publisher;

			const irsdk_varHeader* _ir_currentSession;

			void handleSessionInfo(const YAML::Node& yamlSessionInfo, char* ir_data);
			void handleDriverInfo(const YAML::Node& yamlDriverInfo);
		};


	}
}