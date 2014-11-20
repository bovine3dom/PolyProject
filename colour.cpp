#include "colour.h"

using namespace std;

Colour::Colour(int t_r, int t_g, int t_b): r((unsigned char) (t_r)), g((unsigned char)(t_g)), b((unsigned char)(t_b)){
	if ((t_r > 255) || (t_g > 255) || (t_b > 255) || (t_r < 0) || (t_g < 0) || (t_b < 0)){
		throw invalid_argument("Colour colour cannot exceed 255 or be negative");
		throw std::out_of_range("");
	}
}

Colour::Colour(double shade){
	if( shade < 0){ // protects from rounding errors with doubles
		r = g = b = 0;
	} else if (shade > 1){
		r = g = b = 255;
	} else {
		r = g = b = int (255*shade);
	}
}

Colour Colour::operator+(const Colour &rColour) const {
	int resultR = min(r,rColour.r);
	int resultG = min(g,rColour.g);
	int resultB = min(b,rColour.b);
	return Colour(resultR, resultG, resultB);
}
