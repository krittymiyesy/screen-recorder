#ifndef RECORDER_EXPORT
#define RECORDER_EXPORT

#include <stdint.h>


#ifdef AMRECORDER_IMPORT
#define AMRECORDER_API extern "C"  __declspec(dllimport)
#else
#define AMRECORDER_API extern "C"  __declspec(dllexport)
#endif


/**
* AMRECORDER_DEVICE
*/
#pragma pack(push,1)
typedef struct {
	/**
	* Device id in utf8
	*/
	char id[260];

	/**
	* Device name in utf8
	*/
	char name[260];

	/**
	* Is default device
	*/
	uint8_t is_default;
}AMRECORDER_DEVICE;
#pragma pack(pop)

/**
* AMRECORDER_SETTING
*/
#pragma pack(push,1)
typedef struct {

	/**
	* Left of desktop area
	*/
	int v_left;

	/**
	* Top of desktop area
	*/
	int v_top;

	/**
	* Width of desktop area
	*/
	int v_width;

	/**
	* Height of desktop area
	*/
	int v_height;

	/**
	* Output video quality, value must be between 0 and 100, 0 is least, 100 is best
	*/
	int v_qb;

	/**
	* Output video bitrate, the larger value you set,
	* the better video quality you get, but the file size is also larger.
	* Suggestion: 960|1280|2500 *1000
	*/
	int v_bit_rate;

	/**
	* FPS(frame per second)
	*/
	int v_frame_rate;

	/**
	* Video encoder id
	* Must get by recorder_get_vencoders
	*/
	int v_enc_id;

	/**
	* Output file path,the output file format is depended on the ext name.
	* Support .mp4|.mkv for now.
	*/
	char output[260];

	/**
	* Desktop device
	* Unused
	*/
	AMRECORDER_DEVICE v_device;

	/**
	* Microphone device info
	*/
	AMRECORDER_DEVICE a_mic;

	/**
	* Speaker device info
	*/
	AMRECORDER_DEVICE a_speaker;
}AMRECORDER_SETTING;
#pragma pack(pop)

/**
* AMRECORDER_ENCODERS
*/
#pragma pack(push,1)
typedef struct {

	/**
	* Encoder id
	*/
	int id;

	/**
	* Encoder name
	*/
	char name[260];
}AMRECORDER_ENCODERS;
#pragma pack(pop)

/**
* Recording duration callback function
* @param[in] duration time in millisecond
*/
typedef void(*AMRECORDER_FUNC_DURATION)(uint64_t duration);

/**
* Recording error callback function
* Should call recorder_err2str to get stringify error info
* @param[in] error
*/
typedef void(*AMRECORDER_FUNC_ERROR)(int error);

/**
* Device changed callback function
* Should refresh devices
* @param[in] type 0 for video, 1 for speaker, 2 for microphone
*/
typedef void(*AMRECORDER_FUNC_DEVICE_CHANGE)(int type);

/**
* YUV data callback function
* Should refresh devices
* @param[in] data   yuv buffer
* @param[in] size   yuv buffer size
* @param[in] width  picture with
* @param[in] height picture height
* @param[in] type   yuv type, 0 for 420, 1 fro 444
*/
typedef void(*AMRECORDER_FUNC_PREVIEW_YUV)(
	const unsigned char *data,
	unsigned int size,
	int width,
	int height,
	int type
	);

/**
* Unused callback function
*/
typedef void(*AMRECORDER_FUNC_PREVIEW_AUDIO)();

/**
* Remux progress callback function
* @param[in] path       source file path
* @param[in] progress   remuxing progress in total
* @param[in] total      always will be 100
*/
typedef void(*AMRECORDER_FUNC_REMUX_PROGRESS)(const char *path, int progress, int total);

/**
* Remux state callback function
* @param[in] path    source file path
* @param[in] state   0 for unremuxing,1 for remuxing
* @param[in] error   0 for succed,otherwhise error code
*/
typedef void(*AMRECORDER_FUNC_REMUX_STATE)(const char *path, int state, int error);

/**
* Callback functions structure
*/
#pragma pack(push,1)
typedef struct {
	AMRECORDER_FUNC_DURATION func_duration;
	AMRECORDER_FUNC_ERROR func_error;
	AMRECORDER_FUNC_DEVICE_CHANGE func_device_change;
	AMRECORDER_FUNC_PREVIEW_YUV func_preview_yuv;
	AMRECORDER_FUNC_PREVIEW_AUDIO func_preview_audio;
}AMRECORDER_CALLBACK;
#pragma pack(pop)

/**
* Get error string by specified error code
* @return error string
*/
AMRECORDER_API const char * recorder_err2str(int error);

/**
* Initialize recorder with specified seetings¡¢speaker¡¢mic¡¢encoder...
* @return 0 if succeed,error code otherwise
*/
AMRECORDER_API int recorder_init(const AMRECORDER_SETTING &setting, const AMRECORDER_CALLBACK &callbacks);

/**
* Release all recorder resources
*/
AMRECORDER_API void recorder_release();

/**
* Start recording
* @return 0 if succeed,error code otherwise
*/
AMRECORDER_API int recorder_start();

/**
* Stop recording
*/
AMRECORDER_API void recorder_stop();

/**
* Pause recording
* @return 0 if succeed,error code otherwise
*/
AMRECORDER_API int recorder_pause();

/**
* Resume recording
* @return 0 if succeed,error code otherwise
*/
AMRECORDER_API int recorder_resume();

/**
* Get valid speaker devices
* @param[in] devices a pointer to a device array,should call recorder_free_array to free memory
* @return count of speakers
*/
AMRECORDER_API int recorder_get_speakers(AMRECORDER_DEVICE **devices);

/**
* Get valid mic devices
* @param[in] devices a pointer to a device array,should call recorder_free_array to free memory
* @return count of mics
*/
AMRECORDER_API int recorder_get_mics(AMRECORDER_DEVICE **devices);

/**
* Get valid camera devices
* @param[in] devices a pointer to a device array,should call recorder_free_array to free memory
* @return count of cameras
*/
AMRECORDER_API int recorder_get_cameras(AMRECORDER_DEVICE **devices);

/**
* Get valid encoders
* @param[in] encoders a pointer to a encoder array,should call recorder_free_array to free memory
* @return count of encoders
*/
AMRECORDER_API int recorder_get_vencoders(AMRECORDER_ENCODERS **encoders);

/**
* Free memory allocate by recorder
* @param[in] array_address the pointer of array buffer
*/
AMRECORDER_API void recorder_free_array(void *array_address);

/**
* Recorder create a remux job
* @param[in] src    source file path
* @param[in] dst   0 for unremuxing,1 for remuxing
* @param[in] func_progress   0 for succed,otherwhise error code
* @param[in] func_state   0 for succed,otherwhise error code
*/
AMRECORDER_API int recorder_remux(
	const char *src, const char *dst,
	AMRECORDER_FUNC_REMUX_PROGRESS func_progress,
	AMRECORDER_FUNC_REMUX_STATE func_state);

/**
* Enable or disable preview include video and audio
* @param[in] enable 1 for enable,0 for disable
*/
AMRECORDER_API void recorder_set_preview_enabled(int enable);


#if defined(_WIN32)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define RAY_CALL __cdecl
#if defined(RAY_EXPORT)
#define RAY_API extern "C" __declspec(dllexport)
#else
#define RAY_API extern "C" __declspec(dllimport)
#endif
#elif defined(__APPLE__)
#include <TargetConditionals.h>
#define RAY_API __attribute__((visibility("default"))) extern "C"
#define RAY_CALL
#elif defined(__ANDROID__) || defined(__linux__)
#define RAY_API extern "C" __attribute__((visibility("default")))
#define RAY_CALL
#else
#define RAY_API extern "C"
#define RAY_CALL
#endif

#define DEVICE_MAX_NAME_LEN 260
#define DEVICE_MAX_ID_LEN DEVICE_MAX_NAME_LEN

#define ENCODER_MAX_NAME_LEN 260
#define ENCODER_MAX_ID_LEN ENCODER_MAX_NAME_LEN

#define RECORDER_MAX_PATH_LEN 260

namespace ray {
	namespace base {

		typedef char utf8_char;

		typedef enum {
			VIDEO_CAPTURER_UNKNOWN = 0,

			/**
			* whole monitor video capturer
			*/
			VIDEO_CAPTURER_MONITOR,

			/**
			* rect video capturer
			*/
			VIDEO_CAPTURER_RECT,

			/**
			* window video capturer
			*/
			VIDEO_CAPTURER_WINDOW,

			/**
			* game video capturer, means dx or gl hook capture
			*/
			VIDEO_CAPTURER_GAME,

			/**
			* camera video capturer
			*/
			VIDEO_CAPTURER_CAMERA,

			/**
			* source video capturer, like text what you want to draw topmost or picture and video files
			*/
			VIDEO_CAPTURER_SOURCE,
		}VIDEO_CAPTURER_TYPE;

		typedef enum {
			AUDIO_CAPTURER_UNKNOWN = 0,

			/**
			* speaker audio capturer
			*/
			AUDIO_CAPTURER_SPEAKER,

			/**
			* microphone audio capturer
			*/
			AUDIO_CAPTURER_MICROPHONE,

			/**
			* app audio capturer
			*/
			AUDIO_CAPTURER_APP
		}AUDIO_CAPTURER_TYPE;

		typedef enum {
			RECORDER_IID_UNKNOWN = 0,

			RECORDER_IID_VIDEO_DEVICE_MGR,
			RECORDER_IID_VIDEO_CAPTURER_MGR,
			RECORDER_IID_VIDEO_ENCODER_MGR,

			RECORDER_IID_AUDIO_DEVICE_MGR,
			RECORDER_IID_AUDIO_CAPTURER_MGR,
			RECORDER_IID_AUDIO_ENCODER_MGR,

			RECORDER_IID_MUXER_MGR,
			RECORDER_IID_REMUXER_MGR
		}RECORDER_INTERFACE_IID;

		typedef struct {
			utf8_char name[DEVICE_MAX_NAME_LEN];
			utf8_char id[DEVICE_MAX_ID_LEN];
		}VideoDevice;

		typedef struct {
			utf8_char name[DEVICE_MAX_NAME_LEN];
			utf8_char id[DEVICE_MAX_ID_LEN];
		}AudioDevice;

		typedef struct {
			utf8_char name[DEVICE_MAX_NAME_LEN];
			utf8_char id[DEVICE_MAX_ID_LEN];
		}VideoEncoderParameter;

		typedef struct {
			utf8_char name[DEVICE_MAX_NAME_LEN];
			utf8_char id[DEVICE_MAX_ID_LEN];
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

		class IVideoCapturer {
		protected:
			virtual ~IVideoCapturer() {};
		};

		class IVideoCapturerManager {
		protected:
			virtual ~IVideoCapturerManager() {};

		public:
			virtual IVideoCapturer *createVideoCapturer() = 0;

			virtual uint32_t addVideoCapturer(IVideoCapturer *capturer) = 0;

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
			virtual IAudioCapturer *createAudioCapturer() = 0;

			virtual uint32_t addAudioCapturer(IAudioCapturer *capturer) = 0;

			virtual void deleteAudioCapturer(IAudioCapturer *capturer) = 0
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

			virtual uint32_t start(const utf8_char outputFileName[RECORDER_MAX_PATH_LEN]) = 0;

			virtual void stop() = 0;

			virtual uint32_t pause() = 0;

			virtual uint32_t resume() = 0;

			virtual uint32_t setVideoEncoderParameters(VideoEncoderParameter *param) = 0;

			virtual uint32_t setAudioEncoderParameters(AudioEncoderParameter *param) = 0;
		};

		class IRemuxer {
		protected:
			virtual ~IRemuxer() {};

		public:
			virtual uint32_t remux(const utf8_char srcFilePath[RECORDER_MAX_PATH_LEN], const utf8_char dstFilePath[RECORDER_MAX_PATH_LEN]) = 0;
		};

		/**
		*
		*/
		class IScreenRecorderEventHandler {
		public:
			virtual ~IScreenRecorderEventHandler() {};


		};

		/**
		* Define all interfaces of ScreenRecorder
		*/
		class IScreenRecorder {
		protected:
			virtual ~IScreenRecorder() {};

		public:
			virtual uint32_t initialize() = 0;

			virtual void release() = 0;

			virtual void setLogPath(const utf8_char logPath[RECORDER_MAX_PATH_LEN]) = 0;

			virtual void setEventHandler(const IScreenRecorderEventHandler *handler) = 0;

			virtual void queryInterface(RECORDER_INTERFACE_IID iid, void **interface) = 0;
		};
	}
}


RAY_API ray::base::IScreenRecorder* RAY_CALL createScreenRecorder();

#endif