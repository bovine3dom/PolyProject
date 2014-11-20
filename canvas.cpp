#include "canvas.h"

using namespace std;


void Canvas::autocrop(){
	Coordinate coordMin = this->getBoundaries().getStart();
	for(auto ppShape = vecPPoly.begin(); ppShape != vecPPoly.end(); ppShape++){
		auto pShape = *ppShape;
		pShape->move(-coordMin(1), -coordMin(2));
	}
}

Line Canvas::getBoundaries() const{
	auto ppShape = vecPPoly.begin();
	auto pShape = *ppShape;
	Line shapeBounds = pShape->getBoundaries();
	double minX = shapeBounds.getStart()(1);
	double minY = shapeBounds.getStart()(2);
	double maxX = shapeBounds.getEnd()(1);
	double maxY = shapeBounds.getEnd()(2);
	for (; ppShape != vecPPoly.end(); ppShape++){
		pShape = *ppShape;
		shapeBounds = pShape->getBoundaries();
		minX = min(minX, shapeBounds.getStart()(1));
		minY = min(minY, shapeBounds.getStart()(2));
		maxX = max(maxX, shapeBounds.getEnd()(1));
		maxY = max(maxY, shapeBounds.getEnd()(2));
	}
	return Line(Coordinate(minX, minY),Coordinate(maxX,maxY));
}

Coordinate Canvas::getCentre() const{
	Line linBound = this->getBoundaries();
	return (linBound.getStart() + linBound.getEnd()) * 0.5;
}

Coordinate Canvas::getSize() const {
	Line linBound = this->getBoundaries();
	Coordinate coordUpper = linBound.getEnd();
	Coordinate coordLower = linBound.getStart();
	return Coordinate(coordUpper-coordLower);
}

void Canvas::scale(double x, double y){
	Coordinate coordCentre = this->getCentre();
	for(auto ppShape = vecPPoly.begin(); ppShape != vecPPoly.end(); ppShape++){
		auto pShape = *ppShape;
		pShape->scale(x, y, coordCentre);
	}
}

void Canvas::rotate(double theta){
	Coordinate coordCentre = this->getCentre();
	for(auto ppShape = vecPPoly.begin(); ppShape != vecPPoly.end(); ppShape++){
		auto pShape = *ppShape;
		pShape->rotate(theta, coordCentre);
	}
}

void Canvas::reflect(double x, double y){
	Coordinate coordCentre = this->getCentre();
	for(auto ppShape = vecPPoly.begin(); ppShape != vecPPoly.end(); ppShape++){
		auto pShape = *ppShape;
		pShape->reflect(x, y, coordCentre);
	}
}


void Canvas::resize(int width, int height){
	Coordinate coordSize = this->getSize();
	double xScaleFactor = double(width) / coordSize(1);
	double yScaleFactor = double(height) / coordSize(2);
	this->scale(xScaleFactor, yScaleFactor);
}


ostream & operator<<(std::ostream &os, const Canvas &c){
	for(auto ppShape = c.vecPPoly.begin(); ppShape != c.vecPPoly.end(); ppShape++){
		os << *ppShape << endl;
	}
	return os;
}

void Canvas::move(double x, double y){
	for(auto ppShape = vecPPoly.begin(); ppShape != vecPPoly.end(); ppShape++){ // Move everything into the positive quadrant
		auto pShape = *ppShape;
		pShape->move(x, y);
	}
}

void Canvas::write(const char* cFilename) {
	if(vecPPoly.size() != 0){ // Don't bother making an empty image.

		// Prepare the shapes to be put onto the canvas
		Line linBound = this->getBoundaries();		// Work out the dimensions of the image,
		
		// Prepare
		int imgHeight = int ((linBound.getEnd()-linBound.getStart())(2,1))+2;	// Calculates size of image and adds a pixel of padding
		int imgWidth = int ((linBound.getEnd()-linBound.getStart())(1,1))+2;	// to each border

		if(imgHeight*imgWidth > 225000000) throw invalid_argument("Image is larger than 225 megapixels. Programme cannot allocate this much memory");

		Coordinate coordMin = linBound.getStart();	// Store minimum coordinate										
		this->move(-coordMin(1)+1,-coordMin(2)+1);	// with which we shift all shapes into the positive quadrant with a pixel of padding

		vector<Colour> vecPixelData;
		vecPixelData.resize(imgHeight*imgWidth);	// Prepare the pixel vector to recieve data; implicitly paints the background white.

		// Initialise variables to monitor progress.
		double n(0);								// Number of shapes rasterized,
		double iVecSize = vecPPoly.size();			// total number of shapes,
		clock_t begin_time = clock();				// time started,
		int iPrevTime(0);							// the amount of time we had left last time we checked.
	
		// Rasterise the shapes and write the pixels to our canvas' pixel vector.
		for(auto ppShape = this->vecPPoly.begin(); ppShape != this->vecPPoly.end(); ppShape++){					// For every shape on the canvas,
			auto pShape = *ppShape;
			vector<Line> vecShapeLines = pShape->getLines();													// find the lines that make the shape.
			for(auto pLine = vecShapeLines.begin(); pLine != vecShapeLines.end(); pLine++){						// For every line,
				vector<Pixel> vecRasterPixels = pLine->getRaster();												// rasterise the line.
				for(auto pPixel = vecRasterPixels.begin(); pPixel != vecRasterPixels.end(); pPixel++){			// For every point on the rasterised line,
					Coordinate coordVertex = pPixel->getPos();		
					int iPixelPos = int (coordVertex(1) + ((imgHeight - 1 - coordVertex(2))*imgWidth));			// flip its y-axis, and,
					if (!(iPixelPos >= imgHeight*imgWidth || iPixelPos < 0)){									// if it lies within the bounds of the image,
						vecPixelData[iPixelPos] = vecPixelData[iPixelPos] + pPixel->getColour();				// set the pixel colour (addition defined as 'darkest wins')
					}
				}
			}

			// Chrono
			n++;
			int iCurTime = int ((iVecSize-n)*(clock()-begin_time)/(CLOCKS_PER_SEC*n) + 0.5);
			if (iCurTime != iPrevTime) {
				cout << " - Estimated time remaining: " << iCurTime << " seconds. \r" << flush;					// Carriage return, \r, moves the cursor back to the start of the line so that the text can be overwritten.
				iPrevTime = iCurTime;																			// "flush" sends the output to the console, much like "endl", but without making a new line.
			}

		}
		cout << endl << "Image drawn in " << int((clock()-begin_time)/CLOCKS_PER_SEC + 0.5) << " seconds." << endl;
		Image tempImage(imgWidth, imgHeight, vecPixelData);
		cout << "Writing image..." << endl;
		ofstream imgFile;
		imgFile.open(cFilename, ios::binary);		// Open the file with in binary mode.
		tempImage.write(imgFile);
		cout << "Image written." << endl;
		this->move(coordMin(1)-1,coordMin(2)-1);	// Move all the shapes back and remove padding.
	}
}

// END OF FILE
