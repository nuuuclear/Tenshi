#pragma once

#include <SDL3/SDL.h>
#include <miniaudio.h>

namespace Tenshi {

class Sound;

bool AudioInit();
bool AudioStart();
void AudioEnd();

// audio objects
Sound* MakeSound();
void DestroySound();

void PutSound(const char* path);

void PlaySound();
void StopSound();
void ResetSound();

void SoundSetFreq();
void SoundSetVol();
void SoundSetPan();
void SoundSetAutomation();

void SoundSetCallback();

}