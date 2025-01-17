#include "PixelSpace.h"

// =============================================================================
// Barycentric Coord
// =============================================================================

#define TOLERANCE   (1e-6f)
bool BaryCoord::in_triangle() {
    // return (0 < alpha && alpha < 1) && (0 < beta && beta < 1) &&
            // (0 < gamma && gamma < 1);

    return alpha >= 0 && beta >= 0 && gamma >= 0 &&
           std::abs(1.0f - alpha - beta - gamma) < TOLERANCE;
}


// =============================================================================
// Pixel
// =============================================================================

Pixel::Pixel(Vertex v, PixelTransform pmatrix) {
    x = pmatrix.width_scalar * v.x + pmatrix.width_offset;
    y = pmatrix.height_scalar * v.y + pmatrix.height_offset;
    z = v.z;
    color = v.color;
}

PixelVector Pixel::get_vector(const Pixel& vertex) const {
    return PixelVector(vertex.x - x, vertex.y - y);
}

BaryCoord Pixel::calc_bary_coords(const Pixel& a, const Pixel& b, 
									const Pixel& c) {

    BaryCoord bary;
    int area_abc = a.get_vector(b).cross(a.get_vector(c));
    int area_a = this->get_vector(b).cross(this->get_vector(c));
    int area_b = this->get_vector(c).cross(this->get_vector(a));
    int area_c = this->get_vector(a).cross(this->get_vector(b));

    bary.alpha = static_cast<float>(area_a) / area_abc;
    bary.beta = static_cast<float>(area_b) / area_abc;
    bary.gamma = static_cast<float>(area_c) / area_abc;
    
    return bary;
}


// =============================================================================
// PixelFace
// =============================================================================

PixelFace::PixelFace(Pixel p0, Pixel p1, Pixel p2) {
    pixels[0] = p0;
    pixels[1] = p1;
    pixels[2] = p2;
}


PixelFace::PixelFace(Face f, PixelTransform pmatrix) {
    pixels[0] = Pixel(f.v0, pmatrix);
    pixels[1] = Pixel(f.v1, pmatrix);
    pixels[2] = Pixel(f.v2, pmatrix);
}


// =============================================================================
// PixelVector
// =============================================================================

int PixelVector::dot(const PixelVector& vec) {
    return i * vec.i + j * vec.j;
}

// cross(v0,v1)= v0​.x⋅v1​.y−v0​.y⋅v1​.x        
int PixelVector::cross(const PixelVector& vec) {
    return i * vec.j - j * vec.i;
}


// =============================================================================
// PixelTransform
// =============================================================================

PixelTransform::PixelTransform(int width, int height, ViewVolume vvolume) {
    width_scalar = (width - 1) / (vvolume.right - vvolume.left);
    width_offset = -vvolume.left * width_scalar;
    height_scalar = (height - 1) / (vvolume.top - vvolume.bottom);
    height_offset = -vvolume.bottom * height_scalar;
}


// =============================================================================
// Bounding Box
// =============================================================================

BoundingBox::BoundingBox(const std::array<Pixel, 3> &triangles) {
    this->calc_box(triangles);
}


void BoundingBox::calc_box(const std::array<Pixel, 3>& pixels) {
    for (Pixel p : pixels) {
        if (x_max < p.x) x_max = p.x;
        if (x_min > p.x) x_min = p.x;

        if (y_max < p.y) y_max = p.y;
        if (y_min > p.y) y_min = p.y; 
    }
}
