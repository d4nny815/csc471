#include "camera.h"



Camera::Camera(float aspect_ratio, size_t image_width, size_t image_height) {
    focal_length = 1.0f;
    viewport_height = 2.0f;
    viewport_width = viewport_height * aspect_ratio;
    pos = point3(0, 0, 0);

    // calc viewport vectors
    v_u = vec3(viewport_width, 0, 0);
    v_v = vec3(0, -viewport_height, 0);

    // calc delta viewport vectors
    pixel_du = v_u / image_width;
    pixel_dv = v_v / image_height;

    // calc upper-left corner of viewport
    viewport_upper_left = pos - vec3(0, 0, focal_length);
    viewport_upper_left = viewport_upper_left - v_u / 2 - v_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_du + pixel_dv);
}

ray Camera::get_ray(size_t col, size_t row) {
    vec3 pixel_center = pixel00_loc + (col * pixel_du) + (row * pixel_dv);
    vec3 ray_dir = pixel_center - pos;
    return ray(pos, ray_dir);
}

color Camera::ray_color(const ray& r, const hittable& world) const {
    hit_record rec;
    if (world.hit(r, interval(0, MY_INFINITY), rec)) {
        return 0.5 * (rec.normal + color(1,1,1));
    }

    vec3 unit_direction = unit_vector(r.dir);
    auto a = 0.5*(unit_direction.y() + 1.0);
    return (1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0);
}

