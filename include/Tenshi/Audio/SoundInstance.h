#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace Tenshi {

class Sound;

class SoundInstance {
public:
    SoundInstance();
    ~SoundInstance();

    SoundInstance(const SoundInstance&) = delete;
    SoundInstance& operator=(const SoundInstance&) = delete; 

    SoundInstance(SoundInstance&& other) noexcept;
    SoundInstance& operator=(SoundInstance&& other) noexcept;

    void Stop();
    void Pause();
    void Resume();

    bool IsPlaying() const;

    void SetVolume(float volume);
    float GetVolume() const;

    void SetPitch(float pitch);
    float GetPitch() const;

    void SetPan(float pan); 
    float GetPan() const;

    void SetLooping(bool looping);
    bool IsLooping() const;

private:
    friend class Sound;

    void Init(Sound& sound, float volume, float pitch, float pan, bool loop);
    void Destroy();

    std::shared_ptr<std::vector<uint8_t>> data;
    struct State;
    std::unique_ptr<State> state;
};

} // namespace Tenshi
