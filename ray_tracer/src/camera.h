#ifndef CAMERA_H
#define CAMERA_H

#include "primatives.h"
#include "material.h"
#include "object.h"

class Camera {
public:
    char* image_name;
    size_t image_width;            
    size_t image_height;          
    float aspect_ratio;            
    size_t samples_per_pixel;       
    float scale_per_pixel;          
    size_t child_rays;            

    float vfov;    
    float defocus_angle_deg;         // Variation angle of rays through each pixel
    float focus_dist;            // Distance from camera lookfrom point to plane of perfect focus       
    point3 pos;               
    point3 look_at;                
    point3 up_vector;             
    // float focal_length; 

    float viewport_width;          
    float viewport_height;         

    vec3 w, u, v;               // cam basis vectors
    vec3 v_u, v_v;              // viewport vectors.
    vec3 pixel_du, pixel_dv;    // per-pixel delta vectors.
    vec3 defocus_disk_u;        // Defocus disk horizontal radius
    vec3 defocus_disk_v;        // Defocus disk vertical radius
    vec3 viewport_upper_left;       
    vec3 pixel00_loc;           // location of the center of pixel (0,0)
    uint32_t* frame_buffer;        
    

    Camera(char* filename, float aspect_ratio, size_t image_width, size_t samples_per_pixel, 
        size_t child_rays, float vfov_deg, float defocus_angle_deg, float focus_dist,
        const point3& position, const point3& look_at, const point3& up_vector);

    __host__ __device__ Ray get_ray(int col, int row);
    __host__ __device__ Color ray_color(const Ray& r, const size_t depth, const HittableList& world) 
        const;
    __host__ __device__ inline vec3 sample_square();
    __host__ __device__ point3 defocus_disk_sample() const;

    void render(const HittableList& world);

    void write_framebuffer();

};

__host__ __device__ uint32_t write_color(Color k);
__host__ __device__ inline float linear_to_gamma(float linear_comp) {
    return linear_comp > 0 ? std::sqrt(linear_comp) : 0;
}

#endif /* camera.h */