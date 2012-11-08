#include "pch.h"
#include "Oscillator.h"
#include "ManagedMixer.h"

using namespace Windows::Foundation::Collections;
using namespace Platform::Collections;
using std::unique_ptr;
using std::vector;

ManagedMixer::ManagedMixer(void)
	: BufferDurationSeconds(1)
{}

IVector<int16>^ ManagedMixer::MixToStream(IVector<IOscillator^>^ oscillators, int32 sampleRate)
{
	int32 numSamples = BufferDurationSeconds * sampleRate;
	data.resize(numSamples);

	for(auto current : oscillators)
	{
		auto sound = current->GenerateSample(BufferDurationSeconds, sampleRate);
		std::transform(begin(sound), end(sound), begin(data), begin(data), std::plus<int16>());
	}

	return ref new Vector<int16>(data);
}
