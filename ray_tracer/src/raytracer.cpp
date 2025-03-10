#include <cstdio>

#include "primatives.h"
#include "camera.h"
#include "material.h"
#include "geometry.h"
#include "interval.h"
#include "object.h"
#include "util.h"

int main() {
    const size_t image_width = 900;
    const float aspect_ratio = 3.0f / 2.0f;

    Camera cam(aspect_ratio, image_width, 10, 20);

    hittable_list world;
    auto material_ground = make_shared<diffuse>(color(.7, .7, .1));
    auto material_center = make_shared<diffuse>(color(0.1, 0.2, 0.5));
    auto material_top = make_shared<metal>(color(.8, .8, .8), 0.1);
    auto material_right = make_shared<metal>(color(.3, .6, .3), .3);

    world.add(make_shared<sphere>(point3(0, 0, -1), .5, material_center));
    world.add(make_shared<sphere>(point3(0, 1, -1), .5, material_top));
    world.add(make_shared<sphere>(point3(.8, 0, -1), .2, material_right));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100, material_ground));

    cam.render(world);

    return 0;
}
