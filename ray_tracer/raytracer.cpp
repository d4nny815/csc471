#include <stdio.h>
#include <iostream>

#include "primatives.h"

const size_t image_width = 600;
const size_t image_height = 300;

int main(void) {
    printf("P3\n%zu %zu\n255\n", image_width, image_height);

    for (size_t j = 0; j < image_height; j++) {
        fprintf(stderr, "\rScanlines remaining: %zu ", (image_height - j));
        for (size_t i = 0; i < image_width; i++) {
            auto pixel_color = color(double(i)/(image_width-1), 
                double(j)/(image_height-1), 0);
            write_color(stdout, pixel_color);
        }
    }
    fprintf(stderr, "\rDone.                 \n");
    
    return 0;
}