#include "object.h"

//*============================================================================
//* hittable_list
//*============================================================================

void hittable_list::clear() {
    objects.clear();
}

void hittable_list::add(shared_ptr<hittable> object) {
    objects.push_back(object);
}

bool hittable_list::hit(const ray& r, interval t, hit_record& hr) const {
    bool hit_anything = false;
    hit_record tmp_hr;
    float closest_intersect = t.max;

    for (const auto& obj : objects) {
        if (obj->hit(r, interval(t.min, closest_intersect), tmp_hr)) {
            hit_anything = true;
            closest_intersect = tmp_hr.t;
            hr = tmp_hr;
        }
    }

    return hit_anything;
}