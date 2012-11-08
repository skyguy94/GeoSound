#include "pch.h"
#include "ManagedSineWaveOscillator.h"

using namespace Platform::Collections;
using namespace Windows::Foundation::Collections;

const double PI  = std::atan(1.0) * 4;

ManagedSineWaveOscillator::ManagedSineWaveOscillator(int32 frequency, int32 amplitude)
	: frequency_(frequency),
	  amplitude_(amplitude)
{
}

IVector<int16>^ ManagedSineWaveOscillator::GenerateSample(float32 duration, int32 sampleRate)
{
	int32_t numSamples = duration * sampleRate;
	double angle = 2 * PI * frequency_ / sampleRate;

	std::vector<int16_t> data;
	for (int i = 0; i < numSamples; i++)
	{
		data.push_back(amplitude_ * sin(angle * i));
	}

	return ref new Vector<int16>(data);
}
