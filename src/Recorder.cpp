#define RAY_EXPORT

#include "recorder.h"
#include "constants\version.h"
#include "utils\singleton.h"
#include "utils\strings.h"
#include "utils\log.h"

namespace ray {
	namespace recorder {

		class Recorder :
			public IRecorder,
			public utils::Singleton<Recorder>
		{

		private:
			Recorder() {

			}

			~Recorder() {
				release();
			}

			// coz Singleton and auto_ptr need to call 
			// construct and deconstruct of Recorder
			friend utils::Singleton<Recorder>;
			friend class std::auto_ptr<Recorder>;

			Recorder(const Singleton&) = delete;
			Recorder& operator=(const Recorder&) = delete;

		public:
			virtual rt_error initialize(const rt_utf8 logPath[RECORDER_MAX_PATH_LEN]) override {
				utils::InitLogImpl(utils::strings::utf8_unicode(logPath).c_str());

				return ERR_NONE;
			}

			virtual void release() override {

			}

			virtual void getVersion(
				uint32_t *major,
				uint32_t *minor,
				uint32_t *patch,
				uint32_t *build) override {

				if (major)
					*major = VER_MAJOR;

				if (minor)
					*minor = VER_MINOR;

				if (patch)
					*patch = VER_PATCH;

				if (build)
					*build = VER_BUILD;
			}

			virtual void setEventHandler(const IRecorderEventHandler *handler) override {

			}

			virtual void queryInterface(RECORDER_INTERFACE_IID iid, void **interface) override {

			}
		};

	}
}

RAY_API ray::recorder::IRecorder *createRecorder()
{
	return &ray::recorder::Recorder::getInstance();
}
