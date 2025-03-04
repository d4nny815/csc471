#ifndef PRIMATIVES_H
#define PRIMATIVES_H

#include <stdio.h>

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
    
    inline vec3 operator+(const vec3& u) const;    
    inline vec3 operator-(const vec3& u) const;
    inline vec3 operator*(const vec3& u) const;
    inline vec3 operator*(double t) const;
    inline vec3 operator/(double t) const;
    
    inline float dot(const vec3& u, const vec3& v);
    inline vec3 cross(const vec3& u, const vec3& v);
    inline vec3 unit_vector(const vec3& v);
    
    inline void print(FILE* fp) {
        fprintf(fp, "<%f, %f, %f>\n", data[0], data[1], data[2]);
    }
};

using point3 = vec3; // alias for coords

using color = vec3; // alias for colors
void write_color(FILE* fp, const color& c);

#endif /* primatives.h */