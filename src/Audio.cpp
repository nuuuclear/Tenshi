#include <stdio.h>

#include "Audio.h"
#include "Log.h"

#include "Sound.h"

namespace Tenshi {

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount) {
	ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
	if (pDecoder == NULL) {
		return;
	}

	ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

	(void)pInput;
}

ma_result result;
ma_decoder decoder;
ma_device_config deviceConfig;
ma_device device;

bool AudioInit() {
	deviceConfig = ma_device_config_init(ma_device_type_playback);
	deviceConfig.playback.format = decoder.outputFormat;
	deviceConfig.playback.channels = decoder.outputChannels;
	deviceConfig.sampleRate = decoder.outputSampleRate;
	deviceConfig.dataCallback = data_callback;
	deviceConfig.pUserData = &decoder;

	if (ma_device_init(NULL, &deviceConfig, &device) != MA_SUCCESS) {
		printf("<Miniaudio> Failed to open playback device.\n");
		ma_decoder_uninit(&decoder);
		return false;
	}

	return true;
}

bool AudioStart() {
	if (ma_device_start(&device) != MA_SUCCESS) {
		printf("<Miniaudio> Failed to start playback device.\n");

		AudioEnd();
		return false;
	}

	return true;
}

void AudioEnd() {
	ma_device_uninit(&device);
	ma_decoder_uninit(&decoder);
	//SDL_CloseAudioDevice(device);
	//SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

// very simple for now
void PutSound(const char* path) {
	result = ma_decoder_init_file(path, NULL, &decoder);
	if (result != MA_SUCCESS) {
		LogWarning(
			LogCategory::Resource,
			"<Miniaudio> could not load file: {}",
			path
		);
	}
}

} // namespace Tenshi