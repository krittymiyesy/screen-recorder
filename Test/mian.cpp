#define AMRECORDER_IMPORT
#include "../Recorder/export.h"

#include <Windows.h>
#include <stdlib.h>
#include <stdio.h>

void on_preview_image(
	const unsigned char *data,
	unsigned int size,
	int width,
	int height,
	int type) {
	//printf("on_preview_image size:%d type %d\r\n", size, type);
}

int main()
{
	AMRECORDER_DEVICE *speakers = NULL, *mics = NULL;
	AMRECORDER_ENCODERS *vencoders = NULL;

	AMRECORDER_SETTING setting = { 0 };
	AMRECORDER_CALLBACK callback = { 0 };


	int nspeaker = recorder_get_speakers(&speakers);

	int nmic = recorder_get_mics(&mics);

	int n_vencoders = recorder_get_vencoders(&vencoders);


	setting.v_left = 0;
	setting.v_top = 0;
	//setting.v_width = GetSystemMetrics(SM_CXSCREEN);
	//setting.v_height = GetSystemMetrics(SM_CYSCREEN);
	HDC hdc = GetDC(NULL);
	setting.v_width = GetDeviceCaps(hdc, DESKTOPHORZRES);
	setting.v_height = GetDeviceCaps(hdc, DESKTOPVERTRES);
	ReleaseDC(NULL, hdc);

	//setting.v_left = 100;
	//setting.v_top = 100;
	//setting.v_width = 1920;
	//setting.v_height = 1080;

	if (setting.v_width % 32 != 0) {
		setting.v_width = setting.v_width / 32 * 32;
	}
	if (setting.v_height % 2 != 0) {
		setting.v_height = setting.v_height / 2 * 2;
	}
	
	setting.v_qb = 100;
	setting.v_bit_rate = 1280 * 1000;
	//setting.v_bit_rate = 1280 * 720;
	setting.v_frame_rate = 30;

	//////////////should be the truely encoder id,zero will always be soft x264
	setting.v_enc_id = 0;

	sprintf(setting.output, "..\\..\\save.mp4");
	//sprintf(setting.output, "..\\..\\save.mkv");

#if 1 //record speaker
	for (int i = 0; i < nspeaker; i++) {
		if (speakers[i].is_default == 1)
			memcpy(&setting.a_speaker, &speakers[i], sizeof(AMRECORDER_DEVICE));
	}
#endif

#if 1 //record microphone
	for (int i = 0; i < nmic; i++) {
		if (mics[i].is_default == 1)
			memcpy(&setting.a_mic, &mics[i], sizeof(AMRECORDER_DEVICE));
	}
#endif

	callback.func_preview_yuv = on_preview_image;

	int err = recorder_init(setting, callback);

	err = recorder_start();



	//CAN NOT PAUSE FOR NOW
	/*getchar();
	recorder_pause();
	printf("recorder paused\r\n");
	getchar();
	recorder_resume();
	printf("recorder resumed\r\n");*/
	//input enter key,then recording stopped
	getchar();


	recorder_stop();

	recorder_release();

	recorder_free_array(speakers);
	recorder_free_array(mics);
	recorder_free_array(vencoders);

	printf("press any key to exit...\r\n");
	system("pause");

	return 0;
}