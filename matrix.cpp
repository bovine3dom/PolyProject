/*

	Adapted from my Matrix assignment code.
		Changes:
					- Added throwing of exceptions.
					- Added transformation matrices.
					- Removed determinant calculation to save on lines of code.

*/

#include "matrix.h"

using namespace std;


// CONSTRUCTORS
// Parameterised constructor
Matrix::Matrix(int t_rows, int t_columns){
	rows = t_rows;
	columns = t_columns;
	mdata = new double[rows*columns];
}

// Deep-copy constructor
Matrix::Matrix(const Matrix &mat){
	rows = mat.getRows();
	columns = mat.getColumns();
	mdata = new double[rows*columns];
	if (rows > 0 && columns > 0){
		for (int row = 1; row <= rows; row++){
			for (int column = 1; column <= columns; column++){
				(*this)(row, column) = mat(row, column);
			}
		}
	}
}

const double & Matrix::index(int row, int column) const {
	if (row > 0 && column > 0 && row <= rows && column <= columns){
		return mdata[(column - 1) + (row - 1)*columns];
	}
	else {
		throw out_of_range("Matrix accessed out of range");
	}
}
const double & Matrix::operator()(int row, int column) const { return this->index(row, column); }

double & Matrix::index(int row, int column){
	if (row > 0 && column > 0 && row <= rows && column <= columns){
		return mdata[(column - 1) + (row - 1)*columns];
	}
	else {
		throw out_of_range("Matrix accessed out of range");
	}
}
double & Matrix::operator()(int row, int column){ return this->index(row, column); }


// Arithmetic
Matrix Matrix::operator+ (const Matrix &mat) const {
	if (mat.getColumns() == columns && mat.getRows() == rows){ // if matrices are of same dimension
		Matrix result(rows, columns);
		for (int row = 1; row <= rows; row++){
			for (int column = 1; column <= columns; column++){
				result(row, column) = (*this)(row, column) + mat(row, column); // add each element in turn
			}
		}
		return result; // return the resulting matrix
	}
	else { // else, terminate the program
		throw invalid_argument("Matrices must have same dimension for addition");
	}
}

Matrix Matrix::operator-(const Matrix &mat) const {
	if (mat.getColumns() == columns && mat.getRows() == rows){
		Matrix result(rows, columns);
		for (int row = 1; row <= rows; row++){
			for (int column = 1; column <= columns; column++){
				result(row, column) = (*this)(row, column) - mat(row, column);
			}
		}
		return result;
	}
	else {
		throw invalid_argument("Matrices must have same dimension for subtraction");
	}
}

Matrix Matrix::operator*(const Matrix &mat) const {
	if (mat.getRows() == columns){ // if dimensions satisfy m*n,n*p
		int m_columns = mat.getColumns();
		Matrix result(rows, m_columns);
		for (int row = 1; row <= rows; row++){
			for (int column = 1; column <= m_columns; column++){
				double temp = 0;
				for (int k = 1; k <= columns; k++){
					temp += (*this)(row, k)*mat(k, column); // do matrix multiplication
				}
				result(row, column) = temp;
			}
		}
		return result;
	}
	else {
		throw invalid_argument("Matrices must have dimension mxn,nxp for multiplication");
	}
}

Matrix Matrix::operator*(double i) const { // Allows us to multiply matrix by a constant integer
	Matrix result(rows, columns);
	for (int row = 1; row <= rows; row++){
		for (int column = 1; column <= columns; column++){
			result(row, column) = i*(*this)(row, column);
		}
	}
	return result;
}

// INPUT, OUTPUT & ASSIGNMENT
// Assignment operator
Matrix & Matrix::operator=(const Matrix &mat){
	if (&mat == this) return *this; // handles self-assignment
	delete mdata;
	rows = mat.getRows();
	columns = mat.getColumns();
	mdata = new double[rows*columns];
	if (rows > 0 && columns > 0){
		for (int row = 1; row <= rows; row++){
			for (int column = 1; column <= columns; column++){
				(*this)(row, column) = mat(row, column); // performs deep copy cell by cell
			}
		}
	}
	return *this;
}

// BOOLEAN OPERATOR
 bool Matrix::operator==(const Matrix &mat) const {
	if (&mat == this) return true; // 'Self-consistency' executed cheaply.
	if (rows != mat.getRows()) return false;
	if (columns != mat.getColumns()) return false;
	for (int row = 1; row <= rows; row++){
		for (int column = 1; column <= columns; column++){
			if ((*this)(row, column) != mat(row, column)) return false; // If a single cell doesn't match, we can escape.
		}
	}
	return true; // "Once you eliminate the impossible, whatever remains, no matter how improbable, must be the truth."
 }

Matrix RotMatrix(double theta){
	Matrix result(2,2);
	result(1, 1) = result(2,2) = cos(theta);
	result(1, 2) = sin(theta);
	result(2, 1) = -result(1, 2);
	return result;
}

Matrix ShearMatrix(double x, double y){
	Matrix result(2,2);
	result(1, 1) = result(2, 2) = 1;
	result(1, 2) = x;
	result(2, 1) = y;
	return result;
}

Matrix ReflectMatrix(double x, double y){
	Matrix result(2,2);
	double l = x*x + y*y;
	result(1, 1) = (x*x - y*y) / l;
	if (x != 0 && y != 0) {
		result(1, 2) = result(2, 1) = (2 * x*y) / l;
	} else {
		result(1, 2) = result(2, 1) = 0;
	}
	result(2, 2) = -result(1, 1);
	return result;
}

Matrix ScaleMatrix(double x, double y){
	Matrix result(2,2);
	result(1,2) = result(2,1) = 0;
	result(1,1) = x;
	result(2,2) = y;
	return result;
}