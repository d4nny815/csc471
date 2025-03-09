#include "material.h"


bool diffuse::scatter(const ray& r, const hit_record& hr, color& attenuation, 
    ray& scattered) const {

    vec3 scatter_dir = hr.normal + random_unit_vector();
    if (scatter_dir.near_zero()) {
        scatter_dir = hr.normal;
    }
    
    scattered = ray(r.origin, scatter_dir);
    attenuation = albedo;
    
    return true;
}

bool metal::scatter(const ray& r, const hit_record& hr, color& attenuation, 
    ray& scattered) const {

    vec3 reflected = reflect(r.dir, hr.normal);
    reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
    scattered = ray(hr.point, reflected);
    attenuation = albedo;
    return (dot(scattered.dir, hr.normal) > 0);
}
