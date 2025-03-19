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
    vec3(uint32_t hexcode) : data{
        static_cast<float>((hexcode >> 16) & 0xff) / 255.0f,
        static_cast<float>((hexcode >> 8) & 0xff) / 255.0f,
        static_cast<float>((hexcode) & 0xff) / 255.0f
    } {};

    // getters
    float x() const { return data[0]; }
    float y() const { return data[1]; }
    float z() const { return data[2]; }

    // operator overloading
    inline vec3 operator-() const {
        return vec3(-data[0], -data[1], -data[2]);
    }
    
    inline float operator[](int i) const {
        return data[i];
    }
    
    inline float& operator[](int i) {
        return data[i];
    }
    
    inline vec3& operator+=(const vec3& v) {
        data[0] += v.data[0];
        data[1] += v.data[1];
        data[2] += v.data[2];
        return *this;
    }
    
    inline vec3& operator*=(float t) {
        data[0] *= t;
        data[1] *= t;
        data[2] *= t;
        return *this;
    }

    inline vec3& operator*=(const vec3& v) {
        data[0] *= v.data[0];
        data[1] *= v.data[1];
        data[2] *= v.data[2];
        return *this;
    }
    
    inline vec3& operator/=(float t) {
        data[0] /= t;
        data[1] /= t;
        data[2] /= t;
        return *this;
    }

    inline float length() const {
        return std::sqrt(length_squared());
    }
    
    inline float length_squared() const {
        return data[0] * data[0] + data[1] * data[1] + data[2] * data[2]; 
    }

    static vec3 random();
    static vec3 random(float min, float max);

    inline bool near_zero() const {
        auto s = 1e-6;
        return (std::fabs(data[0]) < s) && (std::fabs(data[1]) < s) && 
            (std::fabs(data[2]) < s);
    }
    
    inline void print(FILE* fp) {
        fprintf(fp, "<%f, %f, %f>\n", data[0], data[1], data[2]);
    }
};

vec3 random_on_hemisphere(const vec3& normal);

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.data[0] + v.data[0], u.data[1] + v.data[1], u.data[2] + v.data[2]);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.data[0] - v.data[0], u.data[1] - v.data[1], u.data[2] - v.data[2]);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.data[0] * v.data[0], u.data[1] * v.data[1], u.data[2] * v.data[2]);
}

inline vec3 operator*(float t, const vec3& v) {
    return vec3(t*v.data[0], t*v.data[1], t*v.data[2]);
}

inline vec3 operator*(const vec3& v, float t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, float t) {
    return (1/t) * v;
}

inline float dot(const vec3& u, const vec3& v) {
    return u.data[0] * v.data[0] + 
            u.data[1] * v.data[1] + 
            u.data[2] * v.data[2];  
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.data[1] * v.data[2] - u.data[2] * v.data[1],
            u.data[2] * v.data[0] - u.data[0] * v.data[2],
            u.data[0] * v.data[1] - u.data[1] * v.data[0]);
}

vec3 unit_vector(const vec3& v);

inline vec3 random_unit_vector() {
    while (true) {
        auto p = vec3::random(-1,1);
        auto lensq = p.length_squared();
        if (1e-20 < lensq && lensq <= 1) return p / sqrt(lensq);
    }
}


inline vec3 random_in_unit_disk() {
    while (true) {
        auto p = vec3(rand_float(-1,1), rand_float(-1,1), 0);
        if (p.length_squared() < 1) return p;
    }
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

inline vec3 refract(const vec3& r, const vec3& n, const float eta_eta_prime) {
    float cos_theta = fmin(dot(-r, n), 1.0);
    vec3 r_perpendicular = eta_eta_prime * (r + cos_theta * n);
    vec3 r_parallel = -sqrt(fabs(1.0f - r_perpendicular.length_squared())) * n;
    
    return r_perpendicular + r_parallel;
}

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