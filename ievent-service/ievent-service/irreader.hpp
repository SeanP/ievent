#include <vector>
#include <boost/shared_ptr.hpp>
#include "irsdk_defines.h"
#include "updatehandler.hpp"
#include "Responder.h"
#include "publisher.hpp"

namespace IEvent {
	namespace Service {
		class iRacingReader {
		public:
			iRacingReader ();
			~iRacingReader ();

			void run();

			void setIsRunning(bool isRunning);
			void setRunOnce(bool runOnce);
			bool isRunning();
		private:
			
			bool init();
			bool shutdown();

			int _timeout;
			
			const irsdk_header *pHeader;
			
			bool _running;
			bool _runOnce;
			bool _connected;
			char * _g_data;
			int _g_nData;
			time_t _g_ttime;
			Responder _resp;
			PublisherPtr _pub;
			std::vector<UpdatePtr> _handlers;
		};

	}
}