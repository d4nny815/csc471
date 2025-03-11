#include "material.h"

/**
 * @brief handles the scattering behavior for Lambertian (diffuse) materials.
 *          simulates a random light bounce on a rough surface
 * @param r incoming ray that hit the surface
 * @param hr hit record containing information about the hit point, 
 *          surface normal
 * @param attenuation color attenuation caused by the material
 * @param scattered the resulting scattered ray after hitting the surface
 * @return always returns true since Lambertian surfaces always scatter light.
 */
bool Lambertian::scatter(const Ray& r, const HitRecord& hr, Color& attenuation, 
    Ray& scattered) const {

    // calc the scattered ray direction by adding the normal vector and a 
    // random unit vector to simulate diffuse reflection
    vec3 scatter_dir = hr.normal + random_unit_vector();
    if (scatter_dir.near_zero()) {
        // set new dir to the normal if the vectors cancel out
        scatter_dir = hr.normal;
    }

    // new ray with hitpoint as origin 
    scattered = Ray(hr.point, scatter_dir);
    attenuation = albedo;
    
    return true;
}

/**
 * @brief handles the scattering behavior for Metal (reflective) materials.
 *        simulates a light bounce with a reflective surface and some fuzziness.
 * @param r incoming ray that hit the surface
 * @param hr hit record containing information about the hit point, 
 *          surface normal
 * @param attenuation color attenuation caused by the material
 * @param scattered the resulting scattered ray after hitting the surface
 * @return Returns true if the scattered ray reflects, otherwise 
 *          returns false if the reflection is invalid.
 */
bool Metal::scatter(const Ray& r, const HitRecord& hr, Color& attenuation, 
    Ray& scattered) const {

    vec3 reflected = reflect(r.dir, hr.normal);
    // add some fuzziness to the reflection based on a random unit vector 
    // scaled by fuzz.
    reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
    
    scattered = Ray(hr.point, reflected);
    attenuation = albedo;
    
    // check if the scattered ray is in the same hemisphere as the normal
    return (dot(scattered.dir, hr.normal) > 0);
}
