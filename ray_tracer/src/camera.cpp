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

ray Camera::get_ray(size_t col, size_t row) {
    vec3 offset = sample_square();
    vec3 pixel_sample = pixel00_loc + ((col + offset.x()) * pixel_du) +
        ((row + offset.y()) * pixel_dv);
    
    // vec3 ray_origin = pos;
    vec3 ray_dir = pixel_sample - pos;

    return ray(pos, ray_dir);
}

color Camera::ray_color(const ray& r, const size_t depth, const hittable& world) 
    const {
    
    if (depth <= 0) return color(0,0,0);
    
    hit_record rec;
    if (world.hit(r, interval(0.001, MY_INFINITY), rec)) {
        // shoot child rays and see if they hit anything
        vec3 direction = random_on_hemisphere(rec.normal);
        return 0.5 * ray_color(ray(rec.point, direction), depth - 1, world);
        // return 0.5 * (rec.normal + color(1,1,1));
    }

    vec3 unit_direction = unit_vector(r.dir);
    auto a = 0.5*(unit_direction.y() + 1.0);
    return ((1.0-a)*color(1.0, 1.0, 1.0) + a*color(0.5, 0.7, 1.0));
}

vec3 Camera::sample_square() {
    return vec3(rand_float() - 0.5, rand_float() - 0.5, 0);
}

void Camera::render(const hittable& world) {
    for (size_t row = 0; row < image_height; row++) {
        fprintf(stderr, "\rScanlines remaining: %zu    ", (image_height - row));
        fflush(stderr);
        for (size_t col = 0; col < image_width; col++) {
            color pixel_color = color(0, 0, 0); 
            
            for (auto i = 0; i < samples_per_pixel; i++) {
                ray r = get_ray(col, row);
                pixel_color += ray_color(r, child_rays, world);
            }
            write_color(pixel_color * scale_per_pixel);                    
        }
    }
    fprintf(stderr, "\rDone                    \n");
}

void Camera::write_color(color k) {
    static const interval color_int(0, .999); 

    int red = int(256 * color_int.clamp(k.x()));
    int green = int(256 * color_int.clamp(k.y()));
    int blue = int(256 * color_int.clamp(k.z()));

    printf("%d %d %d\n", red, green, blue);
}
 
