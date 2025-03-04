#include <stdio.h>
#include <iostream>

#include "primatives.h"

#define RED     (color(1, 0, 0))
#define GREEN   (color(0, 1, 0))
#define BLUE    (color(0, 0, 1))
#define WHITE   (color(1, 1, 1))
#define BLACK   (color(0, 0, 0))

bool hit_sphere(const point3& center, double radius, const ray& r) {
    vec3 origin_2_center = center - r.origin;
    float a = dot(r.dir, r.dir);
    float b = -2 * dot(r.dir, origin_2_center);
    float c = dot(origin_2_center, origin_2_center) - radius * radius;
    auto discrimant = b * b - 4 * a * c;
    return discrimant >= 0;
}

color ray_color(const ray& r) {
    if (hit_sphere(vec3(0, 0, -1), .5, r)) {
        return RED;
    }

    vec3 unit_direction = unit_vector(r.dir);
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * WHITE + a * BLUE;
}

int main(void) {
    // setup image
    const size_t image_width = 600;
    const float aspect_ratio = 3.0f / 2.0f;
    size_t image_height = static_cast<size_t>(image_width / aspect_ratio);
    printf("P3\n%zu %zu\n255\n", image_width, image_height);
    
    // setup camera
    float focal_length = 1.0f;
    float viewport_height = 2.0f;
    float viewport_width = viewport_height * 
        (static_cast<float>(image_width) / image_height); 
    point3 cam_pos = point3(0, 0, 0);

    // calc viewport vecs
    vec3 v_u = vec3(viewport_width, 0, 0);
    vec3 v_v = vec3(0, -viewport_height, 0);

    // calc delta viewport vec
    vec3 pixel_du = v_u / image_width;
    vec3 pixel_dv = v_v / image_height;

    // calc upper left corner
    // center of viewport 
    vec3 viewport_upper_left = cam_pos - vec3(0, 0, focal_length); 
    // move to top left
    viewport_upper_left = viewport_upper_left - v_u / 2 - v_v / 2; 
    // move to half of pixel spacing
    vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_du + pixel_dv);


    for (size_t row = 0; row < image_height; row++) {
        fprintf(stderr, "\rScanlines remaining: %zu ", (image_height - row));
        for (size_t col = 0; col < image_width; col++) {
            vec3 pixel_center = pixel00_loc + (col * pixel_du) + (row * pixel_dv);
            vec3 ray_dir = pixel_center - cam_pos;
            ray r(cam_pos, ray_dir);

            color pixel_color = ray_color(r);
            write_color(stdout, pixel_color);
        }
    }
    fprintf(stderr, "\rDone.                 \n");
    
    return 0;
}