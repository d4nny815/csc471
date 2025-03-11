#ifndef MATERIAL_H
#define MATERIAL_H

#include "primatives.h"
#include "object.h"

class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& r, const HitRecord& hr, Color& attenuation, 
        Ray& scattered) const {
        return false;
    }
};

class Lambertian : public Material {
public:
    Color albedo;

    Lambertian(const Color& albedo) : albedo(albedo) {}

    bool scatter(const Ray& r, const HitRecord& hr, Color& attenuation, 
        Ray& scattered) const override;

};

class Metal : public Material {
public:
    Color albedo;
    float fuzz;
    
    Metal(const Color& albedo, float fuzz) : albedo(albedo), 
        fuzz(fuzz < 1 ? fuzz : 1) {}
    bool scatter(const Ray& r, const HitRecord& hr, Color& attenuation, 
        Ray& scattered) const override;
};

class Dielectric : public Material {
public:
    float refractive_index;

    Dielectric(float refractive_index) : refractive_index(refractive_index) {}

    bool scatter(const Ray& r, const HitRecord& hr, Color& attenuation, 
        Ray& scattered) const override;

    static float reflectance(float cosine, float refraction_index);
};


#endif /* material.h */