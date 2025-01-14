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

#define MIN_ARG_LEN	(10)

int main(int argc, char **argv)
{
	if(argc < MIN_ARG_LEN) {
		cout << "Usage: Lab01 filename width height vax vay vbx vby vcx vcy" << endl;
		return 0;
	}
	
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
	std::vector<Vertex> vertices; 
	for (int i = 0; i < 3; i++) {
		int x = atoi(argv[vertex_arg++]);
		int y = atoi(argv[vertex_arg++]);
		vertices.push_back(Vertex(x, y));
	}
	box.calc_box(vertices);
	// printf("Box (%d, %d) -> (%d, %d)\n", 
		// box.x_min, box.x_max, box.y_min, box.y_max);
	
	// Draw a rectangle
	for(int y = box.y_min; y <= box.y_max; ++y) {
		for(int x = box.x_min; x <= box.x_max; ++x) {
			unsigned char r = 255;
			unsigned char g = 255;
			unsigned char b = 255;
			image->setPixel(x, y, r, g, b);
		}
	}

	for (Vertex v : vertices) {
		image->setPixel(v.x, v.y, 255, 0, 0);
	}

	// Write image to file
	image->writeToFile(filename);
	return 0;
}
