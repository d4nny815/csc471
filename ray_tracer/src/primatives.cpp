#include "primatives.h"

//*============================================================================
//* vec3
//*============================================================================


vec3 vec3::random() {
    return vec3(rand_float(), rand_float(), rand_float());
}

vec3 vec3::random(float min, float max) {
    return vec3(rand_float(min,max), rand_float(min,max), rand_float(min,max));
}

vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_Sphere = random_unit_vector();
    if (dot(on_unit_Sphere, normal) > 0.0) {
        return on_unit_Sphere;
    } else {
        return -on_unit_Sphere;
    }
}

vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

//*============================================================================
//* ray
//*============================================================================

vec3 Ray::at(float t) const {
    return origin + t * dir;
}


