#ifndef OAB_COORD_H_INCLUDED
#define OAB_COORD_H_INCLUDED

#include "matrix.h"
#include <stdexcept>
#include <iostream>
#include <vector>

// Coordinate. Behaves like a 2D mathematical vector.
class Coordinate : public Matrix {
	friend std::ostream & operator<<(std::ostream &os, Coordinate v);
public:
	Coordinate();
	Coordinate(const Matrix &mat);			// Performs deep copy of Matrix. Throws range_error if Matrix is of wrong dimension.
	Coordinate(double x, double y);

	Coordinate(Coordinate start, double distance, double theta); // Theta is measured clockwise from positive x-axis.
	const double & index(int in) const;		// Returns reference to coordinate member. 1 => x; 2 => y.
	const double & operator()(int in) const;// Vanity for index function.
	double & index(int in);
	double & operator()(int in);
	Coordinate getNormal() const;			// Returns Coordinate with x,y swapped. Not normalised.

	using Matrix::index;					// Forces the compiler to inherit partially-overloaded member functions from Matrix.
	using Matrix::operator();
	using Matrix::operator=;

	double operator*(const Coordinate & v) const;	// Scalar product.
	using Matrix::operator*;
};


#endif