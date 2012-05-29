#include <boost/shared_ptr.hpp>

#include "irsdk_defines.h"
#include "publisher.hpp"

#pragma once

namespace IEvent {
	namespace Service {
		class UpdateHandler
		{
		public:
			virtual bool handleUpdate(const irsdk_header *pHeader, char *ir_data) = 0;
		private:
		};

		typedef boost::shared_ptr<IEvent::Service::UpdateHandler> UpdatePtr;
		
	}
}