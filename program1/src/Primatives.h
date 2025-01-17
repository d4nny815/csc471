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


class Vertex {
public:
    Vertex() : x(0.0f), y(0.0f), z(0.0f), color(Color()) {}
    Vertex(float x, float y, float z, Color c) : x(x), y(y), z(z), color(c) {}
    
    float x , y, z;
    Color color;

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

#endif