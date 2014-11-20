#include "image.h"

using namespace std;

Image::Image(int w, int h, const vector<Colour> & vecColourData): width(w), height(h){ // Adapted from https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
	if(w*h != vecColourData.size()){
		throw out_of_range("Image had a mismatch between dimensions and number of colours");
	}
	int iFileSize = this->getFileSize();
	unsigned char bmpFileHeader[BMP_FILE_H_SIZE] = {'B','M',0,0,0,0,0,0,0,0,BMP_H_SIZE,0,0,0};
	unsigned char bmpInfoHeader[BMP_DIB_H_SIZE] = {BMP_DIB_H_SIZE,0,0,0,0,0,0,0,0,0,0,0,1,0,24,0};

	imgData = new unsigned char [iFileSize];

	bmpFileHeader[2] = char (iFileSize);
	bmpFileHeader[3] = char (iFileSize >> 8);	// Bit-wise shift operator: shifts everything one byte to the right.
	bmpFileHeader[4] = char (iFileSize >> 16);	// We use this to ensure we write the data in a 'little endian' fashion.
	bmpFileHeader[5] = char (iFileSize >> 24);

	bmpInfoHeader[4] = char (width);
	bmpInfoHeader[5] = char (width >> 8);
	bmpInfoHeader[7] = char (width >> 24);
	bmpInfoHeader[8] = char (height);
	bmpInfoHeader[9] = char (height >> 8);
	bmpInfoHeader[10] = char (height >> 16);
	bmpInfoHeader[11] = char (height >> 24);

	for(int i = 0; i < BMP_FILE_H_SIZE; i++){
		imgData[i] = bmpFileHeader[i];
	}

	for(int i = 0; i < BMP_DIB_H_SIZE; i++){
		imgData[i+BMP_FILE_H_SIZE] = bmpInfoHeader[i];
	}

	int n(0);
	for(auto pColour = vecColourData.begin(); pColour != vecColourData.end(); pColour++){
		imgData[BMP_H_SIZE + 3*n + 2] = char (pColour->getRed());
		imgData[BMP_H_SIZE + 3*n + 1] = char (pColour->getGreen());
		imgData[BMP_H_SIZE + 3*n + 0] = char (pColour->getBlue());
		n++;
	}

};

ofstream & Image::write(ofstream &fs) const {
	unsigned char bmpPad[3] = {0,0,0};
	fs.write(reinterpret_cast<char*>(imgData), BMP_H_SIZE);
	for(int i = 0; i < height; i++){
		fs.write(reinterpret_cast<char*>(imgData+BMP_H_SIZE+(width*(height-i-1)*3)),3*width);
		fs.write(reinterpret_cast<char*>(bmpPad),(4-(width*3)%4)%4); // Pads out each row to be a multiple of 4 bytes.
	}
	return fs;
}
