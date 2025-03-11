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
    
    Metal(const Color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz) {}
    bool scatter(const Ray& r, const HitRecord& hr, Color& attenuation, 
        Ray& scattered) const override;
};


#endif /* material.h */