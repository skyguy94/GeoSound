#pragma once

class IOscillator
{
public:
	virtual std::vector<int16> GenerateSample(float32 duration, int32 sampleRate) = 0;
};
