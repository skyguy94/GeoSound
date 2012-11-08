#pragma once
#include "Oscillator.h"

ref class ManagedMixer sealed
{
private:
	const int BufferDurationSeconds;
	std::vector<int16> data;

public:
	ManagedMixer(void);

	Windows::Foundation::Collections::IVector<int16>^ MixToStream(Windows::Foundation::Collections::IVector<IOscillator^>^ oscillators, int32 sampleRate);
};
