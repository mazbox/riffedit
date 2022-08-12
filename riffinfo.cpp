#include "Riff.h"

#include <stdio.h>
#include <vector>
#include <iostream>
#include <fstream>

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


int main(int argc, char *argv[]) {
	if(argc!=2) {
		printf("\n\nUsage: \t%s file.wav\n\n", argv[0]);
		return 1;
	}

	std::vector<uint8_t> data;
	if(!readFile(argv[1], data)) {
		return 1;
	}

	Riff riff(data);
	riff.print();
	return 0;
}
