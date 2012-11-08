#pragma once
#include "Oscillator.h"

ref class ManagedSineWaveOscillator sealed : public IOscillator
{
private:
	uint32_t frequency_;
	uint32_t amplitude_;

public:
	ManagedSineWaveOscillator(int32 frequency, int32 amplitude);

	virtual Windows::Foundation::Collections::IVector<int16>^ GenerateSample(float32 duration, int32 sampleRate);
};
