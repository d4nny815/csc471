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
#define VFOV_DEG            (100.0f)

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
    
    float look_radius = 4.0f;  // Distance from the camera to the look-at point
    float pitch = 60;           // rotation around X-axis
    float yaw = 180;             // rotation around Y-axis

    // Convert pitch & yaw to Cartesian coordinates
    point3 cam_pos = point3(
        look_radius * sin(degrees_to_radians(yaw)) * cos(degrees_to_radians(pitch)),  
        look_radius * sin(degrees_to_radians(pitch)),             
        look_radius * cos(degrees_to_radians(yaw)) * cos(degrees_to_radians(pitch))  
    );

    const point3 lookat = point3(0, 0, -.5);

    // Compute correct up vector
    const vec3 world_up(0, 1, 0);
    const vec3 forward = unit_vector(lookat - cam_pos);
    const vec3 right = unit_vector(cross(world_up, forward));
    const vec3 up = cross(forward, right); // Ensure up is perpendicular
    
    const float defocus_ang_deg = 5.0f;
    const float focus_dist = 3;

    Camera cam(filename, aspect_ratio, image_width, samples_per_ray, child_rays,
        VFOV_DEG, defocus_ang_deg, focus_dist, cam_pos, lookat, up);

    HittableList world;
    
    // Define different colors for each face
    auto base_mat1 = make_shared<Lambertian>(Color(0.3, 0.7, 0.7)); // Cyan
    auto base_mat2 = make_shared<Lambertian>(Color(0.2, 0.5, 0.2)); // Green

    auto front_mat = make_shared<Lambertian>(Color(0.8, 0.1, 0.0)); // Red
    auto right_mat = make_shared<Lambertian>(Color(0.2, 0.1, 0.8)); // Blue
    auto back_mat  = make_shared<Lambertian>(Color(0.8, 0.8, 0.0)); // Yellow
    auto left_mat  = make_shared<Metal>(Color(0.7, 0.2, 0.5), .9); // Purple

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

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(point3(-2.5, 1, 0), 1.0, material1));
    auto material2 = make_shared<Lambertian>(Color(0xea3f24));
    world.add(make_shared<Sphere>(point3(3, 1, 0), 1.0, material2));
    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(point3(0, 1, -2), 1.0, material3));

    auto ground_material = make_shared<Lambertian>(Color(0.8, 0.8, 0.8));
    world.add(make_shared<Sphere>(point3(0,-1000,0), 1000, ground_material));

    auto start = system_clock::now();
    cam.render(world);
    auto elapsed_sec = duration<double>(system_clock::now() - start);

    printf("Took %.6f secs\n", elapsed_sec.count());

    return 0;
}
