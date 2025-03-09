#include "primatives.h"
#include <cmath>

//*============================================================================
//* vec3
//*============================================================================

inline vec3 vec3::operator-() const {
    return vec3(-data[0], -data[1], -data[2]);
}

inline float vec3::operator[](int i) const {
    return data[i];
}

inline float& vec3::operator[](int i) {
    return data[i];
}

vec3& vec3::operator+=(const vec3& v) {
    data[0] += v.data[0];
    data[1] += v.data[1];
    data[2] += v.data[2];
    return *this;
}

inline vec3& vec3::operator*=(float t) {
    data[0] *= t;
    data[1] *= t;
    data[2] *= t;
    return *this;
}

inline vec3& vec3::operator/=(float t) {
    data[0] /= t;
    data[1] /= t;
    data[2] /= t;
    return *this;
}

inline float vec3::length() const {
    return std::sqrt(length_squared());
}

inline float vec3::length_squared() const {
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
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) {
        return on_unit_sphere;
    } else {
        return -on_unit_sphere;
    }
}

vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

//*============================================================================
//* color
//*============================================================================

void write_color(FILE* fp, const color& c) {
    auto r = c.x();
    auto g = c.y();
    auto b = c.z();

    int rbyte = int(255.999 * r);
    int gbyte = int(255.999 * g);
    int bbyte = int(255.999 * b);
    
    fprintf(fp, "%d %d %d\n", rbyte, gbyte, bbyte);  
}

//*============================================================================
//* ray
//*============================================================================

vec3 ray::at(float t) const {
    return origin + t * dir;
}

//*============================================================================
//* sphere
//*============================================================================

bool sphere::hit(const ray& r, interval t, hit_record& hr) const {
    vec3 origin_2_center = center - r.origin;
    float a = r.dir.length_squared();
    float h = dot(r.dir, center - r.origin);
    float c = dot(origin_2_center, origin_2_center) - radius * radius;
    float discrimant = h * h - a * c;
    
    if (discrimant < 0) return false;


    float sqrt_disc = std::sqrt(h * h - a * c);
    float root = (h - sqrt_disc) / a; 
    if (!t.contains(root)) {
        root = (h + sqrt_disc) / a;
        if (!t.contains(root)) {
            return false;
        }
    }

    hr.t = root;
    hr.point = r.at(root);
    hr.normal = (hr.point - center) / radius;
    hr.mat = mat;

    return true; 
}

//*============================================================================
//* hittable_list
//*============================================================================

void hittable_list::clear() {
    objects.clear();
}

void hittable_list::add(shared_ptr<hittable> object) {
    objects.push_back(object);
}

bool hittable_list::hit(const ray& r, interval t, hit_record& hr) const {
    bool hit_anything = false;
    hit_record tmp_hr;
    float closest_intersect = t.max;

    for (const auto& obj : objects) {
        if (obj->hit(r, interval(t.min, closest_intersect), tmp_hr)) {
            hit_anything = true;
            closest_intersect = tmp_hr.t;
            hr = tmp_hr;
        }
    }

    return hit_anything;
}










