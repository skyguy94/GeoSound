#pragma once

#include "Oscillator.h"
#include "IndividualSound.h"

ref class ManagedAudioManager sealed
{
public:
	ManagedAudioManager(void);
	virtual ~ManagedAudioManager();

	void Initialize(void);

	void AddSound(IOscillator ^sound);
	void RemoveSound(IOscillator ^sound);
	void ClearSounds();

	Windows::Foundation::Collections::IVector<int16>^ GetAudioGraph();

private:
	Microsoft::WRL::ComPtr<IXAudio2> XAudio2;
	IXAudio2MasteringVoice* pMasterVoice;

	Platform::Collections::Vector<IOscillator^> ^oscillators;
	std::vector<std::unique_ptr<IndividualSound>> sounds;

	void UpdateMixerState(void);
	void PlayCompositeSound(std::vector<int16_t>& sound);
};
