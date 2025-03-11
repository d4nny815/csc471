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

    Camera cam(aspect_ratio, image_width, 200, 20);

    HittableList world;
    auto material_ground = make_shared<Lambertian>(Color(.7, .7, .1));
    auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_top = make_shared<Metal>(Color(.8, .8, .8), 0.1);
    auto material_right = make_shared<Metal>(Color(.3, .6, .3), .3);

    world.add(make_shared<Sphere>(point3(0, 0, -1), .5, material_center));
    world.add(make_shared<Sphere>(point3(0, 1, -1), .5, material_top));
    world.add(make_shared<Sphere>(point3(.8, 0, -1), .2, material_right));
    world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100, material_ground));

    cam.render(world);

    return 0;
}
