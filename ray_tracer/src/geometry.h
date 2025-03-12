#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
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

class Triangle : public Hittable {
public:
    point3 v[3];
    shared_ptr<Material> mat;

    Triangle(shared_ptr<Material> mat) : 
        v{point3(-1, 0, 0), point3(1, 0, 0), point3(0, 1, 0)}, mat(mat) {}
    Triangle(point3 p0, point3 p1, point3 p2, shared_ptr<Material> mat) : 
        v{p0, p1, p2}, mat(mat) {}

    bool hit(const Ray& r, Interval t, HitRecord& hr) const override;
};

static Interval BARYCENTRIC_INTERVAL = Interval(0, 1);

class Mesh : public Hittable {
    public:
        std::vector<Triangle> faces;
    
        Mesh() = default;
        Mesh(const std::vector<Triangle>& triangles) : faces(triangles) {}
    
        bool hit(const Ray& r, Interval t, HitRecord& hr) const override;
    };


#endif /* geometry.h */