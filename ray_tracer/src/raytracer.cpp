#include <stdio.h>
#include <iostream>
#include <cmath>
#include <SDL2/SDL.h>
#include <chrono>

#include "primatives.h"
#include "util.h"
#include "camera.h"
#include "interval.h"

#define RED     (color(1, 0, 0))
#define GREEN   (color(0, 1, 0))
#define BLUE    (color(0, 0, 1))
#define WHITE   (color(1, 1, 1))
#define BLACK   (color(0, 0, 0))

using namespace std::chrono::_V2;


class Application {
public:
    size_t image_width;
    size_t image_height;
    bool running;
    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    Uint32* frame_buffer = nullptr;
    system_clock::time_point prev_tick, cur_tick;

    // ray tracer components
    int samples_per_pixel = 5;
    float scale_per_pixel = 1.0 / samples_per_pixel;


    Application(size_t width, size_t height)
        : image_width(width), image_height(height), running(true) {
        if (SDL_Init(SDL_INIT_VIDEO) != 0) {
            std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
            running = false;
        }

        window = SDL_CreateWindow("Ray Tracer",
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

        Camera camera(static_cast<float>(image_width) / image_height
            , image_width, image_height, 10);

        // objects
        hittable_list world;
        world.add(make_shared<sphere>(point3(0, 0, -1), .5));
        world.add(make_shared<sphere>(point3(.8, 0, -1), .2));
        world.add(make_shared<sphere>(point3(0, -100.5, -1), 100));


        system_clock::time_point prev_tick;
        system_clock::time_point cur_tick;
        prev_tick = system_clock::now();

        SDL_Event event;
        while (running) {
            cur_tick = system_clock::now();
            auto elapsed = cur_tick - prev_tick;
            prev_tick = cur_tick;
            double fps = 1e9 / elapsed.count();
            fprintf(stderr, "\rFPS: %.4lf        ", fps);
            fflush(stderr);

            for (size_t row = 0; row < image_height; row++) {
                for (size_t col = 0; col < image_width; col++) {
                    color pixel_color = color(0, 0, 0); 
                    
                    for (auto i = 0; i < samples_per_pixel; i++) {
                        ray r = camera.get_ray(col, row);
                        pixel_color += camera.ray_color(r, world);
                    }

                    write_color(pixel_color * scale_per_pixel, row, col);                    
                }
            }

            // Update the texture with the new frame buffer data
            SDL_UpdateTexture(texture, NULL, frame_buffer, image_width * sizeof(Uint32));

            // Handle events
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.scancode) {
                        case SDL_SCANCODE_ESCAPE:
                            running = false;
                            break;
                        case SDL_SCANCODE_W:
                            camera.pos.data[2] -= .2;
                            break;
                        case SDL_SCANCODE_S:
                            camera.pos.data[2] += .2;
                            break;
                        default:
                            break;
                    }
                }
            }

            // Render the texture to the screen
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
    }

    void write_color(color k, size_t row, size_t col) {
        static const interval color_int(0, .999); 

        int red = int(256 * color_int.clamp(k.x()));
        int green = int(256 * color_int.clamp(k.y()));
        int blue = int(256 * color_int.clamp(k.z()));

        SDL_PixelFormat* format = SDL_AllocFormat(SDL_PIXELFORMAT_RGB888);
        Uint32 color = SDL_MapRGB(format, red, green, blue);
        SDL_FreeFormat(format);

        frame_buffer[row * image_width + col] = color;
    }
};

int main() {
    const size_t image_width = 600;
    const float aspect_ratio = 3.0f / 2.0f;
    size_t image_height = static_cast<size_t>(image_width / aspect_ratio);

    Application app(image_width, image_height);
    app.run();

    return 0;
}
