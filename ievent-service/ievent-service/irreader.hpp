#include <vector>
#include <boost/shared_ptr.hpp>
#include "irsdk_defines.h"
#include "updatehandler.hpp"
#include "Responder.h"

namespace IEvent {
	namespace Service {
		class iRacingReader {
		public:
			iRacingReader ();
			~iRacingReader ();

			void run();

			void setIsRunning(bool isRunning);
			bool isRunning();
		private:
			
			bool init();
			bool shutdown();

			int _timeout;
			
			const irsdk_header *pHeader;
			
			bool _running;
			bool _connected;
			char * _g_data;
			int _g_nData;
			time_t _g_ttime;
			Responder _resp;
			std::vector<UpdatePtr> _handlers;
		};

	}
}