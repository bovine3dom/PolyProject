#ifndef OAB_CANVAS_H_INCLUDED
#define OAB_CANVAS_H_INCLUDED

#include <vector>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <ctime>

#include "coordinate.h"
#include "shapes.h"
#include "line.h"
#include "pixel.h"
#include "image.h"

// A way of grouping shapes together.
class Canvas {
	friend std::ostream & operator<<(std::ostream &os, const Canvas &c);
private:
	std::vector<PolygonBase*> vecPPoly;
public:
	~Canvas(){}
	Canvas(){}
	Canvas(std::vector<PolygonBase*> t_vecPPoly): vecPPoly(t_vecPPoly){if(vecPPoly.size() == 0) throw std::invalid_argument("Canvas cannot be initialised with empty vector");} 

	Line getBoundaries() const;		// Expensive - iterates through every vertex. Recommend storage. Returns Line(bottom left coord, top right coord)
	Coordinate getSize() const;		// Expensive - iterates through every vertex; recommend use + storage of getBoundaries(). Returns Coordinate(width,height)
	Coordinate getCentre() const;	// Expensive - iterates through every vertex; recommend use + storage of getBoundaries().
	
	void linkShape(PolygonBase* pPolyShape){vecPPoly.push_back(pPolyShape);}		// Adds a shape to the canvas. Any transformation will be applied to base shape.
	void setShapes(std::vector<PolygonBase*> t_vecPPoly){vecPPoly = t_vecPPoly;}	// Removes all previous links to shapes and replaces them with a new set of links.

	void scale(double x, double y);				// Scales all shapes relative to centre.
	void resize(int width, int height);			// Resizes the canvas such that its size is (width,height)
	void rotate(double theta);					// Rotates all shapes clockwise by theta radians about canvas' centre.
	void reflect(double x, double y);			// Reflects all shapes on the canvas parallel line going through the origin in the direction x,y
	void move(double x, double y);				// Moves all shapes by (x,y)
	void autocrop();							// Moves all shapes on the canvas to the positive quadrant (no coordinates below (0,0))
	void write(const char * cFilename);				// Extremely expensive. Rasterizes all shapes and outputs it as a BMP with the filename specified. 1:1 coordinates to pixels.
	void clear(){vecPPoly.clear();}				// Removes association of shapes with canvas.
};

#endif

// END OF FILE
