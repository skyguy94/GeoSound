#pragma once

class IndividualSound
{
public:
	std::vector<int16> Buffer;
	IXAudio2SourceVoice* Output;
};
