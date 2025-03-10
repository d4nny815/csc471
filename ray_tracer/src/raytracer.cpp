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

    Camera cam(aspect_ratio, image_width, 10, 5);

    HittableList world;
    auto Material_ground = make_shared<Lamertian>(Color(.7, .7, .1));
    auto Material_center = make_shared<Lamertian>(Color(0.1, 0.2, 0.5));
    auto Material_top = make_shared<Metal>(Color(.8, .8, .8), 0.1);
    auto Material_right = make_shared<Metal>(Color(.3, .6, .3), .3);

    world.add(make_shared<Sphere>(point3(0, 0, -1), .5, Material_center));
    world.add(make_shared<Sphere>(point3(0, 1, -1), .5, Material_top));
    world.add(make_shared<Sphere>(point3(.8, 0, -1), .2, Material_right));
    world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100, Material_ground));

    cam.render(world);

    return 0;
}
