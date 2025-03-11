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

#define DEF_IMAGE_WIDTH     (300)
#define DEF_SAMPLES_PER_RAY (20)
#define DEF_CHILD_RAYS      (5)
#define VFOV_DEG            (50.0f)

// raytracer <image_name> <image_width> <SAMPLES> <CHILD_RAYS>
int main(int argc, char** argv) {
    const float aspect_ratio = 3.0f / 2.0f;
    
    if (argc < 2) {
        fprintf(stderr, "raytracer <image_name>.ppm\n");
        return 0;
    }
    char* filename = argv[1];

    size_t image_width = argc > 2 ? atoi(argv[2]) : DEF_IMAGE_WIDTH;
    size_t samples_per_ray = argc > 3 ? atoi(argv[3]) : DEF_SAMPLES_PER_RAY;
    size_t child_rays = argc > 4 ? atoi(argv[4]) : DEF_CHILD_RAYS;
    
    const point3 cam_pos = point3(-2.5, .15, 3);
    const point3 lookat = point3(0, 0, 0);
    const point3 up = point3(0, 1, 0);
    const float defocus_ang_deg = -5.0f;
    const float focus_dist = 1.5;

    Camera cam(filename, aspect_ratio, image_width, samples_per_ray, child_rays,
        VFOV_DEG, defocus_ang_deg, focus_dist, cam_pos, lookat, up);

    HittableList world;
    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = rand_float();
            point3 center(a + 0.9*rand_float(), 0.2, b + 0.9*rand_float());
            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;
                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1);
                    auto fuzz = rand_float(0, 1);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(-2.5, 1, 0), 1.0, material1));
    auto material2 = make_shared<Lambertian>(Color(0xea3f24));
    world.add(make_shared<Sphere>(point3(3, 1, 0), 1.0, material2));
    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material3));

    auto start = system_clock::now();
    cam.render(world);
    auto elapsed_sec = duration<double>(system_clock::now() - start);

    printf("Took %.6f secs\n", elapsed_sec.count());

    return 0;
}
