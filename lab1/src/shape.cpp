#include "Shape.h"

Vertex::Vertex(int x, int y) {
    this->x = x;
    this->y = y;
    return;
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
        // printf("Vertex (%d, %d)\n", v.x, v.y);
    }

    return;
}