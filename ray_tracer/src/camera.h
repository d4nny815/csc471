#ifndef CAMERA_H
#define CAMERA_H

#include "primatives.h"

class Camera {
public:
    float focal_length;
    float viewport_height;
    float viewport_width;
    point3 pos;
    vec3 v_u, v_v;
    vec3 pixel_du, pixel_dv;
    vec3 viewport_upper_left;
    vec3 pixel00_loc;

    size_t image_width;
    size_t image_height;
    float aspect_ratio;
    size_t samples_per_pixel;
    float scale_per_pixel;

    Camera(float aspect_ratio, size_t image_width, size_t sample_per_pixel);

    ray get_ray(size_t col, size_t row);
    color ray_color(const ray& r, const hittable& world) const;
    vec3 sample_square();

    void render(const hittable& world);

    void write_color(color k);

};

#endif /* camera.h */