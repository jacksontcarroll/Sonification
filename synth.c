#include <math.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

// This struct is the minimal required header data for a wav file
struct WaveFileHeader {
	// Master RIFF chunk to identify that WAV is being used
	unsigned char chunkID[4];
	uint32_t chunkSize;
	unsigned char waveID[4];

	// Information used to describe the format of the data
	unsigned char formatChunkID[4];
	uint32_t formatChunkSize; // The actual size of the format chunk header in bytes
	uint16_t formatCode;
	uint16_t formatNumChannels;
	uint32_t formatSampleRate;
	uint32_t formatBytesPerSec;
	uint16_t formatBlockAlign;
	uint16_t formatBitsPerSample;

	// "Data chunk" -- contains an identifier and the total number of samples
	unsigned char dataID[4];
	uint32_t dataSize;
};

bool WriteWaveFile(const char *szFileName, void *pData, int32_t nDataSize, int16_t nNumChannels, int32_t nSampleRate, int32_t nBitsPerSample) {
	// Open the file if we can
	FILE *File = fopen(szFileName,"w+");
	if(!File) { return false; }

	// Here is our actual struct defining the formatting information of the WAV file
	struct WaveFileHeader waveHeader;

	// Fill out the main chunk
	memcpy(waveHeader.chunkID, "RIFF", 4);
	waveHeader.chunkSize = nDataSize + 36;
	memcpy(waveHeader.waveID, "WAVE", 4);

	// Fill out sub chunk 1 "fmt "
	memcpy(waveHeader.formatChunkID, "fmt ", 4);
	waveHeader.formatChunkSize = 16;
	waveHeader.formatCode = 1; // 1 corresponds to PCM
	waveHeader.formatNumChannels = nNumChannels;
	waveHeader.formatSampleRate = nSampleRate;
	waveHeader.formatBytesPerSec = nSampleRate * nNumChannels * nBitsPerSample / 8;
	waveHeader.formatBlockAlign = nNumChannels * nBitsPerSample / 8; // Data block size (in bytes)
	waveHeader.formatBitsPerSample = nBitsPerSample;

	// Fill out sub chunk 2 "data"
	memcpy(waveHeader.dataID,"data",4);
	waveHeader.dataSize = nDataSize;

	// Write the header
	fwrite(&waveHeader, sizeof(struct WaveFileHeader), 1, File);

	// Write the wave data itself
	fwrite(pData, nDataSize, 1, File);

	// Close the file and return success
	fclose(File);

	return true;
}

int main() {
	int nSampleRate = 44100;
	int nNumSeconds = 4;
	int nNumChannels = 1;
	int nNumSamples = nSampleRate * nNumChannels * nNumSeconds;
	int freq = 100;

	int32_t *pData = (int32_t *) malloc(sizeof(int32_t) * nNumSamples);

	for (int i = 0; i < nNumSamples; i++) {
		pData[i] = INT_MAX * sin((freq * 2 * M_PI) * i / (float)nSampleRate);
	}

	WriteWaveFile("outmono.wav",pData,nNumSamples * sizeof(pData[0]),nNumChannels,nSampleRate,sizeof(pData[0])*8);
	free(pData);

	return 0;
}
