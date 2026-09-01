#include <stdlib.h>
#include "sdl_window.hpp"

// The one definition of the global declared in sdl_window.h.
Render_State render_state;

#include "renderer.cpp"

static bool init_sdl();
static SDL_Window* create_window();
static void resize_buffer(int width, int height);
static bool process_events();
static void cleanup(SDL_Window* win);

int main(int argc, char* argv[]) {
    if (!init_sdl()) return EXIT_FAILURE;

    SDL_Window* win = create_window();
    if (!win) {
        cleanup(win);
        return EXIT_FAILURE;
    }
    clear_screen(0x000000);
    while (process_events()) {
        // Render goes here.
        // render_background();
        clear_screen(0xff5500);
        // draw_rect_in_pixels(50, 50, 200, 500, 0x00ff22);
        draw_rect(0, 0, 0.2, 0.2, 0x00ff22);

        SDL_UpdateTexture(render_state.texture, nullptr, render_state.buffer_memory, render_state.win_width * sizeof(unsigned int));
        SDL_RenderTextureRotated(render_state.renderer, render_state.texture, nullptr, nullptr, 0.0, nullptr, SDL_FLIP_VERTICAL);
        SDL_RenderPresent(render_state.renderer);
    }

    cleanup(win);
    return EXIT_SUCCESS;
}

static bool init_sdl() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                                 "Error Initializing SDL3", nullptr);
        return false;
    }
    return true;
}

static SDL_Window* create_window() {
    SDL_Window* win = SDL_CreateWindow("Simple Game", render_state.win_width, render_state.win_height,
                                       SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP);
    if (!win) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                                 "Error creating window", nullptr);
        return nullptr;
    }

    render_state.renderer = SDL_CreateRenderer(win, nullptr);

  
    int pixel_width, pixel_height;
    SDL_GetWindowSizeInPixels(win, &pixel_width, &pixel_height);
    resize_buffer(pixel_width, pixel_height);

    return win;
}

static void resize_buffer(int width, int height) {
    render_state.win_width = width;
    render_state.win_height = height;

    int buffer_size = render_state.win_width * render_state.win_height * sizeof(unsigned int);

    if (render_state.buffer_memory) free(render_state.buffer_memory);
    render_state.buffer_memory = malloc(buffer_size);

    if(render_state.texture) SDL_DestroyTexture(render_state.texture);
    render_state.texture = SDL_CreateTexture(render_state.renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, render_state.win_width, render_state.win_height);
}

static bool process_events() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT: {
                return false;
            } break;

            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
                resize_buffer(event.window.data1, event.window.data2);
            } break;
        }
    }
    return true;
}

static void cleanup(SDL_Window* win) {
    SDL_DestroyTexture(render_state.texture);
    SDL_DestroyRenderer(render_state.renderer);
    free(render_state.buffer_memory);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
