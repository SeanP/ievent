#include "updatehandler.hpp"

namespace IEvent {
	namespace Service {
		class TrackOrder : public UpdateHandler
		{
		public:
			TrackOrder();
			~TrackOrder();
			bool handleUpdate();
		private:

		};

		TrackOrder::TrackOrder()
		{
		}

		TrackOrder::~TrackOrder()
		{
		}
	}
}
