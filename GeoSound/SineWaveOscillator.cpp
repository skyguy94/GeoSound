#include "pch.h"
#include "SineWaveOscillator.h"

using std::vector;

const double PI  = std::atan(1.0) * 4;

SineWaveOscillator::SineWaveOscillator(int32 frequency, int32 amplitude)
	: frequency_(frequency),
	  amplitude_(amplitude)
{ }

std::vector<int16> SineWaveOscillator::GenerateSample(float32 duration, int32 sampleRate)
{
	int32_t numSamples = duration * sampleRate;
	double angle = 2 * PI * frequency_ / sampleRate;

	vector<int16> data;
	for (int i = 0; i < numSamples; i++)
	{
		data.push_back(amplitude_ * sin(angle * i));
	}

	return data;
}