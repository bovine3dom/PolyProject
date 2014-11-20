#include "pixel.h"

Pixel::Pixel(const Pixel & t_Pixel){
	this->colPix = t_Pixel.getColour();
	this->coordPos = t_Pixel.getPos();
}

Pixel & Pixel::operator=(const Pixel & r_Pixel){
	if (&r_Pixel == this) return *this;
	this->colPix = r_Pixel.getColour();
	this->coordPos = r_Pixel.getPos();
	return *this;
}