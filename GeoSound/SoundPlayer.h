#pragma once

#include "AudioManager.h"

ref class ISound;

private ref class SoundPlayer sealed
{
private:
	std::unique_ptr<AudioManager> audioManager_;
	Platform::Collections::Map<Platform::String^, int> namedSounds_;

public:
	SoundPlayer(void);
	void AddNamedSound(Platform::String^ name, int frequency);
	void PlayNamedSound(Platform::String^ name);
	void PlayCustomSound(Platform::String^ name, ISound^ sound);
	void PlaySineWave(int frequency);
	void StopPlayingSound(Platform::String^ name);
	void StopPlayingAllSounds();
	Windows::Foundation::Collections::IVector<int16>^ GetAudioGraph();
};
