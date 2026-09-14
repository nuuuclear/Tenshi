#include "Tenshi/Sound.h"

#include "Tenshi/Audio.h"
#include "Tenshi/SoundInstance.h"

namespace Tenshi {

Sound::Sound() {
}

Sound::~Sound() {
    Unload();
}

bool Sound::Load(AudioSystem& AudioSystem, std::vector<uint8_t> inputData) {
    Unload();

    if (inputData.empty())
        return false;

    if (!AudioSystem.IsInitialized())
        return false;

    data = std::make_shared<std::vector<uint8_t>>(std::move(inputData));
    audio = &AudioSystem;
    loaded = true;

    return true;
}

void Sound::Unload() {
    data.reset();

    audio = nullptr;
    loaded = false;
}

bool Sound::IsLoaded() const {
    return loaded;
}

SoundInstance Sound::Play(float volume, float pitch, float pan, bool loop) {
    SoundInstance instance;

    if (!loaded || !audio || !data)
        return instance;

    instance.Init(*this, volume, pitch, pan, loop);
    
    return instance;
}

} // namespace Tenshi