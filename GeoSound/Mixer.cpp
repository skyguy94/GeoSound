#include "pch.h"
#include "Oscillator.h"
#include "Mixer.h"

using std::unique_ptr;
using std::vector;

using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;

Mixer::Mixer(void)
	: BufferDurationSeconds(1)
{}

std::vector<int16> Mixer::MixToStream(const std::vector<std::unique_ptr<IOscillator>>& oscillators, int32 sampleRate)
{
	int32 numSamples = BufferDurationSeconds * sampleRate;
	std::vector<int16> data(numSamples);

	for(auto& current : oscillators)
	{
		auto sound = current->GenerateSample(BufferDurationSeconds, sampleRate);
		std::transform(begin(sound), end(sound), begin(data), begin(data), std::plus<int16>());
	}

	return data;
}
