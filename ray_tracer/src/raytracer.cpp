#include <stdio.h>
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <chrono>

#include "primatives.h"
#include "util.h"
#include "camera.h"
#include "interval.h"
#include "material.h"

#define RED     (color(1, 0, 0))
#define GREEN   (color(0, 1, 0))
#define BLUE    (color(0, 0, 1))
#define WHITE   (color(1, 1, 1))
#define BLACK   (color(0, 0, 0))

int main() {
    const size_t image_width = 900;
    const float aspect_ratio = 3.0f / 2.0f;

    Camera cam(aspect_ratio, image_width, 20, 10);

    hittable_list world;
    auto material_ground = make_shared<diffuse>(BLUE * .2);
    auto material_center = make_shared<metal>(color(0.1, 0.2, 0.5));
    auto material_top = make_shared<metal>(color(.8, .4, .2));
    auto material_right = make_shared<diffuse>(GREEN * .9);

    world.add(make_shared<sphere>(point3(0, 0, -1), .5, material_center));
    world.add(make_shared<sphere>(point3(0, 1, -1), .5, material_top));
    world.add(make_shared<sphere>(point3(.8, 0, -1), .2, material_right));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));

    cam.render(world);

    return 0;
}
