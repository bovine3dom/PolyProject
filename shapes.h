#ifndef OAB_SHAPE_H_INCLUDED
#define OAB_SHAPE_H_INCLUDED


#include "coordinate.h"
#include "matrix.h"
#include "line.h"
#include <stdexcept>
#include <vector>
#include <algorithm>

// Base class from which all other polygons are formed. Does not allow addition of vertices - see PolygonDynamic instead.
class PolygonBase {
	friend std::ostream & operator<<(std::ostream &os, PolygonBase* pPolygon);
protected:
	std::vector<Coordinate> vecAbsVertices; // Co-ordinates relative to origin - we don't store lines as there would be 2x duplication.
public:
	PolygonBase(){};
	PolygonBase(const std::vector<Coordinate> &vertices);
	virtual ~PolygonBase(){}
	Coordinate getCentre() const;											// Returns a Coordinate that is the average of all the vertices.
	std::vector<Coordinate> getRelVertices(Coordinate coordOrigin) const;	// Returns coordinates of vertices relative to some arbritrary origin. Used in transfomations.
	std::vector<Coordinate> getAbsVertices() const { return vecAbsVertices; };
	std::vector<Line> getLines() const;										// Returns the lines that connect the vertices, including a line between the last and first vertex.
	Coordinate getSize() const;												// Returns Coordinate(width,height), size of boundaries. Expensive: consider storing getBoundaries and doing calculation yourself.
	Line getBoundaries() const;												// Returns a line starting at the bottom left boundary of the shape, ending at the top right boundary.

	void move(double x, double y);
	void transform(Matrix matTrans, Coordinate coordOrigin);				// Transforms with arbritrary transformation matrix wrt. arbritary origin

	void rotate(double theta, Coordinate coordOrigin);			// rotates about some origin.
	void scale(double x, double y, Coordinate coordOrigin);		// scales about some origin.
	void reflect(double x, double y, Coordinate coordOrigin);	// reflects about some origin parallel to vector.

	void rotate(double theta);			// rotates about centre.
	void scale(double x, double y);		// scales about centre.
	void reflect(double x, double y);	// reflects about centre parallel to vector.
	
	void shear(double x, double y);
};

// Allows addition of vertices after construction.
class PolygonDynamic : public PolygonBase{
public:
	PolygonDynamic(const std::vector<Coordinate> &vertices): PolygonBase(vertices){}
	bool addVertex(Coordinate v);		// adds a vertex and checks for collisions with previous lines. Returns true if no collision found.
};


// Special shapes - Glorified named constructors; separted into classes because VS2010 lacks constructor chaining.
class RegularPolygon : public PolygonBase{ // distance_to_vertex is distance from centre to vertex. First vertex will be colinear to x coordinate of centre.
public:
	RegularPolygon(double distance_to_vertex, int sides, Coordinate coordCentre);
	RegularPolygon(double distance_to_vertex, int sides, Coordinate coordCentre, double theta);
};

class Pentagon : public RegularPolygon{
public:
	Pentagon(double distance_to_vertex, Coordinate coordCentre): RegularPolygon(distance_to_vertex, 6, coordCentre){}
};

class Hexagon : public RegularPolygon{
public:
	Hexagon(double distance_to_vertex, Coordinate coordCentre): RegularPolygon(distance_to_vertex, 6, coordCentre){}
};

class Triangle : public PolygonBase {
public:
	Triangle(Coordinate c1, Coordinate c2, Coordinate c3);
};

class IsoscelesTriangle : public Triangle{
public:
	IsoscelesTriangle(double base, double height, Coordinate coordBase);
};

class Rectangle : public PolygonBase {
public:
	Rectangle(double x_length, double y_length, Coordinate coordCentre);
};

class Square : public Rectangle {
public:
	Square(double side_length, Coordinate coordCentre);
};

#endif
