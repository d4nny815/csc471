#ifndef PIXELSPACE_H
#define PIXELSPACE_H

#include <climits>
#include <vector>
#include <array>
#include "Primatives.h"
#include "WorldSpace.h"

class PixelVector;
class PixelTransform;

class BaryCoord {
public:
    BaryCoord() : alpha(0.0f), beta(0.0f), gamma(0.0f) {}
    BaryCoord(float alpha, float beta, float gamma)
        : alpha(alpha), beta(beta), gamma(gamma) {}

    float alpha, beta, gamma;

    bool in_triangle();
    void print()
    {
        printf("Bary (%f, %f, %f)\n", alpha, beta, gamma);
    }
};

class Pixel {
public:
    Pixel() : x(0), y(0), z(0.0f), color(Color()) {}
    Pixel(int x, int y, float z, Color color) : x(x), y(y), z(z), color(color) {}
    Pixel(Vertex v, PixelTransform pmatrix);

    int x, y;
    float z;
    Color color;

    PixelVector get_vector(const Pixel &vertex) const;
    BaryCoord calc_bary_coords(const Pixel &A, const Pixel &B,
                               const Pixel &C);
    void print() {
        printf("Pixel(%d, %d, %f)\n", x, y, z);
        color.print();
    }
};

class PixelFace {
public:
    PixelFace();
    PixelFace(Pixel p0, Pixel p1, Pixel p2);
    PixelFace(Face f, PixelTransform pmatrix);

    std::array<Pixel, 3> pixels;

    void print()
    {
        printf("PFace(\n");
        pixels[0].print();
        pixels[1].print();  
        pixels[2].print();
        printf(")\n\n");
    }
};

class PixelVector {
public:
    PixelVector() : i(0), j(0) {}
    PixelVector(int i, int j) : i(i), j(j) {}

    int i;
    int j;

    int dot(const PixelVector &vec);
    int cross(const PixelVector &vec);
};

class PixelTransform {
public:
    PixelTransform(int width, int height, ViewVolume vvolume);

    float width_scalar, width_offset, height_scalar, height_offset;

    void print() {
        printf("C: %f, D: %f, E: %f, F: %f\n", width_scalar, width_offset,
                height_scalar, height_offset);
    }
};

class BoundingBox {
public:
    BoundingBox() : x_min(INT_MAX), x_max(INT_MIN),
                    y_min(INT_MAX), y_max(INT_MIN) {}
    BoundingBox(const std::array<Pixel, 3> &triangles);
    int x_min, x_max, y_min, y_max;

    // void calc_box(const std::vector<PixelTriangle>& vertices);
    void calc_box(const std::array<Pixel, 3> &triangles);

    void print()
    {
        printf("Bbox(x[%d, %d] y[%d, %d])\n", x_min, x_max, y_min, y_max);
    }
};

#endif