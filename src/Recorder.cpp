#include "recorder.h"

#include "constants\version.h"

#include "utils\strings.h"
#include "utils\log.h"

namespace ray {
	namespace recorder {

		rt_error Recorder::initialize(const rt_utf8 logPath[RECORDER_MAX_PATH_LEN]) {

			utils::InitLogImpl(utils::strings::utf8_unicode(logPath).c_str());

			return ERR_NONE;
		}

		void Recorder::release() {

		}

		void Recorder::getVersion(uint32_t *major, uint32_t *minor, uint32_t *patch, uint32_t *build) {

			if (major) *major = VER_MAJOR;

			if (minor) *minor = VER_MINOR;

			if (patch) *patch = VER_PATCH;

			if (build) *build = VER_BUILD;
		}

		void Recorder::setEventHandler(const IRecorderEventHandler *handler) {

		}

		void Recorder::queryInterface(RECORDER_INTERFACE_IID iid, void **interface) {
			switch (iid)
			{
			default:
				break;
			}
		}

	}
}

RAY_API ray::recorder::IRecorder *createRecorder()
{
	return &ray::recorder::Recorder::getInstance();
}
