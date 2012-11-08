#pragma once

class IndividualSound
{
public:
	Windows::Foundation::Collections::IVector<int16>^ Buffer;
	IXAudio2SourceVoice* Output;
};
