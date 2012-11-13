#pragma once

#include "AudioManager.h"

ref class SoundPlayer sealed
{
private:
	std::unique_ptr<AudioManager> audioManager;

public:
	SoundPlayer(void);
	void PlayNamedSound(Platform::String^ name);
};

