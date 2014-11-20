/*

		Oliver Blanthorn's Polygon Project
		"Programming in C++"
		3rd Year Undergraduate, University of Manchester
		Student ID: 79449310

*/


#include <iostream>
#include <exception>
#include <fstream>
#include <vector>
#include "polylib.h"

using namespace std;
const double PI = atan(double(1)) * 4;

int main(){
	try {

		// PREAMBLE - Very important to run without debugging. Stress test will take about 2 hours if it is enabled, compared to 30 seconds without.
		cout << "Welcome to Oliver Blanthorn's C++ Programming Project: Polygons." << endl;
		cout << "This programme is an example of the things that can be done with the polygon"
			<< endl << "library I have created." << endl << endl;
		cout << "Please ensure that you are not running any debuggers before continuing. If" << endl
			<< "you're compiling in Visual Studio, ensure that you have selected \"Release\"" << endl
			<< "rather than \"Debug\", and that you are running without debugging." << endl;
		cin.get();

		// THE EXAMPLES
		// Draws a square and writes it to file.
		cout << endl << "We start out simply. We draw a 200 x 200 pixel square and write it to a file called \"ex1-square.bmp\"." << endl;
		cin.get();
		Square myFirstSquare(200,Coordinate(0,0));		// Creates a Square centred on the origin, 200px x 200px. (Square lives in shapes.h, with all the other shapes.)
		Canvas myCanvas;								// Creates a Canvas. Canvases are used to group shapes together for manipulation and rasterisation.
		myCanvas.linkShape(&myFirstSquare);				// Links the shape to the Canvas.
		myCanvas.write("ex1-square.bmp");				// Writes the square to file.
		


		// Rotate the square and write it to another file.
		cout << endl << "We'll now rotate the square using matrix multiplication and write it to a file called \"ex2-square-rot.bmp\"." << endl;
		cin.get();
		myFirstSquare.rotate(1);						// Here, the Square is rotated directly. We could have equally called the Canvas' method; the outcome would have been the same.
		myCanvas.write("ex2-square-rot.bmp");			// Writes the Canvas to file.



		// Draws a few more shapes, storing them in a vector container before putting them on the canvas.
		cout << endl << "We'll now draw a triangle, a hexagon and a rectangle on a new image and write it to a file called \"ex3-shapes.bmp\"." << endl;
		cin.get();
		vector<PolygonBase*> ppoly;												// We store the pointers to the shapes here, temporarily. If the shapes to be written are created at run-time, this would be a good way to store them.
																				// ppoly is reused in STRESS TEST below.
		ppoly.push_back(new IsoscelesTriangle(200,400,Coordinate(200,200)));	// We make a new IsoscelesTriangle,
		ppoly.push_back(new Hexagon(300,Coordinate(100,-100)));					// a Hexagon,
		ppoly.push_back(new Rectangle(240,80,Coordinate(0,0)));					// a Rectangle,
		myCanvas.setShapes(ppoly);												// and replace our Canvas with these three shapes.
		myCanvas.write("ex3-shapes.bmp");										// The file is then written to disk.
		


		// Demonstrates transformation of entire canvases
		cout << endl << "We'll now reflect our previous image about the line y=2x and write it to \"ex4-shapes-flipped.bmp\"." << endl;
		cin.get();
		myCanvas.reflect(2,1);						// Flips the canvas about the line y = 2x,
		myCanvas.write("ex4-shapes-flipped.bmp");	// and writes it to file.



		// Demonstrates manipulation of individual shapes after they have been added to a Canvas.
		cout << endl << "We'll now shear the hexagon from previous image and write it to \"ex5-shapes-sheared.bmp\"." << endl;
		cin.get();
		ppoly[1]->shear(0.5,-0.1);					// Shears the 2nd item in ppoly (a Hexagon)
		myCanvas.write("ex5-shapes-sheared.bmp");	// and writes it to file.


		// Cleans up the mess we've made.
		ppoly.clear();
		myCanvas.clear();


		// Demonstrates construction of polygon from vertices
		cout << endl << "We'll now draw a polygon by specifying its vertices manually, and write it to \"ex6-custom-polygon.bmp\"." << endl;
		cin.get();
		vector<Coordinate> myCustomPolygonVertices;
		myCustomPolygonVertices.push_back(Coordinate(0,0));
		myCustomPolygonVertices.push_back(Coordinate(200,0));
		myCustomPolygonVertices.push_back(Coordinate(200,100));
		myCustomPolygonVertices.push_back(Coordinate(100,200));
		myCustomPolygonVertices.push_back(Coordinate(0,100));
		PolygonDynamic myCustomPolygon(myCustomPolygonVertices);
		myCanvas.linkShape(&myCustomPolygon);
		myCanvas.write("ex6-custom-polygon.bmp");

		// Demonstrates addition of vertex to polygon after construction, and detection of intersection.
		cout << endl << "We'll now add a vertex to that polygon, and see if it has caused any lines to intersect, and write it to \"ex7-custom-polygon-overlap.bmp\"." << endl;
		cin.get();
		if(!myCustomPolygon.addVertex(Coordinate(200,200))) {cout << "Extra vertex has caused intersection." << endl;}
		myCanvas.write("ex7-custom-polygon-overlap.bmp");



		// STRESS TEST
		// Preamble... don't run this with debugging turned on...
		cout << endl << "I'd now like to run a stress test. It will use about 200MB of RAM and store a 200MB image on your hard drive." << endl;
		cout << "If this isn't OK, exit the programme now." << endl;
		cin.get();

		cout << "Continuing with stress test." << endl;
		cout << "We'll now write a 8194 x 8194 image to your hard drive. It will contain 972 pentagons." << endl;
		cout << "It will be called \"ex8-finale.bmp\"." << endl;
		cout << "Starting stress test..." << endl << endl;
		cout << "This should take no longer than a minute or so, depending on the speed of your computer." << endl;
		cout << "If it is taking much longer, double check that you are not running any debuggers." << endl;




		// Set up variables
		const int SHAPES = 81*3*4;				// The total number of shapes we draw.
		const double ROTATIONS_PER_CYCLE = 12;	// How many times the shapes rotate as you move around the image.
		const int SIZE = 200;					// The initial size of the pentagons.
		const double OFFSET = 1;				// Percentage (1 = 100%) offset of centre of polygons from centre of image.
		const int RESOLUTION = 8192;			// x,y dimension of final square image. Excludes padding.

		for(int i = 0; i < SHAPES; i++){																						// For every shape we wish to draw,
			auto pShape = new RegularPolygon(
				double(SIZE),				// Size of polygon
				5,							// number of sides
				Coordinate(					// position of centre of polygon
					Coordinate(0,0),			// Start at the origin,
					double(SIZE*OFFSET),		// move this distance,
					2*PI*i/(SHAPES)				// in this direction
				)
			);																													// create it at the position required,
			pShape->rotate(double(i)*2*PI/(double(SHAPES/ROTATIONS_PER_CYCLE)));												// rotate it,
			ppoly.push_back(pShape);																							// and push it back to our storage.
		};

		myCanvas.setShapes(ppoly);					// Link the shapes to a canvas,
		myCanvas.resize(RESOLUTION,RESOLUTION);		// resize the canvas to our desired resolution,
		myCanvas.write("ex8-finale.bmp");			// and write it to file. Canvas uses <ctime> to produce an estimate of the time it will take.



		cout << endl << "Finished! I hope you enjoyed this programme. Be sure to look at the images it has produced. Press any key to exit." << endl;

	} catch (exception &e) {
		cerr << "FATAL ERROR: Exception encountered: " << e.what() << ". Cannot proceed, exiting program." << endl;
		cin.get();
		exit(1);
	}
	return 0;
}
