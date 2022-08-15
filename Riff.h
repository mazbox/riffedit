#include "MappedFile.h"
#include <cstdint>
#include <vector>
#include <string>
#include <memory>

/**
 * Be careful! this needs to be little endian - you can't just cast it out of the file.
 */
std::string fourccToDescription(uint32_t fourcc) {

	// from https://exiftool.org/TagNames/RIFF.html
	switch(fourcc) {
		case 'AGES': return "Rated";
		case 'CMNT': return "Comment";
		case 'CODE': return "EncodedBy";
		case 'COMM': return "Comments";
		case 'DIRC': return "Directory";
		case 'DISP': return "SoundSchemeTitle";
		case 'DTIM': return "DateTimeOriginal";
		case 'GENR': return "Genre";
		case 'IARL': return "ArchivalLocation";
		case 'IART': return "Artist";
		case 'IAS1': return "FirstLanguage";
		case 'IAS2': return "SecondLanguage";
		case 'IAS3': return "ThirdLanguage";
		case 'IAS4': return "FourthLanguage";
		case 'IAS5': return "FifthLanguage";
		case 'IAS6': return "SixthLanguage";
		case 'IAS7': return "SeventhLanguage";
		case 'IAS8': return "EighthLanguage";
		case 'IAS9': return "NinthLanguage";
		case 'IBSU': return "BaseURL";
		case 'ICAS': return "DefaultAudioStream";
		case 'ICDS': return "CostumeDesigner";
		case 'ICMS': return "Commissioned";
		case 'ICMT': return "Comment";
		case 'ICNM': return "Cinematographer";
		case 'ICNT': return "Country";
		case 'ICOP': return "Copyright";
		case 'ICRD': return "DateCreated";
		case 'ICRP': return "Cropped";
		case 'IDIM': return "Dimensions";
		case 'IDIT': return "DateTimeOriginal";
		case 'IDPI': return "DotsPerInch";
		case 'IDST': return "DistributedBy";
		case 'IEDT': return "EditedBy";
		case 'IENC': return "EncodedBy";
		case 'IENG': return "Engineer";
		case 'IGNR': return "Genre";
		case 'IKEY': return "Keywords";
		case 'ILGT': return "Lightness";
		case 'ILGU': return "LogoURL";
		case 'ILIU': return "LogoIconURL";
		case 'ILNG': return "Language";
		case 'IMBI': return "MoreInfoBannerImage";
		case 'IMBU': return "MoreInfoBannerURL";
		case 'IMED': return "Medium";
		case 'IMIT': return "MoreInfoText";
		case 'IMIU': return "MoreInfoURL";
		case 'IMUS': return "MusicBy";
		case 'INAM': return "Title";
		case 'IPDS': return "ProductionDesigner";
		case 'IPLT': return "NumColors";
		case 'IPRD': return "Product";
		case 'IPRO': return "ProducedBy";
		case 'IRIP': return "RippedBy";
		case 'IRTD': return "Rating";
		case 'ISBJ': return "Subject";
		case 'ISFT': return "Software";
		case 'ISGN': return "SecondaryGenre";
		case 'ISHP': return "Sharpness";
		case 'ISMP': return "TimeCode";
		case 'ISRC': return "Source";
		case 'ISRF': return "SourceForm";
		case 'ISTD': return "ProductionStudio";
		case 'ISTR': return "Starring";
		case 'ITCH': return "Technician";
		case 'ITRK': return "TrackNumber";
		case 'IWMU': return "WatermarkURL";
		case 'IWRI': return "WrittenBy";
		case 'LANG': return "Language";
		case 'LOCA': return "Location";
		case 'PRT1': return "Part";
		case 'PRT2': return "NumberOfParts";
		case 'RATE': return "Rate";
		case 'STAR': return "Starring";
		case 'STAT': return "Statistics";
		case 'TAPE': return "TapeName";
		case 'TCDO': return "EndTimecode";
		case 'TCOD': return "StartTimecode";
		case 'TITL': return "Title";
		case 'TLEN': return "Length";
		case 'TORG': return "Organization";
		case 'TRCK': return "TrackNumber";
		case 'TURL': return "URL";
		case 'TVER': return "Version";
		case 'VMAJ': return "VegasVersionMajor";
		case 'VMIN': return "VegasVersionMinor";
		case 'YEAR': return "Year";
		default: return "Unknown";
	}
}








// don't think I need this but maybe
#pragma pack(2)

struct ChunkFormat {
	char id[4];
	uint32_t size;
	uint8_t data[1];
	
};

struct ContainerChunkFormat {
	char id[4];
	uint32_t size;
	char format[4];
	uint8_t data[1];
	
};


#define WAVE_FORMAT_PCM        0x0001	// PCM
#define WAVE_FORMAT_IEEE_FLOAT 0x0003	// IEEE float
#define WAVE_FORMAT_ALAW       0x0006	// 8-bit ITU-T G.711 A-law
#define WAVE_FORMAT_MULAW      0x0007	// 8-bit ITU-T G.711 µ-law
#define WAVE_FORMAT_EXTENSIBLE 0xFFFE	// Determined by SubFormat

struct WavFmtChunkFormat {
	// char id[4]; // 'fmt '
	// uint32_t size;
	uint16_t format;
	uint16_t channels;
	uint32_t sampleRate;
	uint32_t dataRate;
	uint16_t blockAlign;
	uint16_t bitsPerSample;


	// this stuff is optionally in the file
	// see here: http://www-mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
	// probably best not to use these
	uint16_t extensionSize;
	uint16_t validBitsPerSample;
	uint32_t channelMask;
	uint32_t subFormat[4];

};

typedef uint32_t fourcc;

std::string rawToFourccString(uint8_t *d) {
		return std::string(1, (char)d[0]) + (char)d[1] + (char)d[2] + (char)d[3];
	}

/**
 * Be careful here! this flips the endianness
 * in order to work with 4 char long single quoted literals.
 */
uint32_t rawToFourcc(uint8_t *data) {
	return data[3]       |
	       data[2] << 8  |
	       data[1] << 16 |
	       data[0] << 24 ;
}

fourcc stringToFourcc(const std::string &s) {
	return rawToFourcc((uint8_t*)s.data());
}

void fourccToRaw(fourcc f, uint8_t *data) {
	uint8_t *cc = (uint8_t*)&f;
	data[0] = cc[3];
	data[1] = cc[2];
	data[2] = cc[1];
	data[3] = cc[0];
}



std::string fourccToString(fourcc v) {
	char out[5];
	out[4] = 0;
	out[0] = (v >> 24) & 0xFF;
	out[1] = (v >> 16) & 0xFF;
	out[2] = (v >>  8) & 0xFF;
	out[3] = v & 0xFF;
	return out;
}

struct BaseChunk {
	fourcc id;

	virtual ~BaseChunk() {}
	virtual void printInfo() = 0;
	virtual uint8_t *write(uint8_t *d) = 0;

	virtual int getSizeIncludingHeaderAndPadding() = 0;


	virtual void printTree(int indent = 0) {
		for(int i = 0; i < indent; i++) printf("  ");
		printInfo();
	}
};


struct RiffStorage {
	std::vector<uint8_t> vec;
	uint8_t *ptr = nullptr;
	int sizeAtPtr = 0;

	size_t size() {
		if(ptr==nullptr) {
			return vec.size();
		}
		return sizeAtPtr;
	}

	void copyString(const std::string &strData) {
		vec.resize(strData.size()+1);
		memcpy(vec.data(), strData.data(), strData.size());
		vec[strData.size()] = 0;
		ptr = nullptr;
	}

	void set(uint8_t *data, int sz) {
		this->ptr = data;
		this->sizeAtPtr = sz;
	}
	uint8_t *data() {
		if(ptr!=nullptr) return ptr;
		return vec.data();
	}
};

struct Chunk : public BaseChunk {
	
	
	RiffStorage storage;

	Chunk(fourcc id, const std::string &strData) {
		this->id = id;
		setValue(strData);
	}

	void setValue(const std::string &strData) {
		storage.copyString(strData);
	}
	
	Chunk(uint8_t *d) {
		auto *strct = (ChunkFormat*)d;
		id = rawToFourcc((uint8_t*)&strct->id[0]);
		// data.resize(strct->size);
		// memcpy(data.data(), &strct->data, data.size());
		storage.set(strct->data, strct->size);
	}

	void printInfo() override {
		printf("'%s' chunk (size: %lu bytes)\n", 
			fourccToString(id).c_str(), 
			storage.size());
	}

	std::string dataAsString() {
		return std::string((char*)storage.data());
	}

	int getSizeIncludingHeaderAndPadding() override {
		int sz = storage.size() + 8;
		if(sz%2==1) sz++; // padding
		return sz;
	}

	uint8_t *write(uint8_t *d) override {
		fourccToRaw(id, d);
		d += 4;
		auto *dd = (uint32_t*)d;
		dd[0] = storage.size();
		d += 4;
		memcpy(d, storage.data(), storage.size());

		d += storage.size();
		if(storage.size()%2==1) {
			d[0] = 0;
			d++; // padding
		}

		return d;
	}
};


#include <assert.h>
struct ContainerChunk : public BaseChunk {

	fourcc format;
	std::vector<std::shared_ptr<BaseChunk>> children;

	ContainerChunk(fourcc id, fourcc format) {
		this->id = id;
		this->format = format;
	}

	ContainerChunk(const uint8_t *d) {
		ContainerChunkFormat *data = (ContainerChunkFormat*)d;
		id = rawToFourcc((uint8_t*)&data->id[0]);
		format = rawToFourcc((uint8_t*)&data->format[0]);

		parseChildren(data);
		assert(originalSize==calculateSize());
		// printf("size: %d, calculated size: %d\n", originalSize, calculateSize());
	}


	uint8_t *write(uint8_t *d) override {
		fourccToRaw(id, d);
		d += 4;
		auto *dd = (uint32_t*)d;
		dd[0] = calculateSize();
		d += 4;

		fourccToRaw(format, d);
		d += 4;
		for(auto &c : children) {
			d = c->write(d);
		}
		return d;
	}

	int getSizeIncludingHeaderAndPadding() override {
		int sz = 0;
		for(auto ch : children) {
			sz += ch->getSizeIncludingHeaderAndPadding();
		}
		sz += 4; // for the format tag
		sz += 8; // header
		if(sz%2==1) sz++; // padding
		return sz;
	}

	void addOrModifyChunk(fourcc id, const std::string &data) {
		auto newChunk = findChunk(id);
		if(newChunk==nullptr) {
			newChunk = std::make_shared<Chunk>(id, data);
			addChild(newChunk);
		} else {
			auto ch = std::dynamic_pointer_cast<Chunk>(newChunk);
			ch->setValue(data);
		}
	}

	int calculateSize() {
		return getSizeIncludingHeaderAndPadding() - 8;
	}

	ContainerChunk(const std::vector<uint8_t> &d) : ContainerChunk(d.data()) {}
	
	void printInfo() override {
		printf("'%s' container chunk (size: %d bytes) - format '%s'\n",  
			fourccToString(id).c_str(), 
			originalSize,
			fourccToString(format).c_str());
	}


	void addChild(std::shared_ptr<BaseChunk> ch) {
		children.push_back(ch);
	}

	void printTree(int indent = 0) override {
		for(int i = 0; i < indent; i++) printf("  ");
		printInfo();
		indent++;
		for(auto ch : children) {
			ch->printTree(indent);
		}
	}

	bool deleteChunk(fourcc id) {
		for(int i = 0; i < children.size(); i++) {
			if(children[i]->id==id) {
				children.erase(children.begin() + i);
				return true;
			}
		} 
		return false;
	}

	std::shared_ptr<BaseChunk> findChunk(fourcc id) {
		for(auto ch : children) {
			if(ch->id==id) return ch;
			else printf("Found chunk %s\n", fourccToString(ch->id).c_str());
		}
		return nullptr;
	}

	std::shared_ptr<ContainerChunk> findOrCreateInfoChunk() {
		auto info = findInfoChunk();
		if(info!=nullptr) {
			return info;
		}
		info = std::make_shared<ContainerChunk>('LIST', 'INFO');
		children.push_back(info);
		return info;
	}

	/**
	 * Returns the info chunk if present, or returns nullptr if it can't find it
	 */
	std::shared_ptr<ContainerChunk> findInfoChunk() {
		for(auto ch : children) {
			if(ch->id!='LIST') {
				continue;
			}
			
			auto con = std::dynamic_pointer_cast<ContainerChunk>(ch);

			if(con->format!='INFO') {
				continue;				
			}
			return con;
		}
		return nullptr;
	}


private:
	int originalSize = 0;
	void parseChildren(ContainerChunkFormat *data) {

		// pos should always point to the beginning of the next chunk
		uint8_t *pos = data->data;

		originalSize = data->size;
		auto *dataEnd = data->data + data->size - 8;
		while(pos < dataEnd) {
			auto type = rawToFourcc(pos);
			
			std::shared_ptr<BaseChunk> child = nullptr;

			if(type=='RIFF' || type=='LIST') {
				child = std::make_shared<ContainerChunk>(pos);	
			} else {
				child = std::make_shared<Chunk>(pos);	
			}

			children.push_back(child);
			auto *childData = (ChunkFormat*) pos;
			int sz = childData->size;
			pos += sz + 8;
			if(sz%2==1) pos++; // padding byte
		}
	}
};


#pragma pack()
class Riff : public ContainerChunk {
public:

	

	Riff(std::shared_ptr<MappedFile> mappedFile) : Riff(mappedFile->data()) {}
	Riff(const uint8_t *data) : ContainerChunk(data) {}
	
	void print() {
		printTree();

		if(format=='WAVE') {
			auto ch = findChunk('fmt ');
			if(ch!=nullptr) {
				printf("Its a WAVE file\n");
				printWaveFormat(std::dynamic_pointer_cast<Chunk>(ch));
			}
		}

		auto info = std::dynamic_pointer_cast<ContainerChunk>(findInfoChunk());
		if(info!=nullptr) {
            printf("found info list with %lu children\n", info->children.size());
			for(auto i : info->children) {
				auto ic = std::dynamic_pointer_cast<Chunk>(i);
				printf("%s (%s) = '%s'\n", fourccToDescription(i->id).c_str(), fourccToString(i->id).c_str(), ic->dataAsString().c_str());	
			}			
		}
	}

	void printWaveFormat(std::shared_ptr<Chunk> fmtChunk) {
		auto *fmt = (WavFmtChunkFormat*)fmtChunk->storage.data();
		printf("%d channel%c @ %dHz / %d bit\n", fmt->channels, fmt->channels>1?'s':0x1a, fmt->sampleRate, fmt->bitsPerSample);
	}


	void writeToMemory(std::vector<uint8_t> &d) {
		d.resize(getSizeIncludingHeaderAndPadding());
		write(d.data());
	}
};



