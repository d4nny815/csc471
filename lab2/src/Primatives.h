#ifndef SHAPE_H_
#define SHAPE_H_

#include <iostream>
#include <array>
#include <vector>
#include <climits>

class Vector;

class Color {
	public:
		Color() : r(100), b(100), g(100) {}
		Color(uint8_t r, uint8_t g, uint8_t b) : r(r), b(b), g(g) {} 
		
		uint8_t r;
		uint8_t b;
		uint8_t g;
};

class BaryCoord {
public:
    BaryCoord() : alpha(0.0), beta(0.0), gamma(0.0) {}
    BaryCoord(float alpha, float beta, float gamma)
        : alpha(alpha), beta(beta), gamma(gamma) {}
    
	float alpha, beta, gamma;
	void print() {
		printf("Bary (%f, %f, %f)\n", alpha, beta, gamma);
	}
};

class Vertex {
	public:
        Vertex() : x(0), y(0), color(Color()) {}
		Vertex(int x, int y, Color c);
		
		int x;
		int y;
		Color color;

		Vector get_vector(const Vertex& vertex) const;
		BaryCoord calc_bary_coords(const Vertex& A, const Vertex& B, 
									const Vertex& C);

};


class Vector {
	public:
		Vector() : i(0), j(0) {} 
		Vector(int i, int j) : i(i), j(j) {} 

		int i;
		int j;

		int dot(const Vector& vec);
		int cross(const Vector& vec);

};
// ? might have to change def to dir and magnitude?
// ? is that a ray?
// ? what is the difference?




class BoundingBox {
	public:
		BoundingBox();
		
		int x_min;
		int x_max;
		int y_min;
		int y_max;

        void calc_box(const std::vector<Vertex>& vertices);
};

#endif