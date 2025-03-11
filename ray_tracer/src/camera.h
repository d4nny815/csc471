#ifndef CAMERA_H
#define CAMERA_H

#include "primatives.h"
#include "material.h"
#include "object.h"

class Camera {
public:
    size_t image_width;            
    size_t image_height;          
    float aspect_ratio;            
    size_t samples_per_pixel;       
    float scale_per_pixel;          
    size_t child_rays;            

    point3 pos;               
    point3 look_at;                
    point3 up_vector;             
    float focal_length;            

    float viewport_width;          
    float viewport_height;         

    vec3 w, u, v;                  // cam basis vectors
    vec3 v_u, v_v;                 // viewport vectors.
    vec3 pixel_du, pixel_dv;       // per-pixel delta vectors.
    vec3 viewport_upper_left;       
    vec3 pixel00_loc;             // location of the center of pixel (0,0)
    uint32_t* frame_buffer;        
    

    Camera(float aspect_ratio, size_t image_width, size_t samples_per_pixel, 
        size_t child_rays, float fov_deg, const point3& position, 
        const point3& look_at, const point3& up_vector);

    Ray get_ray(size_t col, size_t row);
    Color ray_color(const Ray& r, const size_t depth, const HittableList& world) 
        const;
    inline vec3 sample_square();

    void render(const HittableList& world);

    void write_color(Color k, size_t row, size_t col);
    void write_framebuffer();

};

inline float linear_to_gamma(float linear_comp);

#endif /* camera.h */