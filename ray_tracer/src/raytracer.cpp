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

int main(void) {
    // Setup image
    const size_t image_width = 600;
    const float aspect_ratio = 3.0f / 2.0f;
    size_t image_height = static_cast<size_t>(image_width / aspect_ratio);

    // SDL initialization
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("Ray Tracer in SDL", 
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                          image_width, image_height, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    if (!window || !renderer) {
        std::cerr << "SDL_CreateWindow/Renderer Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    // Setup camera
    float focal_length = 1.0f;
    float viewport_height = 2.0f;
    float viewport_width = viewport_height * 
        (static_cast<float>(image_width) / image_height); 
    point3 cam_pos = point3(0, 0, 0);

    // Calculate viewport vectors
    vec3 v_u = vec3(viewport_width, 0, 0);
    vec3 v_v = vec3(0, -viewport_height, 0);

    // Calculate delta viewport vectors
    vec3 pixel_du = v_u / image_width;
    vec3 pixel_dv = v_v / image_height;

    // Calculate upper-left corner of viewport
    vec3 viewport_upper_left = cam_pos - vec3(0, 0, focal_length); 
    viewport_upper_left = viewport_upper_left - v_u / 2 - v_v / 2; 
    vec3 pixel00_loc = viewport_upper_left + 0.5 * (pixel_du + pixel_dv);

    // Objects 
    hittable_list world;
    world.add(make_shared<sphere>(point3(0, 0, -1), .5));
    world.add(make_shared<sphere>(point3(.8, 0, -1), .2));
    world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Create texture once (outside of the event loop)
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB888, 
                                             SDL_TEXTUREACCESS_STREAMING, 
                                             image_width, image_height);
    if (!texture) {
        std::cerr << "Failed to create texture: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    Uint32* frame_buffer = new Uint32[image_width * image_height];

    bool running = true;
    SDL_Event event;
    while (running) {
        // Ray tracing loop inside the event loop
        for (size_t row = 0; row < image_height; row++) {
            for (size_t col = 0; col < image_width; col++) {
                vec3 pixel_center = pixel00_loc + (col * pixel_du) + (row * pixel_dv);
                vec3 ray_dir = pixel_center - cam_pos;  
                ray r(cam_pos, ray_dir);

                color pixel_color = ray_color(r, world);

                int red = static_cast<int>(pixel_color.x() * 255.999);
                int green = static_cast<int>(pixel_color.y() * 255.999);
                int blue = static_cast<int>(pixel_color.z() * 255.999);

                // Map to SDL color
                SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
                Uint32 color = SDL_MapRGB(format, red, green, blue);
                SDL_FreeFormat(format);

                // Store the color in the frame buffer
                frame_buffer[row * image_width + col] = color;
            }
        }

        // Update the texture with the new frame buffer data
        SDL_UpdateTexture(texture, NULL, frame_buffer, image_width * sizeof(Uint32));

        // keyboard movements
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

    // Cleanup
    delete[] frame_buffer;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}


// SDL_PIXELFORMAT_RGB888