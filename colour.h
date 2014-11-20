#ifndef OAB_COLOUR_H_INCLUDED
#define OAB_COLOUR_H_INCLUDED

#include <stdexcept>
#include <algorithm>

class Colour { // 24-bit colour container.
private:
	unsigned char r, g, b;
public:
	~Colour(){}
	Colour(): r(255), g(255), b(255){}
	Colour(int t_r, int t_g, int t_b);	// ints between 0 and 255.
	Colour(double shade);				// double between 0 and 1, where 1 = white, 0 = black. Forgiving if input exceeds bounds to allow for rounding errors.
	int getRed() const {return r;}
	int getGreen() const {return g;}
	int getBlue() const {return b;}
	double getShade() const {return (r+b+g)/(255*3);}

	Colour operator+(const Colour &rColour) const; // Crude approximation: darkest shade overides lightest shade.
};

#endif