#include "primatives.h"

//*============================================================================
//* vec3
//*============================================================================

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

vec3 vec3::random() {
    return vec3(rand_float(), rand_float(), rand_float());
}

vec3 vec3::random(float min, float max) {
    return vec3(rand_float(min,max), rand_float(min,max), rand_float(min,max));
}

vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.data[0] + v.data[0], u.data[1] + v.data[1], u.data[2] + v.data[2]);
}

vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.data[0] - v.data[0], u.data[1] - v.data[1], u.data[2] - v.data[2]);
}

vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.data[0] * v.data[0], u.data[1] * v.data[1], u.data[2] * v.data[2]);
}

vec3 operator*(float t, const vec3& v) {
    return vec3(t*v.data[0], t*v.data[1], t*v.data[2]);
}

vec3 operator*(const vec3& v, float t) {
    return t * v;
}

vec3 operator/(const vec3& v, float t) {
    return (1/t) * v;
}

float dot(const vec3& u, const vec3& v) {
    return u.data[0] * v.data[0] + 
            u.data[1] * v.data[1] + 
            u.data[2] * v.data[2];  
}

vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.data[1] * v.data[2] - u.data[2] * v.data[1],
            u.data[2] * v.data[0] - u.data[0] * v.data[2],
            u.data[0] * v.data[1] - u.data[1] * v.data[0]);
}

vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

vec3 random_unit_vector() {
    while (true) {
        auto p = vec3::random(-1,1);
        auto lensq = p.length_squared();
        if (1e-20 < lensq && lensq <= 1) return p / sqrt(lensq);
    }
}

vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_Sphere = random_unit_vector();
    if (dot(on_unit_Sphere, normal) > 0.0) {
        return on_unit_Sphere;
    } else {
        return -on_unit_Sphere;
    }
}

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3& r, const vec3& n, const float eta_eta_prime) {
    float cos_theta = fmin(dot(-r, n), 1.0);
    vec3 r_perpendicular = eta_eta_prime * (r + cos_theta * n);
    vec3 r_parallel = -sqrt(fabs(1.0f - r_perpendicular.length_squared())) * n;
    
    return r_perpendicular + r_parallel;
}

//*============================================================================
//* ray
//*============================================================================

vec3 Ray::at(float t) const {
    return origin + t * dir;
}


