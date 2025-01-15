#ifndef SHAPE_H_
#define SHAPE_H_

#include <iostream>
#include <array>
#include <vector>
// #include <climits>

class Vector;

class Color {
public:
    Color() : r(100), g(100), b(100) {}
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {} 
    
    uint8_t r;
    uint8_t g;
    uint8_t b;

};

class BaryCoord {
public:
    BaryCoord() : alpha(0.0f), beta(0.0f), gamma(0.0f) {}
    BaryCoord(float alpha, float beta, float gamma)
        : alpha(alpha), beta(beta), gamma(gamma) {}
    
    
    float alpha, beta, gamma;
    
    bool in_triangle();
    void print() {
        printf("Bary (%f, %f, %f)\n", alpha, beta, gamma);
    }
};

class Vertex {
public:
    Vertex() : x(0.0f), y(0.0f), z(0.0f), color(Color()) {}
    Vertex(float x, float y, float z, Color c) : x(x), y(y), z(z), color(c) {}
    
    float x , y, z;
    Color color;

    Vector get_vector(const Vertex& vertex) const;
    BaryCoord calc_bary_coords(const Vertex& A, const Vertex& B, 
                                const Vertex& C);
    void print() {
        printf("Vertex(%f, %f, %f)\n", x, y, z);
    }
};


class Face {
public:
    Face() : v0(Vertex()), v1(Vertex()), v2(Vertex()) {}
    Face(Vertex v0, Vertex v1, Vertex v2) : v0(v0), v1(v1), v2(v2) {}

    Vertex v0, v1, v2;

    void print() {
        printf("Face(\n");
        v0.print();
        v1.print();
        v2.print();
        printf(")\n");
    }
};

// ? might have to change def to dir and magnitude?
// ? is that a ray?
// ? what is the difference?
// TODO: change the defn
class Vector {
public:
    Vector() : i(0), j(0) {} 
    Vector(int i, int j) : i(i), j(j) {} 

    int i;
    int j;

    int dot(const Vector& vec);
    int cross(const Vector& vec);

};

// class Pixel {
// public:
//     Pixel() : x(0), y(0) {}
//     Pixel(int x, int y) : x(x), y(y) {}
    
//     int x, y;
// };

// class BoundingBox {
// public:
//     BoundingBox() : x_min(INT_MAX), x_max(INT_MIN), 
//                     y_min(INT_MAX), y_max(INT_MIN) {}
    
//     int x_min, x_max, y_min, y_max;

//     void calc_box(const std::vector<Vertex>& vertices);
// };

#endif