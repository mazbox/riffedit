#include <stdio.h>
// This is for open. 
#include <fcntl.h>
#include <string>
#include <stdint.h>
#include <exception>

// For the size of the file. 
#include <sys/stat.h>
// This contains the mmap calls. 
#include <sys/mman.h> 

// for close
#include <unistd.h>

class MappedFile {
public:
	int fd = 0;
	int size = 0;
	void *mapped = nullptr;
	MappedFile(const std::string &path) {
		// Open the file for reading.
		int fd = open (path.c_str(), O_RDONLY);
		if(fd<0) {
			printf("open %s failed: %s\n", path.c_str(), strerror (errno));
			throw std::runtime_error("Can't open file for reading: " + path);
		}

		// use fs filesize for this
		struct stat s;
	    // Get the size of the file. 
	    int status = fstat (fd, &s);
		
		if(status < 0) {
			printf("stat %s failed: %s\n", path.c_str(), strerror (errno));
			throw std::runtime_error("Can't stat file: " + path);
		}

	    size = s.st_size;

	    // Memory-map the file.
	    mapped = mmap (0, size, PROT_READ, MAP_PRIVATE, fd, 0);
		if(mapped == MAP_FAILED) {
			printf("mmap %s failed: %s\n", path.c_str(), strerror (errno));
			throw std::runtime_error("Can't mmapf file: " + path);
		}

		madvise (mapped, size, MADV_SEQUENTIAL);
	}

	uint8_t *data() {
		return (uint8_t*) mapped;
	}
	~MappedFile() {
		munmap(mapped, size);
		close(fd);
	}
};
