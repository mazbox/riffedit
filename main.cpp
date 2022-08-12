#include <stdio.h>
#include <vector>
#include <math.h>
#include <iostream>
#include <fstream>

#define DR_WAV_IMPLEMENTATION
#include "dr_wav.h"


size_t myWrite(void* pUserData, const void* pData, size_t bytesToWrite) {
	auto *myfile = (std::ofstream*)pUserData;
	myfile->write((const char*)pData, bytesToWrite);
	if(!myfile->bad()) return bytesToWrite;
	return 0;
}

drwav_bool32 mySeek(void* pUserData, int offset, drwav_seek_origin origin) {
	auto *myfile = (std::ofstream*)pUserData;
	auto dir = origin==drwav_seek_origin_start ? std::ios_base::beg : std::ios_base::cur;
	myfile->seekp(offset, dir);
	return !myfile->bad();
}




int writeMetadataFile() {
	drwav wav;
	
	const drwav_allocation_callbacks *pAllocationCallbacks = nullptr;
	drwav_uint32 metadataCount = 1;
	drwav_metadata pMetadata[1];

	std::string label = "onetwothree"; // needs to be kept alive whilst this runs
	pMetadata[0].type = drwav_metadata_type_list_note;
	pMetadata[0].data.labelOrNote.stringLength = label.size();
	pMetadata[0].data.labelOrNote.pString = (char*)label.c_str();


	drwav_data_format pFormat;
    pFormat.container = drwav_container_riff;     // <-- drwav_container_riff = normal WAV files, drwav_container_w64 = Sony Wave64.
    pFormat.format = DR_WAVE_FORMAT_PCM;          // <-- Any of the DR_WAVE_FORMAT_* codes.
    pFormat.channels = 1;
    pFormat.sampleRate = 44100;
    pFormat.bitsPerSample = 16;


	std::ofstream myfile;
 	myfile.open ("test.wav", std::ios::out | std::ios::binary);


 	if (!myfile.is_open()) {
 		printf("failed to open file\n");
 		return 1;
 	}

	bool success = drwav_init_write_with_metadata(&wav, 
		&pFormat, 
		&myWrite, 
		&mySeek, 
		(void*)&myfile, //void* pUserData, 
		pAllocationCallbacks, 
		pMetadata, 
		metadataCount);
	
	if(!success) {
		printf("failed to init write\n");
		drwav_uninit(&wav);
		return 1;
	}

	// no metadata file is 88244 bytes
	
	int numFrames = 44100;
	std::vector<int16_t> data(numFrames, 0);
	for(int i = 0; i < numFrames; i++) {
		data[i] = sin(i*0.04) * 32764.f * 0.3f;
	}

	int read = drwav_write_pcm_frames(&wav, numFrames, (void*)data.data());

	if(read!=numFrames) {
		printf("failed to write audio\n");
		drwav_uninit(&wav);
		return 1;
	}
// printf("success\n");

	drwav_uninit(&wav);
	return 0;
}


size_t myRead(void* pUserData, void* pBufferOut, size_t bytesToRead) {
	// printf("try read\n");
	auto *myfile = (std::ifstream*)pUserData;
	myfile->read((char*)pBufferOut, bytesToRead);
	// if(!myfile->bad()) 
		return bytesToRead;
	// return 0;
}

drwav_bool32 myReadSeek(void* pUserData, int offset, drwav_seek_origin origin) {
	// printf("try seek %d\n", offset);
	auto *myfile = (std::ifstream*)pUserData;
	auto dir = origin==drwav_seek_origin_start ? std::ios_base::beg : std::ios_base::cur;
	myfile->seekg(offset, dir);
	return true;//!myfile->bad();
}

#include "RIFF.h"



bool readFile(std::string filename, std::vector<uint8_t> &outData) {
	
	std::ifstream strm(filename, std::ios_base::binary);
	if (!strm) {
		printf("cannot open file\n");
		return false;
	}
	
	strm.unsetf(std::ios_base::skipws);
	std::istream_iterator<unsigned char> isi(strm), isiEOF;
	outData.assign(isi, isiEOF);
	if (!strm.eof()) {
		printf("read error\n");
		return false;
	}
	return true;
}


int readMetadataFile() {


	std::vector<uint8_t> data;
	if(!readFile("test.wav", data)) {
		printf("error opening file for reading\n");
		return 1;
	}


	Riff riff(data);

	// drwav wav;

	// if(!drwav_init_with_metadata(&wav, 
	// 					&myRead, 
	// 					&myReadSeek, 
	// 					(void*) &myfile, 
	// 					0, // no flags
	// 					nullptr)) {
	// 	printf("Failed to init for reading\n");
	// 	return 1;
	// }
	return 0;
}



int main() {
	int res = writeMetadataFile();	
	if(res!=0) return res;
	res = readMetadataFile();
	return res;
}