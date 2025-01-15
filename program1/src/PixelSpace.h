#ifndef PIXELSPACE_H
#define PIXELSPACE_H

#include <climits>
#include <vector>
#include <array>
#include "Primatives.h"
#include "WorldSpace.h"

class Pixel {
public:
    Pixel() : x(0), y(0), z(0), color(Color()) {}
    Pixel(int x, int y, int z, Color color) : x(x), y(y), z(z), color(color) {}
    
    
    int x, y, z;
    Color color;

    // void calc_pixel_vals(int width, int height, ViewVolume vvolume);
    BaryCoord calc_bary_coords(const Pixel& A, const Pixel& B, 
                                const Pixel& C);
    void print() {
        printf("Pixel(%d, %d, %d)\n", x, y, z);
    }

};

class PixelTriangle {
public:
    PixelTriangle();
    PixelTriangle(Pixel p0, Pixel p1, Pixel p2);

    std::array<Pixel, 3> pixels;

    void print() {
        printf("PTriangle(\n");
        pixels[0].print();
        pixels[1].print();
        pixels[2].print();
        printf(")\n\n");
    }
};

class BoundingBox {
public:
    BoundingBox() : x_min(INT_MAX), x_max(INT_MIN), 
                    y_min(INT_MAX), y_max(INT_MIN) {}
    
    int x_min, x_max, y_min, y_max;

    // void calc_box(const std::vector<PixelTriangle>& vertices);
    void calc_box(const std::array<Pixel, 3>& triangles);

    void print() {
        printf("Bbox(x[%d, %d] y[%d, %d])\n", x_min, x_max, y_min, y_max);
    }    
};


#endif