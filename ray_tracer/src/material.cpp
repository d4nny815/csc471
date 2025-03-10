#include "material.h"


bool Lamertian::scatter(const Ray& r, const HitRecord& hr, Color& attenuation, 
    Ray& scattered) const {

    vec3 scatter_dir = hr.normal + random_unit_vector();
    if (scatter_dir.near_zero()) {
        scatter_dir = hr.normal;
    }
    
    scattered = Ray(r.origin, scatter_dir);
    attenuation = albedo;
    
    return true;
}

bool Metal::scatter(const Ray& r, const HitRecord& hr, Color& attenuation, 
    Ray& scattered) const {

    vec3 reflected = reflect(r.dir, hr.normal);
    reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
    scattered = Ray(hr.point, reflected);
    attenuation = albedo;
    return (dot(scattered.dir, hr.normal) > 0);
}
