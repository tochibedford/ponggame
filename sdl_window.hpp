#ifndef SDL_WINDOW_HPP
#define SDL_WINDOW_HPP

#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

struct Render_State {
    int win_width {1000}, win_height{500};
    SDL_Renderer* renderer;
    SDL_Texture* texture;
    void* buffer_memory;
};

// Declared here, defined once in sdl_window.cpp.
extern Render_State render_state;

#endif
