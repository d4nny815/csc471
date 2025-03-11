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

/**
 * @brief handles the scattering behavior for Dielectric (transparent/refractive) 
 *          materials. Simulates light passing through or reflecting off a 
 *          transparent surface like glass.
 * @param r incoming ray that hit the surface.
 * @param hr hit record containing information about the hit point, surface normal.
 * @param attenuation color attenuation caused by the material
 * @param scattered the resulting scattered ray
 * @return returns true
 */
bool Dielectric::scatter(const Ray& r, const HitRecord& hr, Color& attenuation, 
    Ray& scattered) const {
        
    // check if the ray hit the surface from the outside
    // since im computing when color instead of geometry
    // if both are in same dir, you get a positive val, and that means its 
    // coming from inside
    bool front_face = dot(r.dir, hr.normal) < 0;
    
    float ri = front_face ? (1.0 / refractive_index) : refractive_index;
    vec3 norm = front_face ? hr.normal : -hr.normal;
    
    vec3 unit_dir = unit_vector(r.dir);
    float cos_theta = fmin(dot(-unit_dir, norm), 1.0);
    float sin_theta = sqrt(1.0 - cos_theta * cos_theta);
    bool total_internal_reflection = ri * sin_theta > 1.0;
    vec3 direction;

    // if (total_internal_reflection || reflectance(cos_theta, ri) > rand_float()) {
    if (total_internal_reflection) {
        direction = reflect(unit_dir, norm);
    } else {
        direction = refract(unit_dir, norm, ri);
    }

    scattered = Ray(hr.point, direction);

    attenuation = WHITE;
    return true;
}

float Dielectric::reflectance(float cosine, float refraction_index) {
    // Use Schlick's approximation for reflectance.
    float r0 = (1 - refraction_index) / (1 + refraction_index);
    r0 = r0 * r0;
    return r0 + (1 - r0) * std::pow((1 - cosine), 5);
}