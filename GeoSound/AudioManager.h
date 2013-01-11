#pragma once
#include "IndividualSound.h"

class IOscillator;

class AudioManager
{
public:
	AudioManager(void);
	virtual ~AudioManager();

	void Initialize(void);

	void AddSound(std::unique_ptr<IOscillator> sound);
	void RemoveSound(std::unique_ptr<IOscillator> sound);
	void ClearSounds();

	std::vector<int16> GetAudioGraph();

private:
	Microsoft::WRL::ComPtr<IXAudio2> XAudio2;
	IXAudio2MasteringVoice* pMasterVoice;

	std::vector<std::unique_ptr<IOscillator>> oscillators;
	std::vector<std::unique_ptr<IndividualSound>> sounds;

	void UpdateMixerState(void);
	void PlayCompositeSound(std::vector<int16_t>& sound);
};
