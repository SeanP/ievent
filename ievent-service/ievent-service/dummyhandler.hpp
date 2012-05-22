#include "updatehandler.hpp"

#include "irsdk_defines.h"

#pragma once

namespace IEvent {
	namespace Service {
		class DummyHandler : public UpdateHandler
		{
		public:
			DummyHandler();
			~DummyHandler();

			bool handleUpdate(const irsdk_header *pHeader, char *ir_data);
			int getUpdateCount();
		private:
			int _callcount;
		};
	}
}