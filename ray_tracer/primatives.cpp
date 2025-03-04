#include "primatives.h"
#include <cmath>

//=============================================================================
// vec3
//=============================================================================

vec3 vec3::operator-() const {
    return vec3(-data[0], -data[1], -data[2]);
}

float vec3::operator[](int i) const {
    return data[i];
}

float& vec3::operator[](int i) {
    return data[i];
}

vec3& vec3::operator+=(const vec3& v) {
    data[0] += v.data[0];
    data[1] += v.data[1];
    data[2] += v.data[2];
    return *this;
}

vec3& vec3::operator*=(float t) {
    data[0] *= t;
    data[1] *= t;
    data[2] *= t;
    return *this;
}

vec3& vec3::operator/=(float t) {
    data[0] /= t;
    data[1] /= t;
    data[2] /= t;
    return *this;
}

float vec3::length() const {
    return std::sqrt(length_squared());
}

float vec3::length_squared() const {
    return data[0] * data[0] + data[1] * data[1] + data[2] * data[2]; 
}

inline vec3 vec3::operator+(const vec3& u) const {
    return vec3(data[0] + u.data[0], data[1] + u.data[1], data[2] + u.data[2]);
}

inline vec3 vec3::operator-(const vec3& u) const {
    return vec3(data[0] - u.data[0], data[1] - u.data[1], data[2] - u.data[2]);
}

inline vec3 vec3::operator*(const vec3& u) const {
    return vec3(data[0] * u.data[0], data[1] * u.data[1], data[2] * u.data[2]);
}

inline vec3 vec3::operator*(double t) const {
    return vec3(t * data[0], t * data[1], t * data[2]);
}

inline vec3 vec3::operator/(double t) const {
    return vec3(1 / t * data[0], 1 / t * data[1], 1 / t * data[2]);
}

inline float vec3::dot(const vec3& u, const vec3& v) {
    return u.data[0] * v.data[0] + 
            u.data[1] * v.data[1] + 
            u.data[2] * v.data[2];  
}

inline vec3 vec3::cross(const vec3& u, const vec3& v) {
    return vec3(u.data[1] * v.data[2] - u.data[2] * v.data[1],
            u.data[2] * v.data[0] - u.data[0] * v.data[2],
            u.data[0] * v.data[1] - u.data[1] * v.data[0]);
}

inline vec3 vec3::unit_vector(const vec3& v) {
    return v / v.length();
}

//=============================================================================
// color
//=============================================================================

void write_color(FILE* fp, const color& c) {
    auto r = c.x();
    auto g = c.y();
    auto b = c.z();

    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);
    
    fprintf(fp, "%d %d %d\n", rbyte, gbyte, bbyte);  
}

//=============================================================================
// blank
//=============================================================================








