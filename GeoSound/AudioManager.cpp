#include "pch.h"
#include "Oscillator.h"
#include "AudioManager.h"
#include "IndividualSound.h"
#include "Mixer.h"

using std::unique_ptr;
using std::vector;

using namespace Platform;
using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;

AudioManager::AudioManager(void)
	: pMasterVoice(nullptr)
{
}

AudioManager::~AudioManager()
{
	if (pMasterVoice != nullptr)
	{
		pMasterVoice->DestroyVoice();
		pMasterVoice = nullptr;
	}
}

void AudioManager::Initialize(void)
{
	HRESULT hr = XAudio2Create(&XAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	if (FAILED(hr)) throw Platform::Exception::CreateException(hr);

	hr = XAudio2->CreateMasteringVoice(&pMasterVoice);
	if (FAILED(hr)) throw Platform::Exception::CreateException(hr);
}

void AudioManager::AddSound(unique_ptr<IOscillator> oscillator)
{
	auto sound(unique_ptr<IndividualSound>(new IndividualSound()));
	
	WAVEFORMATEX wfx;
	ZeroMemory(&wfx, sizeof(WAVEFORMATEX));
	wfx.cbSize = 0;
	wfx.wBitsPerSample = 16;
	wfx.nSamplesPerSec = 44100;
	wfx.nChannels = 1;
	wfx.nBlockAlign = 2;
	wfx.nAvgBytesPerSec = 44100 * 2;
	wfx.wFormatTag = WAVE_FORMAT_PCM;

	HRESULT hr = XAudio2->CreateSourceVoice(&sound->Output, &wfx);
	if (FAILED(hr)) throw Platform::Exception::CreateException(hr);

	auto sample = oscillator->GenerateSample(1.0f, 44100);

	XAUDIO2_BUFFER buffer;
	ZeroMemory(&buffer, sizeof(XAUDIO2_BUFFER));
	buffer.LoopCount = XAUDIO2_MAX_LOOP_COUNT;
	buffer.AudioBytes = sound->Buffer.size() * 2;

	buffer.pAudioData = (BYTE *)&sound->Buffer[0];

	hr = sound->Output->SubmitSourceBuffer(&buffer);
	if (FAILED(hr)) throw Platform::Exception::CreateException(hr);

	sounds.push_back(move(sound));
	oscillators.push_back(oscillator);

	std::for_each(sounds.begin(), sounds.end(), [](unique_ptr<IndividualSound>& i){ i->Output->Start(); });
}

void AudioManager::RemoveSound(std::unique_ptr<IOscillator> sound)
{
}

void AudioManager::ClearSounds()
{
	std::for_each(sounds.begin(), sounds.end(), [](unique_ptr<IndividualSound>& i){ i->Output->Stop(); });

	oscillators.clear();
	sounds.clear();
}

std::vector<int16> AudioManager::GetAudioGraph()
{
	Mixer mixer;
	auto graph(mixer.MixToStream(oscillators, 44100));
	return graph;
}
