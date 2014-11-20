#ifndef OAB_IMAGE_H_INCLUDED
#define OAB_IMAGE_H_INCLUDED

#include <fstream>
#include <stdexcept>
#include <vector>
#include <algorithm>
#include "colour.h"

// Conforms to BMP specification as set out at http://msdn.microsoft.com/en-us/library/windows/desktop/dd183391.aspx
class Image { // Can take up a lot of memory. Highly recommend to write once + dispose.
private:
	static const int BMP_FILE_H_SIZE = 14;	// Non-negotiable: 14 bytes set in stone in BMP specification
	static const int BMP_DIB_H_SIZE = 40;	// Standard size for 24-bit BMPV5.
	static const int BMP_H_SIZE = BMP_DIB_H_SIZE + BMP_FILE_H_SIZE;

	int width, height;
	unsigned char * imgData;

public:
	~Image(){delete imgData;}
	Image(int w, int h, const std::vector<Colour> & vecColourData);		// Vector length must be equal to w*h. Pixels are placed consecutively, left to right, top to bottom.
	int getFileSize() const {return BMP_H_SIZE + 3 * width * height;}	// Returns size of file in bytes.
	std::ofstream & write(std::ofstream &fs) const;						// Writes imgData to the file stream.
};

#endif