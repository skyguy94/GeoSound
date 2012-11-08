#pragma once
class WAVGenerator
{
public:
	WAVGenerator(void);
	~WAVGenerator(void);
};

struct WaveHeader
{
	WaveHeader::WaveHeader()
		: sGroupId("RIFF"),
		dwFileLength(0),
		sRiffType("WAVE")
	{}

	std::string sGroupId;
	uint32_t dwFileLength;
	std::string sRiffType;
};

struct WaveFormatChunk
{
	std::string sChunkId;
	uint32_t dwChunkSize;
	unit16_6
}

struct WaveDataChunk
{
	std::string sChunkId;
	uint32_t dwChunkSize;
	int16_t shortArray;
};
