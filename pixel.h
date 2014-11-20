#ifndef OAB_PIXEL_H_INCLUDED
#define OAB_PIXEL_H_INCLUDED

#include "colour.h"
#include "coordinate.h"


// Container for pixel with 24-bit colour.
class Pixel{ 
private:
	Coordinate coordPos;
	Colour colPix;
public:
	~Pixel(){}
	Pixel(Coordinate t_coord): coordPos(t_coord), colPix(1){}	// Pixels are white unless otherwise specified.
	Pixel(Coordinate t_coord, Colour col): coordPos(t_coord), colPix(col){}
	Pixel(const Pixel & t_Pixel);
	Pixel & operator=(const Pixel & r_Pixel);

	Coordinate getPos() const {return coordPos;}
	Colour getColour() const {return colPix;}
};

#endif
