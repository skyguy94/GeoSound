#pragma once

#include "IOscillator.h"

class SineWaveOscillator sealed : public IOscillator
{
private:
	uint32_t frequency_;
	uint32_t amplitude_;

public:
	SineWaveOscillator(int32 frequency, int32 amplitude);

	virtual std::vector<short> GenerateSample(float32 duration, int32 sampleRate);
};
