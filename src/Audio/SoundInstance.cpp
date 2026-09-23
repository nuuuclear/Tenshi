#include "Tenshi/Audio/SoundInstance.h"

#include "Tenshi/Audio/Audio.h"
#include "Tenshi/Audio/Sound.h"

namespace Tenshi {

struct SoundInstance::State {
    ma_decoder decoder{};
    ma_sound sound{};
    bool decoderInitialized = false;
    bool soundInitialized = false;

    ~State() {
        if (soundInitialized)
            ma_sound_uninit(&sound);
        if (decoderInitialized)
            ma_decoder_uninit(&decoder);
    }
};

SoundInstance::SoundInstance() {
}

SoundInstance::~SoundInstance() {
    Destroy();
}

SoundInstance::SoundInstance(SoundInstance&& other) noexcept {
    data = std::move(other.data);
    state = std::move(other.state);
}

SoundInstance& SoundInstance::operator=(SoundInstance&& other) noexcept {
    if (this == &other)
        return *this;

    Destroy();

    data = std::move(other.data);
    state = std::move(other.state);

    return *this;
}

void SoundInstance::Init(Sound& soundAsset, float volume, float pitch, float pan, bool loop) {
    Destroy();

    if (!soundAsset.audio || !soundAsset.data)
        return;

    ma_engine* engine = soundAsset.audio->GetEngine();
    if (!engine) return;

    auto newState = std::make_unique<State>();
    ma_decoder_config config = ma_decoder_config_init(ma_format_f32, 0, 0);
    ma_result result = ma_decoder_init_memory(
        soundAsset.data->data(),
        soundAsset.data->size(),
        &config,
        &newState->decoder
    );

    if (result != MA_SUCCESS)
        return;

    newState->decoderInitialized = true;
    result = ma_sound_init_from_data_source(
        engine,
        &newState->decoder,
        MA_SOUND_FLAG_NO_SPATIALIZATION,
        nullptr,
        &newState->sound
    );

    if (result != MA_SUCCESS) {
        return;
    }

    data = soundAsset.data;
    newState->soundInitialized = true;
    state = std::move(newState);

    SetVolume(volume);
    SetPitch(pitch);
    SetPan(pan);
    SetLooping(loop);

    ma_sound_start(&state->sound);
}

void SoundInstance::Destroy() {
    state.reset();
    data.reset();
}

void SoundInstance::Stop() {
    if (!state) return;

    ma_sound_stop(&state->sound);
}

void SoundInstance::Pause() {
    if (!state) return;

    ma_sound_stop(&state->sound);
}

void SoundInstance::Resume() {
    if (!state) return;

    ma_sound_start(&state->sound);
}

bool SoundInstance::IsPlaying() const {
    if (!state) return false;

    return ma_sound_is_playing(&state->sound) == MA_TRUE;
}

void SoundInstance::SetVolume(float volume) {
    if (!state) return;

    ma_sound_set_volume(&state->sound, volume);
}

float SoundInstance::GetVolume() const {
    if (!state) return 0.0f;

    return ma_sound_get_volume(&state->sound);
}

void SoundInstance::SetPitch(float pitch) {
    if (!state) return;

    ma_sound_set_pitch(&state->sound, pitch);
}

float SoundInstance::GetPitch() const {
    if (!state)
        return 1.0f;

    return ma_sound_get_pitch(&state->sound);
}

void SoundInstance::SetPan(float pan) {
    if (!state) return;

    ma_sound_set_pan(&state->sound, pan);
}

float SoundInstance::GetPan() const {
    if (!state) return 0.0f;

    return ma_sound_get_pan(&state->sound);
}

void SoundInstance::SetLooping(bool looping) {
    if (!state) return;

    ma_sound_set_looping(&state->sound, looping ? MA_TRUE : MA_FALSE);
}

bool SoundInstance::IsLooping() const {
    if (!state) return false;

    return ma_sound_is_looping(&state->sound) == MA_TRUE;
}

} // namespace Tenshi
