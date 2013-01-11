#include "pch.h"
#include "SoundPlayer.h"
#include "SineWaveOscillator.h"
#include "IOscillator.h"

using std::unique_ptr;

using namespace Platform;
using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;

SoundPlayer::SoundPlayer(void) : 
	audioManager_(new AudioManager())
{
	audioManager_->Initialize();
}

void SoundPlayer::AddNamedSound(Platform::String^ name, int frequency)
{
	namedSounds_.Insert(name, frequency);
}

void SoundPlayer::PlayNamedSound(Platform::String^ name)
{
	if (!namedSounds_.HasKey(name)) return;

	auto frequency = namedSounds_.Lookup(name);
	PlaySineWave(frequency);
	if (name == "A5")
	{
		auto frequency2 = namedSounds_.Lookup("C4");
		PlaySineWave(frequency2);
		
		auto frequency3 = namedSounds_.Lookup("G4");
		PlaySineWave(frequency3);
	}
}

void SoundPlayer::PlayCustomSound(String^ name, ISound^ sound)
{ }

void SoundPlayer::PlaySineWave(int frequency)
{
	auto oscillator = new SineWaveOscillator(frequency, 1000);
	audioManager_->AddSound(unique_ptr<IOscillator>(oscillator));
}

IVector<int16>^ SoundPlayer::GetAudioGraph()
{
	auto graph = audioManager_->GetAudioGraph();
	return ref new Vector<int16>(graph);
}

void SoundPlayer::StopPlayingAllSounds()
{
	audioManager_->ClearSounds();
}
