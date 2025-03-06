#ifndef PRIMATIVES_H
#define PRIMATIVES_H

#include <stdio.h>
#include "util.h"
#include "interval.h"

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
vec3 operator*(const vec3& v, float t);
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

class hit_record {
public:
    point3 point;
    vec3 normal;
    float t;
};

class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval t, hit_record& hr) const = 0;
};

class sphere : public hittable {
public: 
    point3 center;
    float radius;

    sphere() : center(0, 0, 0), radius(1.0) {}
    sphere(const point3& center, double radius) : 
        center(center), radius(std::fmax(0,radius)) {}

    bool hit(const ray& r, interval t, hit_record& hr) const override;
};

class hittable_list : public hittable {
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object);

    void clear();
    void add(shared_ptr<hittable> object);
    bool hit(const ray& r, interval t, hit_record& hr) const override;

};



#endif /* primatives.h */