#ifndef MATERIAL_H
#define MATERIAL_H

#include "primatives.h"

class material {
public:
    virtual ~material() = default;

    virtual bool scatter(const ray& r, const hit_record& hr, color& attenuation, 
        ray& scattered) const {
        return false;
    }
};

class diffuse : public material {
public:
    color albedo;

    diffuse(const color& albedo) : albedo(albedo) {}

    bool scatter(const ray& r, const hit_record& hr, color& attenuation, 
        ray& scattered) const;

};

class metal : public material {
public:
    color albedo;
    float fuzz;
    
    metal(const color& albedo, float fuzz) : albedo(albedo), fuzz(fuzz) {}
    bool scatter(const ray& r, const hit_record& hr, color& attenuation, 
        ray& scattered) const;
};

#endif /* material.h */