#ifndef OAB_MATRIX_H_INCLUDED
#define OAB_MATRIX_H_INCLUDED

#include <iostream>
#include <cmath>
#include <stdexcept>

// Container for n x m dimensional matrices of doubles.
class Matrix{
private:
	int rows, columns;	// IMPORTANT: We number rows and columns from 1 onwards. First column = 1, etc.
	double *mdata;		// C-Style array of doubles in which we store the matrix.

public:
	// Default constructor
	Matrix(){ mdata = 0; rows = columns = 0; }

	// Parameterised constructors
	Matrix(int t_rows, int t_columns);

	// Copy constructor
	Matrix(const Matrix &mat); // Performs a deep copy.

	// Destructor
	virtual ~Matrix(){ delete mdata; }

	// Public access functions
	// const variants
	int getRows() const { return rows; }
	int getColumns() const { return columns; }
	virtual const double & index(int row, int column) const;		// returns a const reference to matrix member at row, column. Rows & columns are numbered from 1.
	virtual const double & operator()(int row, int column) const;	// vanity for index function

	// non-const variants
	virtual double & index(int row, int column);		// returns a reference to matrix member at row, column
	virtual double & operator()(int row, int column);	// vanity for index function

	// Assignment function
	virtual Matrix & operator=(const Matrix &mat);		// performs deep copy

	// Arithmetical operators
	virtual Matrix operator+(const Matrix &mat) const; 
	virtual Matrix operator-(const Matrix &mat) const; 
	virtual Matrix operator*(const Matrix &mat) const; 
	virtual Matrix operator*(double i) const; 
	
	// Boolean operators
	bool operator==(const Matrix &mat) const;			// returns true if every element of matrices match
};

// Helper functions for transfomation of 2D co-ordinates.
Matrix RotMatrix(double theta);				// Rotation of theta degrees clockwise about the origin.
Matrix ReflectMatrix(double x, double y);	// Reflection parallel to a line passing through x,y from the origin.
Matrix ShearMatrix(double x, double y);
Matrix ScaleMatrix(double x, double y);		// Geometrically scales about the origin.

#endif
