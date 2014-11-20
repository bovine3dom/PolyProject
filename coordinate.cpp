#include "coordinate.h"

using namespace std;

Coordinate::Coordinate() : Matrix(2, 1){
	this->index(1) = this->index(2) = 0;
}

Coordinate::Coordinate(double x, double y) : Matrix(2, 1){
	this->index(1) = x;
	this->index(2) = y;
}


Coordinate::Coordinate(Coordinate start, double distance, double theta): Matrix(2, 1){
	this->index(1) = start(1) + distance * cos (theta);
	this->index(2) = start(2) + distance * sin (theta);
}

Coordinate::Coordinate(const Matrix &mat) : Matrix(2, 1){
	if ((mat.getRows() == this->getRows()) && (mat.getColumns() == this->getColumns())){
		*this = mat;
	} else {
		throw range_error("Cannot initialise coordinate with wrong dimension");
	}
}

const double & Coordinate::index(int in) const{
	return Matrix::index(in, 1);
}

const double & Coordinate::operator()(int in) const{
	return this->index(in);
}

double & Coordinate::index(int in){
	return Matrix::index(in,1);
}

double & Coordinate::operator()(int in){
	return this->index(in);
}

Coordinate Coordinate::getNormal() const {
	return Coordinate(this->index(2),this->index(1));
}

double Coordinate::operator*(const Coordinate &v) const{
	if (v.getRows() == this->getRows()){
		double result(0);
		for (int row = 1; row <= v.getRows(); row++){
			result += v(row)*this->index(row);
		}
		return result;
	} else {
		throw invalid_argument("Scalar multiplication undefined for vectors of dissimilar dimensions");
	}
}

ostream & operator<<(ostream &os, Coordinate v){
	os << "(" << v(1) << "," << v(2) << ")";
	return os;
}