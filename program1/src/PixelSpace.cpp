#include "PixelSpace.h"

// =============================================================================
// Pixel
// =============================================================================
PixelTriangle::PixelTriangle(Pixel p0, Pixel p1, Pixel p2) {
    pixels[0] = p0;
    pixels[1] = p1;
    pixels[2] = p2;
}


BaryCoord Pixel::calc_bary_coords(const Pixel& A, const Pixel& B, 
                                const Pixel& C) {
        auto


}


// =============================================================================
// Bounding Box
// =============================================================================

// void BoundingBox::calc_box(const std::vector<Vertex>& vertices) {
    // for (Vertex v : vertices) {
        // if (x_max < v.x) x_max = v.x;
        // if (x_min > v.x) x_min = v.x;
// 
        // if (y_max < v.y) y_max = v.y;
        // if (y_min > v.y) y_min = v.y;
    // }
// 
    // return;
// }

void BoundingBox::calc_box(const std::array<Pixel, 3>& pixels) {
    for (Pixel p : pixels) {
        if (x_max < p.x) x_max = p.x;
        if (x_min > p.x) x_min = p.x;

        if (y_max < p.y) y_max = p.y;
        if (y_min > p.y) y_min = p.y; 
    }
}
