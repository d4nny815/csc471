#ifndef SHAPE_H_
#define SHAPE_H_

#include <iostream>
#include <array>
#include <climits>

class Vertex {
	public:
        Vertex() : x(0), y(0) {}
		Vertex(int x, int y);
		int x;
		int y;

};

class BoundingBox {
	public:
		BoundingBox();
        void calc_box(const std::array<Vertex, 3>& vertices);
		int x_min;
		int x_max;
		int y_min;
		int y_max;

};

#endif