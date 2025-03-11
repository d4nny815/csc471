#include <cstdio>
#include <chrono>

using namespace std::chrono;
using namespace std::chrono::_V2;

#include "primatives.h"
#include "camera.h"
#include "material.h"
#include "geometry.h"
#include "interval.h"
#include "object.h"
#include "util.h"

#define SAMPLES_PER_RAY (500)
#define CHILD_RAYS      (100)
#define VFOV_DEG        (90.0f)

int main(int argc, char** argv) {
    const size_t image_width = 1800;
    const float aspect_ratio = 3.0f / 2.0f;

    const point3 cam_pos = point3(-1,1,0);
    const point3 lookat = point3(0, 0, -1);
    const point3 up = point3(0, 1, 0);
    const float defocus_ang_deg = 5.0f;
    const float focus_dist = 1.5;

    Camera cam(aspect_ratio, image_width, SAMPLES_PER_RAY, CHILD_RAYS, VFOV_DEG, 
        defocus_ang_deg, focus_dist, cam_pos, lookat, up);

    HittableList world;
    auto material_ground = make_shared<Lambertian>(Color(0.8, 0.8, 0.0));
    auto material_center = make_shared<Lambertian>(Color(0.1, 0.2, 0.5));
    auto material_left = make_shared<Dielectric>(1.5);
    auto material_bubble = make_shared<Dielectric>(1.00 / 1.50);
    auto material_right = make_shared<Metal>(Color(0.8, 0.6, 0.2), .1);

    world.add(make_shared<Sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<Sphere>(point3( 0.0, 0.0, -1.2), 0.5, material_center));
    world.add(make_shared<Sphere>(point3( 0.0, 2, -4), 2, material_right));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, material_left));
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.4, material_bubble));
    world.add(make_shared<Sphere>(point3( 1.0, 0.0, -1.0), 0.5, material_right));

    auto start = system_clock::now();
    cam.render(world);
    auto elapsed_sec = duration<double>(system_clock::now() - start);

    printf("Took %.6f secs\n", elapsed_sec.count());

    return 0;
}
