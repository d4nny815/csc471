#include "geometry.h"

//*============================================================================
//* Sphere
//*============================================================================

bool Sphere::hit(const Ray& r, Interval t, HitRecord& hr) const {
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