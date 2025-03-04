#ifndef PRIMATIVES_H
#define PRIMATIVES_H

#include <stdio.h>

// * vec3
class vec3 {
public:
    float data[3];

    vec3() : data{0, 0, 0} {}; // default constructor
    vec3(float a, float b, float c) : data{a, b, c} {};

    // getters
    float x() const { return data[0]; }
    float y() const { return data[1]; }
    float z() const { return data[2]; }

    // operator overloading
    vec3 operator-() const;
    float operator[](int i) const;
    float& operator[](int i);
    vec3& operator+=(const vec3& v);
    vec3& operator*=(float t);
    vec3& operator/=(float t);
    float length() const;
    float length_squared() const;
    
    inline void print(FILE* fp) {
        fprintf(fp, "<%f, %f, %f>\n", data[0], data[1], data[2]);
    }
};

vec3 operator+(const vec3& u, const vec3& v); 
vec3 operator-(const vec3& u, const vec3& v);
vec3 operator*(float t, const vec3& v);
vec3 operator/(const vec3& v, float t);
float dot(const vec3& u, const vec3& v);
vec3 cross(const vec3& u, const vec3& v);
vec3 unit_vector(const vec3& v);

using point3 = vec3; // alias for coords

// * Color
using color = vec3; // alias for colors
void write_color(FILE* fp, const color& c);

class ray {
public:
    point3 origin;
    vec3 dir;

    ray() {}
    ray(const vec3& origin, const vec3& direction) 
        : origin(origin), dir(direction) {}
    
    point3 at(float t) const;

};

#endif /* primatives.h */