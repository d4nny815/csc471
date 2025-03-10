#include "camera.h"

Camera::Camera(float aspect_ratio, size_t image_width, size_t samples_per_pixel,
    size_t child_rays) : samples_per_pixel(samples_per_pixel), 
    aspect_ratio(aspect_ratio),  image_width(image_width), child_rays(child_rays) {
    
    image_height = static_cast<size_t>(image_width / aspect_ratio);

    scale_per_pixel = 1.0 / samples_per_pixel;
    
    focal_length = 1.0f;
    viewport_height = 2.0f;
    viewport_width = viewport_height * aspect_ratio;
    pos = point3(0, 0, 0);

    // calc viewport vectors
    v_u = vec3(viewport_width, 0, 0);
    v_v = vec3(0, -viewport_height, 0);

    // calc delta viewport vectors
    pixel_du = v_u / image_width;
    pixel_dv = v_v / image_height;

    // calc upper-left corner of viewport
    viewport_upper_left = pos - vec3(0, 0, focal_length);
    viewport_upper_left = viewport_upper_left - v_u / 2 - v_v / 2;
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_du + pixel_dv);

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";
}

Ray Camera::get_ray(size_t col, size_t row) {
    vec3 offset = sample_square();
    vec3 pixel_sample = pixel00_loc + ((col + offset.x()) * pixel_du) +
        ((row + offset.y()) * pixel_dv);
    
    vec3 ray_dir = pixel_sample - pos;

    return Ray(pos, ray_dir);
}

Color Camera::ray_color(const Ray& r, const size_t depth, const Hittable& world) 
    const {
    
    const float GAMMA = 0.5f;
    if (depth <= 0) return Color(0,0,0);
    
    HitRecord rec;
    if (world.hit(r, Interval(0.001, MY_INFINITY), rec)) {
        Ray scattered;
        Color attenuation;
        if (rec.mat->scatter(r, rec, attenuation, scattered))         
            return attenuation * ray_color(scattered, depth - 1, world);
        return Color(0, 0, 0);
    }

    vec3 unit_direction = unit_vector(r.dir);
    auto a = 0.5*(unit_direction.y() + 1.0);
    return ((1.0-a)*Color(1.0, 1.0, 1.0) + a*Color(0.5, 0.7, 1.0));
}

vec3 Camera::sample_square() {
    return vec3(rand_float() - 0.5, rand_float() - 0.5, 0);
}

void Camera::render(const Hittable& world) {
    for (size_t row = 0; row < image_height; row++) {
        fprintf(stderr, "\rScanlines remaining: %zu    ", (image_height - row));
        fflush(stderr);
        for (size_t col = 0; col < image_width; col++) {
            Color pixel_color = Color(0, 0, 0); 
            
            for (auto i = 0; i < samples_per_pixel; i++) {
                Ray r = get_ray(col, row);
                pixel_color += ray_color(r, child_rays, world);
            }
            write_color(pixel_color * scale_per_pixel);                    
        }
    }
    fprintf(stderr, "\rDone                    \n");
}

void Camera::write_color(Color k) {
    static const Interval color_int(0, .999); 

    float r = linear_to_gamma(k.x());
    float g = linear_to_gamma(k.y());
    float b = linear_to_gamma(k.z());

    int red = int(256 * color_int.clamp(r));
    int green = int(256 * color_int.clamp(g));
    int blue = int(256 * color_int.clamp(b));

    printf("%d %d %d\n", red, green, blue);
}

inline float linear_to_gamma(float linear_comp) {
    return linear_comp > 0 ? std::sqrt(linear_comp) : 0;
}
