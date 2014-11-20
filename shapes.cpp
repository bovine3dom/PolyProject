#include "shapes.h"

using namespace std;

const double PI = atan(double(1)) * 4;

PolygonBase::PolygonBase(const vector<Coordinate> &vertices) {
	vecAbsVertices = vertices;
}

Coordinate PolygonBase::getCentre() const {
	Coordinate result(0,0);
	for (auto pVertex = vecAbsVertices.begin(); pVertex != vecAbsVertices.end(); pVertex++){
		result = result + *pVertex;
	}
	result = result*(1 / (double)vecAbsVertices.size());
	return result;
}

vector<Coordinate> PolygonBase::getRelVertices(Coordinate coordOrigin) const {
	vector<Coordinate> vecRelVertices;
	for (auto pVertex = vecAbsVertices.begin(); pVertex != vecAbsVertices.end(); pVertex++){
		vecRelVertices.push_back(*pVertex - coordOrigin);
	}
	return vecRelVertices;
}

vector<Line> PolygonBase::getLines() const {
	vector<Line> vecLines;
	for(auto pVertex = vecAbsVertices.begin(); pVertex != vecAbsVertices.end(); pVertex++){	// For each of my vertices,
		Coordinate coordBegin = *pVertex;
		Coordinate coordEnd;
		if((pVertex + 1) == vecAbsVertices.end()){											// find the next consecutive vertex.
			coordEnd = *vecAbsVertices.begin();
		} else {
			coordEnd = *(pVertex + 1);
		}
		vecLines.push_back(Line(coordBegin,coordEnd));										// Store the line that connects these vertices in a vector.
	}
	return vecLines;																		// Return the vector once all the lines have been stored.
}

Coordinate PolygonBase::getSize() const {
	auto pVertex = vecAbsVertices.begin();
	double minX = pVertex->index(1,1);
	double maxX = minX;
	double minY = pVertex->index(2,1);
	double maxY = minY;
	for (; pVertex != vecAbsVertices.end(); pVertex++){
		minX = min(minX,pVertex->index(1));
		maxX = max(maxX,pVertex->index(1));
		minY = min(minY,pVertex->index(2));
		maxY = max(maxY,pVertex->index(2));
	}
	return Coordinate(maxX, maxY) - Coordinate (minX, minY);
}

Line PolygonBase::getBoundaries() const {
	auto pVertex = vecAbsVertices.begin();
	double minX = pVertex->index(1);
	double minY = pVertex->index(2);
	double maxX = pVertex->index(1);
	double maxY = pVertex->index(2);
	for (; pVertex != vecAbsVertices.end(); pVertex++){
		minX = min(minX,pVertex->index(1));
		minY = min(minY,pVertex->index(2));
		maxX = max(maxX,pVertex->index(1));
		maxY = max(maxY,pVertex->index(2));
	}
	return Line(Coordinate(minX, minY),Coordinate(maxX,maxY));
}


void PolygonBase::move(double x, double y){
	for (auto pVertex = vecAbsVertices.begin(); pVertex != vecAbsVertices.end(); pVertex++){
		*pVertex = *pVertex + Coordinate(x, y);
	}
}

void PolygonBase::transform(Matrix matTrans, Coordinate coordOrigin){
	vector<Coordinate> vecRelVertices = this->getRelVertices(coordOrigin);
	
	// Iterate through both vectors at once
	auto pAbsVertex = vecAbsVertices.begin();
	for (auto pRelVertex = vecRelVertices.begin(); pRelVertex != vecRelVertices.end(); pRelVertex++){
		*pRelVertex = matTrans**pRelVertex;
		*pAbsVertex = *pRelVertex + coordOrigin;
		pAbsVertex++;
	}
}

void PolygonBase::rotate(double theta, Coordinate coordOrigin){
	this->transform(RotMatrix(theta), coordOrigin);	
}

void PolygonBase::rotate(double theta){
	this->rotate(theta,this->getCentre());
}

void PolygonBase::reflect(double x, double y, Coordinate coordOrigin){
	this->transform(ReflectMatrix(x, y), coordOrigin);
}

void PolygonBase::reflect(double x, double y){
	this->reflect(x,y,this->getCentre());
}

void PolygonBase::scale(double x, double y, Coordinate coordOrigin){
	this->transform(ScaleMatrix(x, y), coordOrigin);
}

void PolygonBase::scale(double x, double y){
	this->scale(x,y,this->getCentre());
}

void PolygonBase::shear(double x, double y){
	this->transform(ShearMatrix(x, y), this->getCentre());
}

bool PolygonDynamic::addVertex(Coordinate v){
	vector<Line> myLines = this->getLines();
	Line newLine1(*(vecAbsVertices.end()-1),v);
	Line newLine2(*(vecAbsVertices.begin()),v);
	bool result(true);
	for(auto pLine = myLines.begin(); pLine != myLines.end() - 1; pLine++){
		if(!(pLine->getStart() == newLine1.getStart() || pLine->getEnd() == newLine1.getStart() || pLine->getStart() == newLine1.getEnd() || pLine->getEnd() == newLine1.getEnd())){
			if (pLine->intersectsWith(newLine1)) result = false;
		}
		if(!(pLine->getStart() == newLine2.getStart() || pLine->getEnd() == newLine2.getStart() || pLine->getStart() == newLine2.getEnd() || pLine->getEnd() == newLine2.getEnd())){
			if (pLine->intersectsWith(newLine2)) result = false;
		}
	}
	vecAbsVertices.push_back(v);
	return result;
}

ostream & operator<<(ostream &os, PolygonBase* pPolygon){
	os << "Centre: " << pPolygon->getCentre() << "; Vertices: ";
	vector<Coordinate> vecAbsVertices = pPolygon->getAbsVertices();
	for (auto pVertex = vecAbsVertices.begin(); pVertex != vecAbsVertices.end(); pVertex++){
		os << *pVertex << " ";
	}
	return os;
}

RegularPolygon::RegularPolygon(double distance_to_vertex, int sides, Coordinate coordCentre){
	if (sides < 2) throw invalid_argument("Polygons cannot have fewer than 3 sides"); // Lines aren't really polygons, but we can draw them easily with this so why not allow it?
	for(int i = 0; i < sides; i++){
		vecAbsVertices.push_back(Coordinate(coordCentre,distance_to_vertex,double(2*i*PI/sides)));
	}
}

RegularPolygon::RegularPolygon(double distance_to_vertex, int sides, Coordinate coordCentre, double theta){
	if (sides < 2) throw invalid_argument("Polygons cannot have fewer than 3 sides");
	for(int i = 0; i < sides; i++){
		vecAbsVertices.push_back(Coordinate(coordCentre,distance_to_vertex,double(2*i*PI/sides)));
	}
	this->rotate(theta);
}

Triangle::Triangle(Coordinate c1, Coordinate c2, Coordinate c3){ // Necessary evil - VS 2010 lacks 11's initialiser lists
	vecAbsVertices.push_back(c1);
	vecAbsVertices.push_back(c2);
	vecAbsVertices.push_back(c3);
}

IsoscelesTriangle::IsoscelesTriangle(double base, double height, Coordinate coordBase) :
	Triangle(
			coordBase+Coordinate(base/2,0),
			coordBase+Coordinate(-base/2,0),
			coordBase+Coordinate(0,height)
	)
{}

Rectangle::Rectangle(double x_length, double y_length, Coordinate coordCentre){
	vecAbsVertices.push_back(coordCentre - Coordinate(x_length*0.5, y_length*0.5));
	vecAbsVertices.push_back(coordCentre - Coordinate(x_length*0.5, -y_length*0.5));
	vecAbsVertices.push_back(coordCentre + Coordinate(x_length*0.5, y_length*0.5));
	vecAbsVertices.push_back(coordCentre + Coordinate(x_length*0.5, -y_length*0.5));
}

Square::Square(double side_length, Coordinate coordCentre) :
	Rectangle(
		side_length,
		side_length,
		coordCentre
	)
{}
