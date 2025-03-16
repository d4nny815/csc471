#include "object.h"

//*============================================================================
//* HittableList
//*============================================================================

void HittableList::clear() {
    objects.clear();
}

void HittableList::add(shared_ptr<Hittable> object) {
    objects.push_back(object);
}

bool HittableList::hit(const Ray& r, Interval t, HitRecord& hr) const {
    bool hit_anything = false;
    HitRecord tmp_hr;
    float closest_intersect = t.max;

    for (const auto& obj : objects) {
        if (obj->hit(r, Interval(t.min, closest_intersect), tmp_hr)) {
            hit_anything = true;
            closest_intersect = tmp_hr.t;
            hr = tmp_hr;
        }
    }

    return hit_anything;
}