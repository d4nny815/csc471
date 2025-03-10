#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "primatives.h"
#include "material.h"
#include "object.h"

class Sphere : public Hittable {
    public: 
        point3 center;
        float radius;
        shared_ptr<Material> mat;
    
        Sphere() : center(0, 0, 0), radius(1.0) {}
        Sphere(const point3& center, double radius, shared_ptr<Material> mat) : 
            center(center), radius(std::fmax(0,radius)), mat(mat) {}
    
        bool hit(const Ray& r, Interval t, HitRecord& hr) const override;
    };

#endif /* geometry.h */