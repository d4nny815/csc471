#ifndef SHAPE_H_
#define SHAPE_H_

#include <iostream>
#include <array>
#include <vector>
#include <climits>

class Color {
	public:
		Color() : r(100), b(100), g(100) {}
		Color(uint8_t r, uint8_t g, uint8_t b) : r(r), b(b), g(g) {} 
		uint8_t r;
		uint8_t b;
		uint8_t g;
};

class Vertex {
	public:
        Vertex() : x(0), y(0), color(Color()) {}
		Vertex(int x, int y, Color c);
		int x;
		int y;
		Color color;
};

class BoundingBox {
	public:
		BoundingBox();
        void calc_box(const std::vector<Vertex>& vertices);
		int x_min;
		int x_max;
		int y_min;
		int y_max;

};

#endif