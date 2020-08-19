
#define RAY_EXPORT
#include "export.h"

#include "utils\singleton.h"

namespace ray {
	namespace recorder {

		class Recorder :
			public IRecorder,
			public utils::Singleton<Recorder>
		{

		private:
			Recorder() {}
			~Recorder() { release(); }

			// coz Singleton and auto_ptr need to call 
			// construct and deconstruct of Recorder
			friend utils::Singleton<Recorder>;
			friend class std::auto_ptr<Recorder>;

			Recorder(const Singleton&) = delete;
			Recorder& operator=(const Recorder&) = delete;

		public:
			virtual rt_error initialize(const rt_utf8 logPath[RECORDER_MAX_PATH_LEN]) override;

			virtual void release() override;

			virtual void getVersion(
				uint32_t *major,
				uint32_t *minor,
				uint32_t *patch,
				uint32_t *build) override;

			virtual void setEventHandler(const IRecorderEventHandler *handler) override;

			virtual void queryInterface(RECORDER_INTERFACE_IID iid, void **interface) override;
		};
	}
}