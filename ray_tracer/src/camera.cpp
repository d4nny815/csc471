#include "camera.h"

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
Camera::Camera(float aspect_ratio, size_t image_width, size_t samples_per_pixel, 
    size_t child_rays, float fov_deg, const point3& position, 
    const point3& look_at, const point3& up_vector) : image_width(image_width), 
    image_height(static_cast<size_t>(image_width / aspect_ratio)),
    aspect_ratio(aspect_ratio), samples_per_pixel(samples_per_pixel),
    scale_per_pixel(1.0f / samples_per_pixel), child_rays(child_rays),
    pos(position), look_at(look_at), up_vector(up_vector) {

    // calc focal length
    focal_length = (position - look_at).length();

    // calc viewport dimensions based on field of view
    float theta = degrees_to_radians(fov_deg);
    float h = tan(theta / 2);
    viewport_height = 2 * h * focal_length;
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
    viewport_upper_left = position - (focal_length * w) - v_u / 2 - v_v / 2;
    pixel00_loc = viewport_upper_left + 0.5f * (pixel_du + pixel_dv);

    frame_buffer = static_cast<uint32_t*>(malloc(sizeof(uint32_t) * 
        image_height * image_width));
}

/**
 * @brief shoot ray from cam to a pixel in the viewport
 * @param col current col in the viewport
 * @param row current row in the viewport
 * @return ray originating from the camera to the sampled pixel
 */
Ray Camera::get_ray(size_t col, size_t row) {
    // add random jitter to enable anti-aliasing
    vec3 offset = sample_square(); 

    auto u = col + offset.x();
    auto v = row + offset.y();

    // get pixel location in the viewport
    vec3 pixel_sample = pixel00_loc + (u * pixel_du) + (v * pixel_dv);
    
    // dir from the cam to the sample pixel
    vec3 ray_dir = pixel_sample - pos;

    return Ray(pos, ray_dir);
}

/**
 * @brief recursively calc the color along a ray
 * 
 * This function traces a ray into the scene and determines the color resulting
 *  from any interactions. It uses recursion to handle multiple bounces of light
 * 
 * @param r the ray to trace into the scene
 * @param depth max number of bounces allowed for the ray
 * @param world hittable objects that can interact with the ray
 * @return resulting color from the ray interaction.
 */
Color Camera::ray_color(const Ray& r, const size_t depth, const HittableList& world) 
    const {
    
    const Interval ray_interval(0.001, MY_INFINITY);
    
    if (depth <= 0) return Color(0,0,0);
    
    HitRecord hr;
    // check if ray hits anything in the world
    if (world.hit(r, ray_interval, hr)) {
        Ray scattered;
        Color attenuation;

        // check if rays scatters if so trace it
        if (hr.mat->scatter(r, hr, attenuation, scattered))         
            return attenuation * ray_color(scattered, depth - 1, world);
        
        // if ray isnt scattered absorb the light (return black)
        return BLACK;
    }

    // If the ray did not hit anything, return the sky color.
    // The sky is interpolated from white to blue based on y axis
    const Color SKY_BLUE = Color(0.5, 0.7, 1.0);
    vec3 unit_direction = unit_vector(r.dir);
    auto a = 0.5 * (unit_direction.y() + 1.0);

    return (1.0 - a) * WHITE + a * SKY_BLUE;
}

/**
 * @brief return vector with x = [-.5, .5] and y = [-.5, .5] 
 */
inline vec3 Camera::sample_square() {
    return vec3(rand_float() - 0.5, rand_float() - 0.5, 0);
}

/**
 * @brief render the scene
 * 
 * shoot multiple rays from cam to each pixel in the viewport to the world
 * 
 * @param world hittable objects in the world 
 */
void Camera::render(const HittableList& world) {
    // #pragma omp parallel for schedule(dynamic)
    for (size_t row = 0; row < image_height; row++) {
        fprintf(stderr, "\rScanlines remaining: %zu    ", (image_height - row));
        fflush(stderr);
        for (size_t col = 0; col < image_width; col++) {
            Color pixel_color = Color(0, 0, 0); 
            
            // cast multiple rays per pixel for anti aliasing
            for (size_t i = 0; i < samples_per_pixel; i++) {
                Ray r = get_ray(col, row);
                pixel_color += ray_color(r, child_rays, world);
            }

            // average the accumulated color
            write_color(pixel_color * scale_per_pixel, row, col);                    
        }
    }
    write_framebuffer();
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
void Camera::write_color(Color k, size_t row, size_t col) {
    static const Interval color_int(0, .999); 

    float r = linear_to_gamma(k.x());
    float g = linear_to_gamma(k.y());
    float b = linear_to_gamma(k.z());

    uint8_t red = uint8_t(256 * color_int.clamp(r));
    uint8_t green = uint8_t(256 * color_int.clamp(g));
    uint8_t blue = uint8_t(256 * color_int.clamp(b));

    uint32_t val = (red << 16) | (green << 8) | blue;
    frame_buffer[row * image_width + col] = val;

    // printf("%d %d %d\n", red, green, blue);
}

void Camera::write_framebuffer() {
    FILE* file = fopen("img.ppm", "w");
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

/**
 * @brief convert linear color component to correct gamma-corrected color
 * 
 * the standard gamma correction assumes a gamma of 2.0, which is achieved 
 * by taking the square root of the component.
 * 
 */
inline float linear_to_gamma(float linear_comp) {
    return linear_comp > 0 ? std::sqrt(linear_comp) : 0;
}
