#ifndef CAMERA_H
#define CAMERA_H

#include "primatives.h"

class Camera {
public:
    float focal_length;
    float viewport_height;
    float viewport_width;
    point3 cam_pos;
    vec3 v_u, v_v;
    vec3 pixel_du, pixel_dv;
    vec3 viewport_upper_left;
    vec3 pixel00_loc;

    Camera(float aspect_ratio, size_t image_width, size_t image_height);

    ray get_ray(size_t col, size_t row);
    color ray_color(const ray& r, const hittable& world) const;
};

#endif /* camera.h */