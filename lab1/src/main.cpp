#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include "Image.h"
#include "Shape.h"

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;


int main(int argc, char **argv)
{
	// if(argc < 10) {
		// cout << "Usage: Lab01 filename width height vax vay vbx vby vcx vcy" << endl;
		// return 0;
	// }
	
	// Output filename
	string filename(argv[1]);
	// Width of image
	int width = atoi(argv[2]);
	// Height of image
	int height = atoi(argv[3]);
	// Create the image. We're using a `shared_ptr`, a C++11 feature.
	auto image = make_shared<Image>(width, height);

	BoundingBox box;

	int vertex_arg = 4;
	std::array<Vertex, 3> vertices; 
	for (Vertex& i : vertices) {
		int x = atoi(argv[vertex_arg++]);
		int y = atoi(argv[vertex_arg++]);
		i = Vertex(x, y);
	}
	box.calc_box(vertices);

	// Draw a rectangle
	for(int y = box.x_min; y < box.x_max; ++y) {
		for(int x = box.y_min; x < box.y_max; ++x) {
			unsigned char r = rand();
			unsigned char g = rand();
			unsigned char b = rand();
			image->setPixel(x, y, r, g, b);
		}
	}
	// Write image to file
	image->writeToFile(filename);
	return 0;
}
