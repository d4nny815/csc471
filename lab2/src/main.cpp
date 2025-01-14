// ./Lab02 out.png 400 300 2 2 100 60 120 190
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

	BoundingBox bbox;

	int vertex_arg = 4;
	std::vector<Vertex> vertices; 
	std::array<Color, 3> colors = {Color(255, 0, 0), Color(0, 255, 0), 
									Color(0, 0, 255)};
	for (int i = 0; i < 3; i++) {
		int x = atoi(argv[vertex_arg++]);
		int y = atoi(argv[vertex_arg++]);
		vertices.push_back(Vertex(x, y, colors[i]));
	}
	bbox.calc_box(vertices);
	
	// draw bounding box
	for (int y = bbox.y_min; y < bbox.y_max; ++y) {
		for (int x = bbox.x_min; x < bbox.x_max; ++x) {
			Vertex p = Vertex(x, y, Color());

			BaryCoord bary = p.calc_bary_coords(vertices[0], vertices[1], 
												vertices[2]);

			if (bary.in_triangle()) {
				p.color.r = bary.alpha * vertices[0].color.r + 
							bary.beta * vertices[1].color.r +
							bary.gamma * vertices[2].color.r;

				p.color.g = bary.alpha * vertices[0].color.g + 
							bary.beta * vertices[1].color.g +
							bary.gamma * vertices[2].color.g;

				p.color.b = bary.alpha * vertices[0].color.b + 
							bary.beta * vertices[1].color.b +
							bary.gamma * vertices[2].color.b;
				image->setPixel(x, y, p.color.r, p.color.g, p.color.b);
			} else {
				image->setPixel(x, y, 255, 255, 255);
			}
		}
	}

	// Write image to file
	image->writeToFile(filename);
	return 0;
}

