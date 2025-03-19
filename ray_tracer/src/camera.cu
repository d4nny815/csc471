#include "camera.h"
#include <cuda_runtime.h>
#include <curand_kernel.h>
#include "cuda_help.h"

/**
 * @brief Constructs a Camera object with specified parameters.
 * @param aspect_ratio The aspect ratio of the image (width/height).
 * @param image_width The width of the output image in pixels.
 * @param samples_per_pixel The number of samples per pixel for anti-aliasing.
 * @param child_rays The number of child rays for each primary ray.
 * @param fov_deg The field of view in degrees, smaller zooms in
 * @param position The position of the camera in world space.
 * @param look_at The point the camera is looking at.
 * @param up_vector The up direction for the camera.
*/
Camera::Camera(char* filename, float aspect_ratio, size_t image_width, size_t samples_per_pixel, 
    size_t child_rays, float vfov_deg, float defocus_angle_deg, float focus_dist,
    const point3& position, const point3& look_at, const point3& up_vector) : 
    image_name(filename), image_width(image_width), 
    image_height(static_cast<size_t>(image_width / aspect_ratio)),
    aspect_ratio(aspect_ratio), samples_per_pixel(samples_per_pixel),
    scale_per_pixel(1.0f / samples_per_pixel), child_rays(child_rays), 
    defocus_angle_deg(defocus_angle_deg), focus_dist(focus_dist),
    pos(position), look_at(look_at), up_vector(up_vector) {

    // calc focal length
    // focal_length = (position - look_at).length();

    // calc viewport dimensions based on field of view
    vfov = degrees_to_radians(vfov_deg);
    float h = tan(vfov / 2);
    viewport_height = 2 * h * focus_dist;
    viewport_width = viewport_height * aspect_ratio;

    // calc orthonormal basis vectors for the camera coordinate system
    w = unit_vector(position - look_at);
    u = unit_vector(cross(up_vector, w));
    v = cross(w, u);

    // calc viewport edge vectors
    v_u = viewport_width * u;
    v_v = viewport_height * -v;

    // calc pixel delta vectors
    pixel_du = v_u / image_width;
    pixel_dv = v_v / image_height;

    // calc upper-left corner of the viewport
    viewport_upper_left = position - (focus_dist * w) - v_u / 2 - v_v / 2;
    pixel00_loc = viewport_upper_left + 0.5f * (pixel_du + pixel_dv);
    
    // Calculate the camera defocus disk basis vectors.
    auto defocus_radius = focus_dist * tan(degrees_to_radians(defocus_angle_deg / 2));
    defocus_disk_u = u * defocus_radius;
    defocus_disk_v = v * defocus_radius;

    frame_buffer = static_cast<uint32_t*>(malloc(sizeof(uint32_t) * 
        image_height * image_width));
}

/**
 * @brief shoot ray from cam to a pixel in the viewport
 * @param col current col in the viewport
 * @param row current row in the viewport
 * @return ray originating from the camera to the sampled pixel
 */
__host__ __device__ Ray Camera::get_ray(int col, int row) {
    // add random jitter to enable anti-aliasing
    // vec3 offset = sample_square(); 
    vec3 offset(0, 0, 0); 

    auto u = col + offset.x();
    auto v = row + offset.y();

    // get pixel location in the viewport
    vec3 pixel_sample = pixel00_loc + (u * pixel_du) + (v * pixel_dv);
    
    // dir from the cam to the sample pixel
    point3 ray_ori = (defocus_angle_deg <= 0) ? pos : defocus_disk_sample();
    vec3 ray_dir = pixel_sample - ray_ori;

    // printf("%d x %d %f x %f rOrigin <%f, %f, %f> rDir <%f, %f, %f>\n", 
    //     col, row, u, v,
    //     ray_ori.x(), ray_ori.y(), ray_ori.z(),
    //     ray_dir.x(), ray_dir.y(), ray_dir.z());
    
    return Ray(ray_ori, ray_dir);
}

/**
 * 
 */
__host__ __device__ point3 Camera::defocus_disk_sample() const {
    vec3 p = random_in_unit_disk();
    return pos + (p.x() * defocus_disk_u) + (p.y() * defocus_disk_v);
}

__host__ __device__ Color Camera::ray_color(const Ray& r, const size_t depth, const HittableList& world) 
    const {
    
    const Interval ray_interval(0.001, MY_INFINITY);
    Color accumulated_color(1.0, 1.0, 1.0); // Start with white (no attenuation)
    Ray current_ray = r;
    size_t current_depth = depth;

    while (current_depth > 0) {
        HitRecord hr;
        
        if (world.hit(current_ray, ray_interval, hr)) {
            Ray scattered;
            Color attenuation;
            
            if (hr.mat->scatter(current_ray, hr, attenuation, scattered)) {
                accumulated_color *= attenuation;  // Apply attenuation
                current_ray = scattered;  // Continue tracing the scattered ray
            } else {
                return BLACK * accumulated_color; // Absorb light (return black)
            }
        } else {
            // If the ray did not hit anything, return the sky color
            const Color SKY_BLUE = Color(0.5, 0.7, 1.0);
            vec3 unit_direction = unit_vector(current_ray.dir);
            auto a = 0.5 * (unit_direction.y() + 1.0);
            return accumulated_color * ((1.0 - a) * WHITE + a * SKY_BLUE);
        }

        current_depth--;
    }

    return Color(0, 0, 0); // Return black if maximum depth is reached
}

/**
 * @brief return vector with x = [-.5, .5] and y = [-.5, .5] 
 */
__host__ __device__ inline vec3 Camera::sample_square() {
    return vec3(rand_float() - 0.5, rand_float() - 0.5, 0);
}



__global__ void render_kernel(uint32_t* framebuffer, int image_width, int image_height, 
    int samples_per_pixel, Camera camera, HittableList world, 
    float scale_per_pixel) {
    int col = blockIdx.x * blockDim.x + threadIdx.x;
    int row = blockIdx.y * blockDim.y + threadIdx.y;

    if (col >= image_width || row >= image_height) return;

    Color pixel_color = Color(1, 0, 0);

    // // Monte Carlo sampling for anti-aliasing
    // for (int i = 0; i < samples_per_pixel; i++) {
    Ray r = camera.get_ray(col, row);
    pixel_color = camera.ray_color(r, camera.child_rays, world);
    // printf("%d x %d rOrigin <%f, %f, %f> rDir <%f, %f, %f> Color: [%f, %f, %f]\n", 
    //     col, row, 
    //     r.origin.x(), r.origin.y(), r.origin.z(),
    //     r.dir.x(), r.dir.y(), r.dir.z(),
    //     pixel_color.x(), pixel_color.y(), pixel_color.z());
    // }

    // Average the accumulated color
    // framebuffer[row * image_width + col] = pixel_color * scale_per_pixel;
    framebuffer[row * image_width + col] = write_color(pixel_color);
}

void Camera::render(const HittableList& world) {
    int num_pixels = image_width * image_height;
    uint32_t* d_framebuffer;
    
    // Allocate memory on the GPU
    CHECK_CUDA(cudaMalloc(&d_framebuffer, num_pixels * sizeof(uint32_t)));

    // Define CUDA block and grid dimensions
    dim3 block_size(16, 16);
    dim3 grid_size((image_width + block_size.x - 1) / block_size.x, 
                   (image_height + block_size.y - 1) / block_size.y);

    // Launch CUDA kernel
    render_kernel<<<grid_size, block_size>>>(d_framebuffer, image_width, image_height, 
                                             samples_per_pixel, *this, world, scale_per_pixel);
    CHECK_CUDA(cudaGetLastError());
    CHECK_CUDA(cudaDeviceSynchronize());

    // Copy framebuffer from GPU to CPU
    CHECK_CUDA(cudaMemcpy(frame_buffer, d_framebuffer, 
                num_pixels * sizeof(uint32_t), cudaMemcpyDeviceToHost));

    // Write framebuffer to file or screen
    write_framebuffer();

    // Cleanup
    CHECK_CUDA(cudaFree(d_framebuffer));
}

/**
 * @brief write the computed color value to the output in PPM format
 * 
 * takes a color vector, applies gamma correction, clamps
 * the values to the valid range, and converts them to 8bit color values and 
 * prints them for PPM format
 * 
 * @param k The color vector containing RGB values in linear space.
 */
__host__ __device__ uint32_t write_color(Color k) {
    float r = linear_to_gamma(k.x());
    float g = linear_to_gamma(k.y());
    float b = linear_to_gamma(k.z());

    uint8_t red = uint8_t(256 * Interval(0, .999).clamp(r));
    uint8_t green = uint8_t(256 * Interval(0, .999).clamp(g));
    uint8_t blue = uint8_t(256 * Interval(0, .999).clamp(b));

    uint32_t val = (red << 16) | (green << 8) | blue;
    return val;

    // printf("%d %d %d\n", red, green, blue);
}

void Camera::write_framebuffer() {
    FILE* file = fopen(image_name, "w");
    fprintf(file, "P3\n%zu %zu\n255\n", image_width, image_height);

    for (size_t row = 0; row < image_height; row++) {
        fprintf(stderr, "\rScanlines remaining: %zu    ", (image_height - row));
        fflush(stderr);

        for (size_t col = 0; col < image_width; col++) {
            auto ind = row * image_width + col;
            uint8_t red = (frame_buffer[ind] >> 16) & 0xff;
            uint8_t green = (frame_buffer[ind] >> 8) & 0xff;
            uint8_t blue = frame_buffer[ind] & 0xff;
            fprintf(file, "%d %d %d\n", red, green, blue);
        }
    }

    fclose(file);
    free(frame_buffer);
    fprintf(stderr, "\rDone                    \n");
}