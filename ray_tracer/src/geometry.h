#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "primatives.h"
#include "material.h"
#include "object.h"

class sphere : public hittable {
    public: 
        point3 center;
        float radius;
        shared_ptr<material> mat;
    
        sphere() : center(0, 0, 0), radius(1.0) {}
        sphere(const point3& center, double radius, shared_ptr<material> mat) : 
            center(center), radius(std::fmax(0,radius)), mat(mat) {}
    
        bool hit(const ray& r, interval t, hit_record& hr) const override;
    };

#endif /* geometry.h */