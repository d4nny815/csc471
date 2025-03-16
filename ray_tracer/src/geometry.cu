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

//*============================================================================
//* Triangle
//*============================================================================

// https://www.scratchapixel.com/lessons/3d-basic-rendering/ray-tracing-rendering-a-triangle/moller-trumbore-ray-triangle-intersection.html
#define EPISLON (1e-8)
bool Triangle::hit(const Ray& r, Interval t, HitRecord& hr) const {
    vec3 edge1 = v[1] - v[0];
    vec3 edge2 = v[2] - v[0];

    vec3 P = cross(r.dir, edge2);
    float det = dot(edge1, P);

    // if determinant is close to zero, the ray is parallel to the triangle
    if (fabs(det) < EPISLON) return false;
    if (det < 0) return false; // ignore back-facing triangles 
    
    // compute beta
    float inv_det = 1.0 / det;
    vec3 T = r.origin - v[0];
    float beta = inv_det * dot(P, T);
    if (!BARYCENTRIC_INTERVAL.contains(beta)) return false;
    
    // compute alpha and gamma
    vec3 Q = cross(T, edge1);
    float gamma = inv_det * dot(Q, r.dir);
    if (!BARYCENTRIC_INTERVAL.contains(gamma)) return false;
    float alpha = 1 - beta - gamma;
    if (!BARYCENTRIC_INTERVAL.contains(alpha)) return false;

    // compute the intersection point
    float t_hit = inv_det * dot(Q, edge2);
    if (!t.contains(t_hit)) return false;

    hr.t = t_hit;
    hr.point = r.at(t_hit);
    hr.mat = mat;
    hr.normal = unit_vector(cross(edge1, edge2));

    return true;
}

//*============================================================================
//* Mesh
//*============================================================================

bool Mesh::hit(const Ray& r, Interval t, HitRecord& hr) const {
    bool hit_anything = false;
    HitRecord temp_hr;
    float closest_t = t.max;

    for (const auto& face : faces) {
        if (face.hit(r, Interval(t.min, closest_t), temp_hr)) {
            hit_anything = true;
            closest_t = temp_hr.t;
            hr = temp_hr;
        }
    }

    return hit_anything;
}

