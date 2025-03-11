#ifndef CAMERA_H
#define CAMERA_H

#include "primatives.h"
#include "material.h"
#include "object.h"

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
    uint32_t* frame_buffer = nullptr;

    size_t image_width;
    size_t image_height;
    float aspect_ratio;
    size_t samples_per_pixel;
    float scale_per_pixel;
    size_t child_rays;

    Camera(float aspect_ratio, size_t image_width, size_t sample_per_pixel,
        size_t child_rays);

    Ray get_ray(size_t col, size_t row);
    Color ray_color(const Ray& r, const size_t depth, const HittableList& world) 
        const;
    inline vec3 sample_square();

    void render(const HittableList& world);

    void write_color(Color k, size_t row, size_t col);
    void write_framebuffer();

};

inline float linear_to_gamma(float linear_comp);

#endif /* camera.h */