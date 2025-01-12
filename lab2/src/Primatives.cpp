#include "Primatives.h"

Vertex::Vertex(int x, int y, Color c) {
    this->x = x;
    this->y = y;
    this->color = c;
    return;
}

Vector Vertex::get_vector(const Vertex& vertex) const {
    return Vector(vertex.x - this->x, vertex.y - this->y);
}

int Vector::dot(const Vector& vec) {
    return this->i * vec.i + this->j * vec.j;
}

// cross(v0,v1)= v0​.x⋅v1​.y−v0​.y⋅v1​.x        
int Vector::cross(const Vector& vec) {
    return this->i * vec.j - this->j * vec.i;
}

BaryCoord Vertex::calc_bary_coords(const Vertex& a, const Vertex& b, 
									const Vertex& c) {

    BaryCoord bary;

    int area_abc = a.get_vector(b).cross(a.get_vector(c));
    bary.alpha = (float)(this->get_vector(b).cross(this->get_vector(c))) / 
                area_abc;
    bary.beta = (float)(this->get_vector(c).cross(this->get_vector(a))) / 
                area_abc;
    bary.gamma = 1.0 - bary.alpha - bary.beta;
    // bary.gamma = this->get_vector(a).cross(this->get_vector(b)) / area_abc;

    return bary;
}


BoundingBox::BoundingBox() {
    this->x_min = INT_MAX;
    this->x_max = INT_MIN;
    this->y_min = INT_MAX;
    this->y_max = INT_MIN;
    return;
}

void BoundingBox::calc_box(const std::vector<Vertex>& vertices) {
    for (Vertex v : vertices) {
        if (this->x_max < v.x) this->x_max = v.x;
        if (this->x_min > v.x) this->x_min = v.x;

        if (this->y_max < v.y) this->y_max = v.y;
        if (this->y_min > v.y) this->y_min = v.y;
    }

    return;
}