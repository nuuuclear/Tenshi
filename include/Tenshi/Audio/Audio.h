#pragma once

#include <miniaudio.h>

namespace Tenshi {

class AudioSystem {
public:
    AudioSystem();
    ~AudioSystem();

    AudioSystem(const AudioSystem&) = delete;
    AudioSystem& operator=(const AudioSystem&) = delete;

    bool Init();
    bool Start();
    void Shutdown();

    bool IsInitialized() const;

    ma_engine* GetEngine();

    void SetMasterVolume(float volume);
    float GetMasterVolume() const;

private:
    ma_engine engine{};
    
    bool initialized = false;
    bool started = false;
};

} // namespace Tenshi
