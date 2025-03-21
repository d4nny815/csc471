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
#define VFOV_DEG            (120.0f)

// raytracer <image_name> <image_width> <SAMPLES> <CHILD_RAYS> <defocus_angle> <focus_dist>
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
    float defocus_ang_deg = argc > 5 ? atof(argv[5]) : 5;
    const float focus_dist = 3;
    
    float look_radius = 2.0f;   // Distance from the camera to the look-at point
    float pitch = 10;           // rotation around X-axis
    float yaw = 60;             // rotation around Y-axis

    const point3 lookat = point3(0, .5, 0);
    point3 cam_pos = point3(
        look_radius * sin(degrees_to_radians(yaw)) * cos(degrees_to_radians(pitch)),  
        look_radius * sin(degrees_to_radians(pitch)) + lookat.y(),             
        look_radius * cos(degrees_to_radians(yaw)) * cos(degrees_to_radians(pitch))  
    );
    // Compute correct up vector
    const vec3 world_up(0, 1, 0);
    const vec3 forward = unit_vector(lookat - cam_pos);
    const vec3 right = unit_vector(cross(world_up, forward));
    const vec3 up = cross(forward, right); 
    
    Camera cam(filename, aspect_ratio, image_width, samples_per_ray, child_rays,
        VFOV_DEG, defocus_ang_deg, focus_dist, cam_pos, lookat, up);

    HittableList world;
    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));
    
    auto base_mat1 = make_shared<Lambertian>(Color(0.3, 0.7, 0.7)); // cyan
    auto base_mat2 = make_shared<Lambertian>(Color(0.2, 0.5, 0.2)); // green

    auto front_mat = make_shared<Metal>(Color(0xd1ff04), 1);        // yellowish
    auto right_mat = make_shared<Lambertian>(Color(0.2, 0.1, 0.8)); // blue
    auto back_mat  = make_shared<Lambertian>(Color(0.8, 0.8, 0.0)); // yellow
    auto left_mat  = make_shared<Metal>(Color(0.7, 0.2, 0.5), .9);  // purple

    float pyramid_scale = 1;
    std::vector<Triangle> triangles = {
        Triangle(point3(-pyramid_scale, 0, -pyramid_scale), 
                point3(-pyramid_scale, 0, pyramid_scale), 
                point3(pyramid_scale, 0, pyramid_scale), base_mat1),
                
        Triangle(point3(pyramid_scale, 0, pyramid_scale), 
                point3(pyramid_scale, 0, -pyramid_scale), 
                point3(-pyramid_scale, 0, -pyramid_scale), base_mat2),
        
        Triangle(point3(-pyramid_scale, 0, pyramid_scale), 
                point3(pyramid_scale, 0, pyramid_scale), 
                point3(0, 3 * pyramid_scale, 0), front_mat), // Front
        
        Triangle(point3(pyramid_scale, 0, pyramid_scale), 
                point3(pyramid_scale, 0, -pyramid_scale), 
                point3(0, 3 * pyramid_scale, 0), right_mat),  // Right
        
        Triangle(point3(pyramid_scale, 0, -pyramid_scale), 
                point3(-pyramid_scale, 0, -pyramid_scale), 
                point3(0, 3 * pyramid_scale, 0), back_mat),  // Back
        
        Triangle(point3(-pyramid_scale, 0, -pyramid_scale), 
                point3(-pyramid_scale, 0, pyramid_scale), 
                point3(0, 3 * pyramid_scale, 0), left_mat) // Left
    };

    std::shared_ptr<Mesh> pyramid = std::make_shared<Mesh>(triangles);
    world.add(pyramid);

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = rand_float();
            point3 center(a + 0.9*rand_float(), 0.2, b + 0.9*rand_float());
            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;
                if (choose_mat < 0.4) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else if (choose_mat < 0.8) {
                    // metal
                    auto albedo = Color::random();
                    auto fuzz = rand_float(0, 1);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<Dielectric>(rand_float(0, 4));
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(4);
    world.add(make_shared<Sphere>(point3(-2.5, 1, 0), 1.0, material1));
    auto material2 = make_shared<Lambertian>(Color(0x113f24));
    world.add(make_shared<Sphere>(point3(2.5, 1, 0), 1.0, material2));
    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(0, 1, -2.5), 1.0, material3));
    auto material4 = make_shared<Metal>(Color(0xff0ff0), 0.5);
    world.add(make_shared<Sphere>(point3(0, 1, 2.5), 1.0, material4));

    // auto material = make_shared<Lambertian>(Color(0x0077ff));
    // auto material = make_shared<Metal>(Color(0x0077ff), 0);
    // auto material = make_shared<Metal>(Color(0x0077ff), 1);
    // auto material = make_shared<Dielectric>(1.5);
    // world.add(make_shared<Sphere>(point3(0, 1, 0), 1.0, material));

    auto start = system_clock::now();
    cam.render(world);
    auto elapsed_sec = duration<double>(system_clock::now() - start);

    printf("Took %.6f secs\n", elapsed_sec.count());

    return 0;
}
