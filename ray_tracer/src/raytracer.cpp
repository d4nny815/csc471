#include <stdio.h>
#include <iostream>

#include "primatives.h"


color ray_color(const ray& r) {return color();};

int main(void) {
    // setup image
    const size_t image_width = 600;
    const float aspect_ratio = 3.0f / 2.0f;
    size_t image_height = static_cast<size_t>(image_width / aspect_ratio);
    printf("P3\n%zu %zu\n255\n", image_width, image_height);
    
    // setup camera
    float focal_length = 1.0f;
    float viewport_height = 2.0f;
    float viewport_width = viewport_height * aspect_ratio; 
    point3 cam_pos = point3(0, 0, 0);

    // calc viewport vecs
    vec3 v_u = vec3(viewport_width, 0, 0);
    vec3 v_v = vec3(0, -viewport_height, 0);

    // calc delta viewport vec
    vec3 pixel_du = v_u / viewport_width;
    // vec3 pixel_dv = v_v / viewport_height;

    // // calc upper left corner
    // // center of viewport 
    // vec3 viewport_upper_left = cam_pos - vec3(0, 0, focal_length); 
    // // move to top left
    // viewport_upper_left = viewport_upper_left - v_u / 2 - v_v / 2; 
    // // move to half of pixel spacing
    // vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_du + pixel_dv);


    // for (size_t j = 0; j < image_height; j++) {
    //     fprintf(stderr, "\rScanlines remaining: %zu ", (image_height - j));
    //     for (size_t i = 0; i < image_width; i++) {
    //         auto pixel_center = pixel00_loc + (i * pixel_du) + (j * pixel_dv);
    //         auto ray_direction = pixel_center - cam_pos;
    //         ray r(cam_pos, ray_direction);

    //         color pixel_color = ray_color(r);
    //         write_color(stdout, pixel_color);
    //     }
    // }
    fprintf(stderr, "\rDone.                 \n");
    
    return 0;
}