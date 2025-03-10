#ifndef OBJECT_H
#define OBJECT_H

#include "primatives.h"

class Material;

class HitRecord {
public:
    point3 point;
    vec3 normal;
    float t;
    shared_ptr<Material> mat;

};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, Interval t, HitRecord& hr) const = 0;
};

class HittableList : public Hittable {
public:
    std::vector<shared_ptr<Hittable>> objects;

    HittableList() {}
    HittableList(shared_ptr<Hittable> object);

    void clear();
    void add(shared_ptr<Hittable> object);
    bool hit(const Ray& r, Interval t, HitRecord& hr) const override;

};

#endif /* object.h */