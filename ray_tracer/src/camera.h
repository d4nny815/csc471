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

    Ray get_ray(size_t col, size_t row);
    Color ray_color(const Ray& r, const size_t depth, const HittableList& world) 
        const;
    inline vec3 sample_square();
    point3 defocus_disk_sample() const;

    void render(const HittableList& world);

    void write_color(Color k, size_t row, size_t col);
    void write_framebuffer();

};

inline float linear_to_gamma(float linear_comp);

#endif /* camera.h */