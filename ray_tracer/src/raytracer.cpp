#include <stdio.h>
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>

#include "primatives.h"
#include "util.h"

#define RED     (color(1, 0, 0))
#define GREEN   (color(0, 1, 0))
#define BLUE    (color(0, 0, 1))
#define WHITE   (color(1, 1, 1))
#define BLACK   (color(0, 0, 0))

color ray_color(const ray& r, const hittable& world) {
    hit_record hr;
    if (world.hit(r, interval(0, MY_INFINITY), hr)) {
        return .5 * (hr.normal + color(1, 1, 1));
    }

    vec3 unit_direction = unit_vector(r.dir);
    auto a = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - a) * WHITE + a * BLUE;
}

class Camera {
public:
    Camera(float aspect_ratio, size_t image_width, size_t image_height) {
        focal_length = 1.0f;
        viewport_height = 2.0f;
        viewport_width = viewport_height * aspect_ratio;
        cam_pos = point3(0, 0, 0);

        // Calculate viewport vectors
        v_u = vec3(viewport_width, 0, 0);
        v_v = vec3(0, -viewport_height, 0);

        // Calculate delta viewport vectors
        pixel_du = v_u / image_width;
        pixel_dv = v_v / image_height;

        // Calculate upper-left corner of viewport
        viewport_upper_left = cam_pos - vec3(0, 0, focal_length);
        viewport_upper_left = viewport_upper_left - v_u / 2 - v_v / 2;
        pixel00_loc = viewport_upper_left + 0.5 * (pixel_du + pixel_dv);
    }

    ray get_ray(size_t col, size_t row) {
        vec3 pixel_center = pixel00_loc + (col * pixel_du) + (row * pixel_dv);
        vec3 ray_dir = pixel_center - cam_pos;
        return ray(cam_pos, ray_dir);
    }

private:
    float focal_length;
    float viewport_height;
    float viewport_width;
    point3 cam_pos;
    vec3 v_u, v_v;
    vec3 pixel_du, pixel_dv;
    vec3 viewport_upper_left;
    vec3 pixel00_loc;
};

class Application {
public:
    Application(size_t width, size_t height)
        : image_width(width), image_height(height), running(true) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            running = false;
        }

        window = SDL_CreateWindow("Ray Tracer in SDL",
                                  SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                  image_width, image_height, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        if (!window || !renderer) {
            std::cerr << "SDL_CreateWindow/Renderer Error: " << SDL_GetError() << std::endl;
            SDL_Quit();
            running = false;
        }

        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888,
                                    SDL_TEXTUREACCESS_STREAMING,
                                    image_width, image_height);

        if (!texture) {
            std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
            SDL_Quit();
            running = false;
        }

        frame_buffer = new Uint32[image_width * image_height];
    }

    ~Application() {
        delete[] frame_buffer;
        SDL_DestroyTexture(texture);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    void run() {
        if (!running) return;

        // Create camera object
        Camera camera(static_cast<float>(image_width) / image_height, image_width, image_height);

        // Objects
        hittable_list world;
        world.add(make_shared<sphere>(point3(0, 0, -2), .5));
        world.add(make_shared<sphere>(point3(.8, 0, -2), .2));
        world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

        // Main event loop
        SDL_Event event;
        while (running) {
            // Render each pixel
            for (size_t row = 0; row < image_height; row++) {
                for (size_t col = 0; col < image_width; col++) {
                    ray r = camera.get_ray(col, row);
                    color pixel_color = ray_color(r, world);

                    int red = static_cast<int>(pixel_color.x() * 255.999);
                    int green = static_cast<int>(pixel_color.y() * 255.999);
                    int blue = static_cast<int>(pixel_color.z() * 255.999);

                    SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
                    Uint32 color = SDL_MapRGB(format, red, green, blue);
                    SDL_FreeFormat(format);

                    frame_buffer[row * image_width + col] = color;
                }
            }

            // Update the texture with the new frame buffer data
            SDL_UpdateTexture(texture, NULL, frame_buffer, image_width * sizeof(Uint32));

            // Handle events
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_KEYDOWN &&
                    event.key.keysym.scancode == SDL_SCANCODE_ESCAPE) {
                    running = false;
                }
            }

            // Render the texture to the screen
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
    }

private:
    size_t image_width;
    size_t image_height;
    bool running;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    Uint32* frame_buffer = nullptr;
};

int main() {
    const size_t image_width = 600;
    const float aspect_ratio = 3.0f / 2.0f;
    size_t image_height = static_cast<size_t>(image_width / aspect_ratio);

    Application app(image_width, image_height);
    app.run();

    return 0;
}
