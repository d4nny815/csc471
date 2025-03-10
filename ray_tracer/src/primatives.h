#ifndef PRIMATIVES_H
#define PRIMATIVES_H

#include <stdio.h>
#include "util.h"
#include "interval.h"
#include <cmath>


// * vec3
class vec3 {
public:
    float data[3];

    vec3() : data{0, 0, 0} {};
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

    static vec3 random();
    static vec3 random(float min, float max);

    bool near_zero() const {
        auto s = 1e-8;
        return (std::fabs(data[0]) < s) && (std::fabs(data[1]) < s) && 
            (std::fabs(data[2]) < s);
    }
    
    inline void print(FILE* fp) {
        fprintf(fp, "<%f, %f, %f>\n", data[0], data[1], data[2]);
    }
};

vec3 operator+(const vec3& u, const vec3& v); 
vec3 operator-(const vec3& u, const vec3& v);
vec3 operator*(const vec3& v, const vec3& u);
vec3 operator*(float t, const vec3& v);
vec3 operator*(const vec3& v, float t);
vec3 operator/(const vec3& v, float t);

float dot(const vec3& u, const vec3& v);
vec3 cross(const vec3& u, const vec3& v);
vec3 unit_vector(const vec3& v);
vec3 random_unit_vector();
vec3 random_on_hemiSphere(const vec3& normal);
vec3 reflect(const vec3& v, const vec3& n);

using point3 = vec3; // alias for coords

// * Color
using Color = vec3; // alias for colors
const Color BLACK = Color(0, 0, 0); 
const Color WHITE = Color(1, 1, 1); 
const Color RED   = Color(1, 0, 0); 
const Color GREEN = Color(0, 1, 0); 
const Color BLUE  = Color(0, 0, 1); 

class Ray {
public:
    point3 origin;
    vec3 dir;

    Ray() {}
    Ray(const vec3& origin, const vec3& direction) 
        : origin(origin), dir(direction) {}
    
    point3 at(float t) const;

};


#endif /* primatives.h */