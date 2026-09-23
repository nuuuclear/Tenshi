#include "Tenshi/Audio/Audio.h"

namespace Tenshi {

AudioSystem::AudioSystem() {
}

AudioSystem::~AudioSystem() {
    Shutdown();
}

bool AudioSystem::Init() {
    if (initialized)
        return true;

    ma_engine_config config = ma_engine_config_init();
    
    // don't start device yet
    config.noAutoStart = MA_TRUE;
    ma_result result = ma_engine_init(&config, &engine);

    if (result != MA_SUCCESS)
        return false;

    initialized = true;

    return true;
}

bool AudioSystem::Start() {
    if (!initialized || started)
        return initialized;

    if (ma_engine_start(&engine) != MA_SUCCESS)
        return false;

    started = true;
    return true;
}

void AudioSystem::Shutdown() {
    if (!initialized)
        return;

    ma_engine_uninit(&engine);

    initialized = false;
    started = false;
}

bool AudioSystem::IsInitialized() const {
    return initialized;
}

ma_engine* AudioSystem::GetEngine() {
    if (!initialized)
        return nullptr;

    return &engine;
}

void AudioSystem::SetMasterVolume(float volume) {
    if (!initialized)
        return;

    ma_engine_set_volume(&engine, volume);
}

float AudioSystem::GetMasterVolume() const {
    if (!initialized)
        return 0.0f;

    return ma_engine_get_volume(const_cast<ma_engine*>(&engine));
}

} // namespace Tenshi
