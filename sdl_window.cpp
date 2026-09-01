#include <iostream>
#include <stdlib.h>
#include "sdl_window.hpp"

// The one definition of the global declared in sdl_window.h.
Render_State render_state;

#include "renderer.cpp"
#include "platform_common.cpp"


static bool init_sdl();
static SDL_Window* create_window();
static void resize_buffer(int width, int height);
static bool process_events();
static void cleanup(SDL_Window* win);
static void process_button(int scancode, bool is_down);

INPUT input = {};

#include "game.cpp"

int main(int argc, char* argv[]) {
    if (!init_sdl()) return EXIT_FAILURE;

    SDL_Window* win = create_window();
    if (!win) {
        cleanup(win);
        return EXIT_FAILURE;
    }
    clear_screen(0x000000);

    float delta_time = 0.016666f;
    Uint64 frame_start_time = SDL_GetPerformanceCounter();
    float performance_frequency = (float)SDL_GetPerformanceFrequency();


    while (process_events()) {
        // Render goes here.
        simulate_game(&input, delta_time);

        SDL_UpdateTexture(render_state.texture, nullptr, render_state.buffer_memory, render_state.win_width * sizeof(unsigned int));
        SDL_RenderTextureRotated(render_state.renderer, render_state.texture, nullptr, nullptr, 0.0, nullptr, SDL_FLIP_VERTICAL);
        SDL_RenderPresent(render_state.renderer);

        Uint64 frame_end_time = SDL_GetPerformanceCounter();
        delta_time = (float)(frame_end_time-frame_start_time)/performance_frequency;
        frame_start_time = frame_end_time;

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

    for(int i=0; i<BUTTON_COUNT;i++){
        input.buttons[i].changed = false;
    }
  
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
    for (int i = 0;i<BUTTON_COUNT;i++){
        input.buttons[i].changed = false;
    }
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_EVENT_QUIT: {
                return false;
            } break;

            case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
                resize_buffer(event.window.data1, event.window.data2);
            } break;

            case SDL_EVENT_KEY_UP:
            case SDL_EVENT_KEY_DOWN: {
                process_button(event.key.scancode, event.key.down);
            } break;
            default: {

            };
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

#define update_buttons(b) do{\
                                input.buttons[b].changed = is_down != input.buttons[b].is_down; \
                                input.buttons[b].is_down = is_down; \
                            } while(0)

void process_button(int scancode, bool is_down){
    switch (scancode) {
        case SDL_SCANCODE_W: {
            update_buttons(BUTTON_UP);
        }break;
        case SDL_SCANCODE_S: {
            update_buttons(BUTTON_DOWN);
        }break;
        case SDL_SCANCODE_A: {
            update_buttons(BUTTON_LEFT);
        }break;
        case SDL_SCANCODE_D: {
            update_buttons(BUTTON_RIGHT);
        }break;
        default: {};
    };
}
