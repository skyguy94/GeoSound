#pragma once

class IOscillator;

class Mixer 
{
private:
	const int BufferDurationSeconds;

public:
	Mixer(void);

	std::vector<int16> MixToStream(const std::vector<std::unique_ptr<IOscillator>>& oscillators, int32 sampleRate);
};
