#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <miniaudio.h>

#include "Tenshi/Audio/SoundInstance.h"

namespace Tenshi {

class AudioSystem;

class Sound {
public:
    Sound();
    ~Sound();

    Sound(const Sound&) = delete;
    Sound& operator=(const Sound&) = delete;

    bool Load(AudioSystem& audio, std::vector<uint8_t> data);

    void Unload();
    bool IsLoaded() const;

    SoundInstance Play(
        float volume = 1.0f,
        float pitch = 1.0f,
        float pan = 0.0f,
        bool loop = false
    );

private:
    friend class SoundInstance;

    AudioSystem* audio = nullptr;
    std::shared_ptr<std::vector<uint8_t>> data;

    bool loaded = false;
};

} // namespace Tenshi