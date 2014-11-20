#ifndef OAB_LINE_H_INCLUDED
#define OAB_LINE_H_INCLUDED

#include "colour.h" // Needed for anti-aliasing
#include "pixel.h"
#include "coordinate.h"
#include <stdexcept>
#include <vector>

class Line { // A set of two co-ordinates. Can be converted to a set of anti-aliased pixels with getRaster().
	friend std::ostream & operator<<(std::ostream &os, Line l); // Human-readable stream output.
private:
	Coordinate start;
	Coordinate end;
public:
	~Line(){}
	Line(){}
	Line(Coordinate t_start, Coordinate t_end);
	bool intersectsWith(const Line & linInter) const;	// Returns true if lines intersect (excludes parallel lines).
	const Coordinate & getStart() const {return start;}
	const Coordinate & getEnd() const {return end;}

	std::vector<Pixel> getRaster() const;				// Expensive: uses Wu's Line Algorithm (http://en.wikipedia.org/wiki/Xiaolin_Wu%27s_line_algorithm),
														// returns vector containing the co-ordinates and colours of pixels corresponding to anti-aliased raster of line.
	Line & operator=(const Line &lLine);				// Performs deep copy.
};

#endif
