#ifndef RAY_RECORDER_H
#define RAY_RECORDER_H

#include "base.h"

namespace ray {
	namespace recorder {

		typedef struct {
			rt_utf8 name[DEVICE_MAX_NAME_LEN];
			rt_utf8 id[DEVICE_MAX_ID_LEN];
		}VideoDevice;

		typedef struct {
			rt_utf8 name[DEVICE_MAX_NAME_LEN];
			rt_utf8 id[DEVICE_MAX_ID_LEN];
		}AudioDevice;

		typedef struct {
			
		}VideoEncoderParameter;

		typedef struct {
			
		}AudioEncoderParameter;

		/**
		*
		*/
		class IVideoDeviceCollection {
		protected:
			virtual ~IVideoDeviceCollection() {};

			virtual void release() = 0;

			virtual void getCount() = 0;
		};

		class IVideoDeviceManager {
		protected:
			virtual ~IVideoDeviceManager() {};
		};

		class IVideoFrame {
		protected:
			virtual ~IVideoFrame() {};

		public:

			enum VIDEO_FRAME_TYPE {
				VIDEO_FRAME_BGRA,
				VIDEO_FRAME_YUV420,
				VIDEO_FRAME_YUV444
			};

			virtual rt_uid getUID() const = 0;

			virtual const uint8_t *getData() const = 0;

			virtual uint32_t getDataSize() const = 0;

			virtual const base::CRect& getSize() const = 0;
		};

		class IVideoCapturer {
		protected:
			virtual ~IVideoCapturer() {};
		};

		class IVideoCapturerManager {
		protected:
			virtual ~IVideoCapturerManager() {};

		public:
			virtual rt_error createVideoCapturer(VIDEO_CAPTURER_TYPE type, IVideoCapturer **capturer, rt_uid *uid) = 0;

			virtual void deleteVideoCapturer(IVideoCapturer *capturer) = 0;
		};

		class IVideoEncoderCollection {
		protected:
			virtual ~IVideoEncoderCollection() {};
		};

		class IVideoEncoderManager {
		protected:
			virtual ~IVideoEncoderManager() {};
		};

		class IAudioDeviceCollection {
		protected:
			virtual ~IAudioDeviceCollection() {};
		};

		class IAudioDeviceManager {
		protected:
			virtual ~IAudioDeviceManager() {};
		};

		class IAudioFrame {
		protected:
			virtual ~IAudioFrame() {};

		public:

			virtual rt_uid getUID() const = 0;

			virtual const uint8_t *getData() const = 0;

			virtual uint32_t getDataSize() const = 0;
		};

		class IAudioCapturer {
		protected:
			virtual ~IAudioCapturer() {};

		public:
			virtual void release() = 0;

			virtual IAudioDeviceManager *getManager() = 0;
		};

		class IAudioCapturerManager {
		protected:
			virtual ~IAudioCapturerManager() {};

		public:
			virtual rt_error createAudioCapturer(AUDIO_CAPTURER_TYPE type, IAudioCapturer **capturer, rt_uid *uid) = 0;

			virtual void deleteAudioCapturer(IAudioCapturer *capturer) = 0;
		};

		class IAudioEncoderCollection {
		protected:
			virtual ~IAudioEncoderCollection() {};
		};

		class IAudioEncoderManager {
		protected:
			virtual ~IAudioEncoderManager() {};
		};

		class IMuxer {
		protected:
			virtual ~IMuxer() {};

		public:
			virtual void release() = 0;

			virtual bool isMuxing() = 0;

			virtual rt_error start(const rt_utf8 outputFileName[RECORDER_MAX_PATH_LEN]) = 0;

			virtual void stop() = 0;

			virtual rt_error pause() = 0;

			virtual rt_error resume() = 0;

			virtual rt_error setVideoEncoderParameters(VideoEncoderParameter *param) = 0;

			virtual rt_error setAudioEncoderParameters(AudioEncoderParameter *param) = 0;
		};

		class IRemuxer {
		protected:
			virtual ~IRemuxer() {};

		public:
			virtual rt_error remux(
				const rt_utf8 srcFilePath[RECORDER_MAX_PATH_LEN],
				const rt_utf8 dstFilePath[RECORDER_MAX_PATH_LEN]
			) = 0;
		};

		/**
		*
		*/
		class IRecorderEventHandler {
		public:
			virtual ~IRecorderEventHandler() {};

			virtual void onDuration(uint64_t duration) = 0;

			virtual void onError(ERROR_CODE code) = 0;

			virtual void onDeviceChanged(bool isAudio) = 0;

			virtual void onRawVideoData(IVideoFrame &frame) = 0;

			virtual void onRawAudioData(IAudioFrame &frame) = 0;

			virtual void onAudioVolume(const rt_uid uid, int volume) = 0;

			virtual void onRemuxProgress(const rt_utf8 *src, uint8_t progress, uint8_t total) = 0;

		};

		/**
		* Define all interfaces of Recorder
		*/
		class IRecorder {
		protected:
			virtual ~IRecorder() {};

		public:
			virtual rt_error initialize() = 0;

			virtual void release() = 0;

			virtual void getVersion(uint32_t *major, uint32_t *minor, uint32_t *patch, uint32_t *build) = 0;

			virtual void setLogPath(const rt_utf8 logPath[RECORDER_MAX_PATH_LEN]) = 0;

			virtual void setEventHandler(const IRecorderEventHandler *handler) = 0;

			virtual void queryInterface(RECORDER_INTERFACE_IID iid, void **interface) = 0;
		};
	}
}


RAY_API ray::recorder::IRecorder* RAY_CALL createRecorder();

#endif // !RAY_RECORDER_H