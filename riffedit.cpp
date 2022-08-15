#include "Riff.h"
#include <fstream>


bool writeFile(const std::string &path, const std::vector<uint8_t> &data) {
	std::ofstream outfile(path, std::ios::out | std::ios::binary);
	if(outfile.fail()) return false;
	outfile.write((const char*)data.data(), data.size());
	if(outfile.fail()) return false;
	outfile.close();
	if(outfile.fail()) return false;
	return true;
}


std::string stripQuotes(std::string in) {
	if(in.size()>2) return in;
	if(in[0]=='\'' || in[0]=='"') {
		in = in.substr(1);
	}
	if(in[in.size()-1]=='\'' || in[in.size()-1]=='"') {
		in = in.substr(in.size()-2);
	}
	return in;
}

#include "popl.hpp"
#include <filesystem>
namespace fs = std::filesystem;

int main(int argc, char *argv[]) {

	using namespace popl;
	OptionParser op("Allowed options");
	auto help_option   = op.add<Switch>("h", "help", "produce help message");
	auto outputFileOption = op.add<Value<std::string>>("o", "output", "output file");
	auto deleteTagOption = op.add<Switch>("d", "delete-tag", "delete a tag in the info");
	auto overwriteOption = op.add<Switch>("w", "overwrite", "overwrite original file");

	op.parse(argc, argv);


	auto args = op.non_option_args();

	if(args.size() < 1 || help_option->is_set()) {
		printf("\n\nUsage: \t%s file.wav [4 char code id] [value] [options]\n\n", argv[0]);
		std::cout << op << "\n";
		return 1;
	}

	std::vector<uint8_t> data;

	std::shared_ptr<MappedFile> m;
	try {
		m = std::make_shared<MappedFile>(args[0]);
	} catch(std::runtime_error &err) {
		printf("Error: %s\n", err.what());
		return 1;
	}

	std::shared_ptr<Riff> riff;
	try {
		riff = std::make_shared<Riff>(m->data());
	} catch(std::runtime_error &err) {
		printf("got error: %s\n", err.what());
		return 1;
	}

	bool mustSave = false;
	if(args.size()==1) {
		riff->print();
	} else if(args.size()==2) {
		// get chunk
		auto info = riff->findInfoChunk();
		if(info==nullptr) {
			printf("Error! no info chunk!\n");
			return 1;
		}
		auto id = stringToFourcc(stripQuotes(args[1]));

		if(deleteTagOption->is_set()) {
			if(!info->deleteChunk(id)) {
				printf("Error - couldn't find tag %s to delete\n", args[1].c_str());
			}
			mustSave = true;
		} else {
			auto chunk = std::dynamic_pointer_cast<Chunk>(info->findChunk(id));
			if(chunk==nullptr) {
				printf("Error! chunk '%s' doesn't exist\n", args[1].c_str());
				return 1;
			}
			printf("'%s' = '%s'\n", args[1].c_str(), chunk->dataAsString().c_str());
		}
		
	} else if(args.size()>=3) {
		
		for(int i = 1; i < args.size()-1; i+=2) {
			auto id = stripQuotes(args[i]);
			auto value = stripQuotes(args[i+1]);

			auto info = riff->findOrCreateInfoChunk();

			info->addOrModifyChunk(stringToFourcc(id), value);
			mustSave = true;
		}
	} else {
		printf("Error, wrong number of options\n");
	}

	if(mustSave) {
		fs::path p(args[0]);

		// printf("parent path: %s\n", p.parent_path().string().c_str());
		p = p.parent_path() / (p.stem().string() + "-new" + p.extension().string());
		// printf("path: %s\n", p.string().c_str());
		std::string outputFile = p.string();

		if(overwriteOption->is_set()) {
			outputFile = args[0];
		}

		riff->writeToMemory(data);

		// invalidate mapping, disconnect from file
		m = nullptr;

		if(outputFile==args[0]) {
			std::string tmpName = tmpnam(nullptr);
			writeFile(tmpName, data);
			fs::remove(outputFile);
			fs::copy(tmpName, outputFile);
			fs::remove(tmpName);
		} else {
			writeFile(outputFile, data);
		}
	}

	return 0;
}
