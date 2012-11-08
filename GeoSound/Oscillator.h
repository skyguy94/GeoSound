#pragma once

interface class IOscillator
{
public:
	Windows::Foundation::Collections::IVector<int16>^ GenerateSample(float32 duration, int32 sampleRate);
};
