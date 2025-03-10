#ifndef OBJECT_H
#define OBJECT_H

#include "primatives.h"

class material;

class hit_record {
public:
    point3 point;
    vec3 normal;
    float t;
    shared_ptr<material> mat;

};

class hittable {
public:
    virtual ~hittable() = default;

    virtual bool hit(const ray& r, interval t, hit_record& hr) const = 0;
};

class hittable_list : public hittable {
public:
    std::vector<shared_ptr<hittable>> objects;

    hittable_list() {}
    hittable_list(shared_ptr<hittable> object);

    void clear();
    void add(shared_ptr<hittable> object);
    bool hit(const ray& r, interval t, hit_record& hr) const override;

};

#endif /* object.h */