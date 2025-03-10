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

class Lamertian : public Material {
public:
    Color albedo;

    Lamertian(const Color& albedo) : albedo(albedo) {}

    bool scatter(const Ray& r, const HitRecord& hr, Color& attenuation, 
        Ray& scattered) const;

};

class Metal : public Material {
public:
    Color albedo;
    float fuzz;
    
    Metal(const Color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz) {}
    bool scatter(const Ray& r, const HitRecord& hr, Color& attenuation, 
        Ray& scattered) const;
};

#endif /* material.h */