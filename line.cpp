#include "line.h"

using namespace std;

Line::Line(Coordinate t_start, Coordinate t_end): start(t_start), end(t_end){
	if (t_end == t_start){
		throw std::invalid_argument("Lines cannot have zero length");
	}
}

bool Line::intersectsWith(const Line & linInter) const{
	// Work out the equation of the line: Ax + By = C
	double x11 = this->getStart()(1);
	double x12 = this->getEnd()(1);
	double y11 = this->getStart()(2);
	double y12 = this->getEnd()(2);

	double x21 = linInter.getStart()(1);
	double x22 = linInter.getEnd()(1);
	double y21 = linInter.getStart()(2);
	double y22 = linInter.getEnd()(2);

	double A1 = y12 - y11;
	double B1 = x11 - x12;
	double C1 = A1*x11 + B1*y11;

	double A2 = y22 - y21;
	double B2 = x21 - x22;
	double C2 = A2*x21 + B2*y21;

	double determinant = A1*B2 - A2*B1;

	if (determinant != 0){ // Deals with parallel case.
		double dInterX = (B2*C1 - B1*C2)/determinant;
		double dInterY = (A1*C2 - A2*C1)/determinant;

		bool bX1 = (min(x12,x11) <= dInterX) && (dInterX <= max(x12,x11));
		bool bY1 = (min(y12,y11) <= dInterY) && (dInterY <= max(y12,y11));

		bool bX2 = (min(x22,x21) <= dInterX) && (dInterX <= max(x22,x21));
		bool bY2 = (min(y22,y21) <= dInterY) && (dInterY <= max(y22,y21));
		return bX1 && bY1 && bY2 && bX2;
	} else {
		return false;
	}
}

vector<Pixel> Line::getRaster() const {
	vector<Pixel> vecRaster;
	double x0 = this->getStart()(1);
	double y0 = this->getStart()(2);

	double x1 = this->getEnd()(1);
	double y1 = this->getEnd()(2);

	bool isSteep = abs(y1 - y0) > abs (x1 - x0);
	if (isSteep) {			// If the line goes up more than it goes across,
		swap(x0,y0);		// swap the x and y co-ordinates (C++ Standard Libary algorithm).
		swap(x1,y1);
	}
	if (x0 > x1) {
		swap(x0,x1);
		swap(y0,y1);
	}
	double gradient = (y1 - y0) / (x1 - x0);

	// Deal with the ends of the line first.
	double xend;
	double xgap = 1 - modf((x0+0.5),&xend);
	double yend = y0 + gradient * (xend - x0);
	double t_ypxl1;
	double ygap = 1 - modf(yend,&t_ypxl1);
	int ypxl1 = int(t_ypxl1);
	int xpxl1 = int(xend);

	if (isSteep) {
		vecRaster.push_back(Pixel(Coordinate(ypxl1,xpxl1),Colour((1-ygap)*xgap)));
	} else {
		vecRaster.push_back(Pixel(Coordinate(xpxl1,ypxl1),Colour((1-ygap)*xgap)));
	}
	double intery = yend + gradient;

	xgap = 1 - modf((x1+0.5),&xend);
	yend = y1 + gradient * (xend - x1);
	double t_ypxl2;
	ygap = 1 - modf(yend,&t_ypxl2);
	int ypxl2 = int(t_ypxl2);
	int xpxl2 = int(xend);

	if (isSteep) {
		vecRaster.push_back(Pixel(Coordinate(ypxl2, xpxl2), Colour((1-ygap)*xgap)));
	} else {
		vecRaster.push_back(Pixel(Coordinate(xpxl2, ypxl2), Colour((1-ygap)*xgap)));
	}


	// Deal with the rest of the line.
	for (int x = xpxl1 + 1; x < xpxl2; x++) {	// For every pixel that we have not yet rasterised,
		double gap, t_pos;
		gap = 1 - modf(intery,&t_pos);			// Work out how much the x-coordinate spills over into the next pixel (C++ Standard Library algorithm)
		int pos = int (t_pos);					// Find the x position.
		if (isSteep){
			vecRaster.push_back(Pixel(Coordinate(pos+1, x), Colour(gap))); // Create the pixel with the right shade,
			vecRaster.push_back(Pixel(Coordinate(pos, x), Colour(1-gap))); // and the pixel next to it.
		} else {
			vecRaster.push_back(Pixel(Coordinate(x, pos+1), Colour(gap)));
			vecRaster.push_back(Pixel(Coordinate(x, pos), Colour(1-gap)));
		}
		intery = intery + gradient;				// Move to the next y position.
	}
	return vecRaster;
}

Line & Line::operator=(const Line &lLine){
	if(&lLine == this) return *this;
	this->start = lLine.getStart();
	this->end = lLine.getEnd();
	return *this;
}

ostream & operator<<(ostream &os, Line l){
	os << l.getStart() << "," << l.getEnd() ;
	return os;
}