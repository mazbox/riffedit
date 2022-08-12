#include "Riff.h"
#include <fstream>
// #include <stdio.h>
// #include <vector>
// #include <iostream>

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

bool writeFile(const std::string &path, const std::vector<uint8_t> &data) {
	std::ofstream outfile(path, std::ios::out | std::ios::binary);
	if(outfile.fail()) return false;
	outfile.write((const char*)data.data(), data.size());
	if(outfile.fail()) return false;
	outfile.close();
	if(outfile.fail()) return false;
	return true;
}




int main(int argc, char *argv[]) {

	if(argc < 2 ||  argc>4) {
		printf("\n\nUsage: \t%s file.wav [4 char code id] [value]\n\n", argv[0]);
		return 1;
	}

	std::vector<uint8_t> data;
	if(!readFile(argv[1], data)) {
		return 1;
	}

	Riff riff(data);
	if(argc==2) {
		riff.print();
	} else if(argc==3) {
		// get chunk
		auto info = riff.findInfoChunk();
		if(info==nullptr) {
			printf("Error! no info chunk!\n");
			return 1;
		}
		auto id = stringToFourcc(argv[2]);
		
		auto chunk = std::dynamic_pointer_cast<Chunk>(info->findChunk(id));
		if(chunk==nullptr) {
			printf("Error! chunk '%s' doesn't exist\n", argv[2]);
			return 1;
		}

		printf("'%s' = '%s'\n", argv[2], chunk->dataAsString().c_str());
	} else if(argc==4) {
		auto info = riff.findOrCreateInfoChunk();
		info->addOrModifyChunk(stringToFourcc(argv[2]), argv[3]);
		riff.writeToMemory(data);
		writeFile(std::string("new.")+argv[1], data);
	} else {
		printf("Error, wrong number of options\n");
	}

	return 0;
}
