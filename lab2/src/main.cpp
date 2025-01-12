// ./Lab02 out.png 200 200 2 2 50 60 150 170

#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <memory>
#include <random>
#include "Image.h"
#include "Primatives.h"

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

	srand(time(0)); // set the seed to cur time so rand colors

	BoundingBox bbox;

	int vertex_arg = 4;
	std::vector<Vertex> vertices; 
	for (int i = 0; i < 3; i++) {
		int x = atoi(argv[vertex_arg++]);
		int y = atoi(argv[vertex_arg++]);
		Color c = Color(rand() % 255, rand() % 255, rand() % 255);
		vertices.push_back(Vertex(x, y, c));
	}
	bbox.calc_box(vertices);
	
	// draw bounding box
	for(int y = bbox.y_min; y <= bbox.y_max; ++y) {
		for(int x = bbox.x_min; x <= bbox.x_max; ++x) {
			// unsigned char r = 255;
			// unsigned char g = 255;
			// unsigned char b = 255;

			// Vertex p = Vertex(x, y, Color());

			// image->setPixel(x, y, r, g, b);
		}
	}

	for (const Vertex& v : vertices) {
		image->setPixel(v.x, v.y, v.color.r, v.color.g, v.color.b);
	}

	// Write image to file
	image->writeToFile(filename);
	return 0;
}

