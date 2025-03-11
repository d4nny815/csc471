#include <cstdio>

#include "primatives.h"
#include "camera.h"
#include "material.h"
#include "geometry.h"
#include "interval.h"
#include "object.h"
#include "util.h"

#define SAMPLES_PER_RAY (50)
#define CHILD_RAYS      (10)
#define FOV_DEG         (100.0f)

int main() {
    const size_t image_width = 600;
    const float aspect_ratio = 3.0f / 2.0f;

    const point3 cam_pos = point3(0, 0, 0);
    const point3 lookat = point3(0, 0, -1);
    const point3 up = point3(0, 1, 0);

    Camera cam(aspect_ratio, image_width, SAMPLES_PER_RAY, CHILD_RAYS, FOV_DEG, 
        cam_pos, lookat, up);

    HittableList world;
    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = make_shared<Dielectric>(1.5);
    auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);
    auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), 1.0);

    world.add(make_shared<Sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(point3( 0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<Sphere>(point3( 1.0, 0.0, -1.0), 0.5, material_right));

    cam.render(world);

    return 0;
}
