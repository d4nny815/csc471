#ifndef SHAPE_H_
#define SHAPE_H_

#include <iostream>
#include <array>
#include <vector>
// #include <climits>

class Vector;

// ? Should this be here or it more in pixel space
// ? leave it cuz it could be changed to something like
//  Mesh:
//      color
//      texture
//       
class Color {
public:
    Color() : r(0), g(0), b(0) {}
    Color(uint8_t r, uint8_t g, uint8_t b) : r(r), g(g), b(b) {} 
    
    uint8_t r;
    uint8_t g;
    uint8_t b;

    void print() {
        printf("\tColor(%hhu, %hhu, %hhu)\n", r, g, b);
    }

};

// class BaryCoord {
// public:
//     BaryCoord() : alpha(0.0f), beta(0.0f), gamma(0.0f) {}
//     BaryCoord(float alpha, float beta, float gamma)
//         : alpha(alpha), beta(beta), gamma(gamma) {}
    
    
//     float alpha, beta, gamma;
    
//     bool in_triangle();
//     void print() {
//         printf("Bary (%f, %f, %f)\n", alpha, beta, gamma);
//     }
// };

// TODO: change to Vertex3
class Vertex {
public:
    Vertex() : x(0.0f), y(0.0f), z(0.0f), color(Color()) {}
    Vertex(float x, float y, float z, Color c) : x(x), y(y), z(z), color(c) {}
    
    float x , y, z;
    Color color;

    // Vector get_vector(const Vertex& vertex) const;
    // FIXME: i dont think this belongs here
    // BaryCoord calc_bary_coords(const Vertex& A, const Vertex& B, 
                                // const Vertex& C); 
    void print() {
        printf("Vertex(%f, %f, %f)\n", x, y, z);
        color.print();
    }
};


class Face {
public:
    Face() {} // ? lets see if this works
    // Face() : v0(Vertex()), v1(Vertex()), v2(Vertex()) {}
    Face(Vertex v0, Vertex v1, Vertex v2) : v0(v0), v1(v1), v2(v2) {}

    Vertex v0, v1, v2;
    Vertex* vertices[3]; 

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

    // int dot(const Vector& vec);
    // int cross(const Vector& vec);

};

#endif