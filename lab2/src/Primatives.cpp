#include "Primatives.h"

// =============================================================================
// Vertex
// =============================================================================

Vector Vertex::get_vector(const Vertex& vertex) const {
    return Vector(vertex.x - x, vertex.y - y);
}

BaryCoord Vertex::calc_bary_coords(const Vertex& a, const Vertex& b, 
									const Vertex& c) {

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
// Vector
// =============================================================================

int Vector::dot(const Vector& vec) {
    return i * vec.i + j * vec.j;
}

// cross(v0,v1)= v0​.x⋅v1​.y−v0​.y⋅v1​.x        
int Vector::cross(const Vector& vec) {
    return i * vec.j - j * vec.i;
}


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
// Bounding Box
// =============================================================================

void BoundingBox::calc_box(const std::vector<Vertex>& vertices) {
    for (Vertex v : vertices) {
        if (x_max < v.x) x_max = v.x;
        if (x_min > v.x) x_min = v.x;

        if (y_max < v.y) y_max = v.y;
        if (y_min > v.y) y_min = v.y;
    }

    return;
}