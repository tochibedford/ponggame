#include "sdl_window.hpp"
#include "util.hpp"

void clear_screen(unsigned int color){
    unsigned int* pixel = (unsigned int*)render_state.buffer_memory;
    for (int y = 0; y < render_state.win_height; y++)
        for (int x = 0; x < render_state.win_width; x++)
            *pixel++ = color;
}

void draw_rect_in_pixels(int x1, int y1, int x2, int y2, unsigned int color){
    unsigned int* pixel = (unsigned int*) render_state.buffer_memory;
    int startX = clamp(0, x1 <= x2 ? x1 : x2, render_state.win_width);
    int endX   = clamp(0, x1 <= x2 ? x2 : x1, render_state.win_width);
    int startY = clamp(0, y1 <= y2 ? y1 : y2, render_state.win_height);
    int endY   = clamp(0, y1 <= y2 ? y2 : y1, render_state.win_height);

    for (int y = startY; y < endY; y++)
        for (int x = startX; x < endX; x++){
            *(pixel + coordinateTo1d(x, y, render_state.win_width)) = color;
        }
}

static float render_scale = 0.01f;

static float reference_extent(){
    return (render_state.win_width * 9 > render_state.win_height * 16)
        ? render_state.win_height
        : render_state.win_width;
}

static float pixels_per_unit(){
    return reference_extent() * render_scale;
}

void draw_rect(float x, float y, float half_size_x, float half_size_y, unsigned int color){
    float scale = pixels_per_unit();
    x *= scale;
    y *= scale;
    half_size_x *= scale;
    half_size_y *= scale;

    x += render_state.win_width / 2.f;
    y += render_state.win_height / 2.f;
    int x1 = x-half_size_x;
    int x2 = x+half_size_x;
    int y1 = y-half_size_y;
    int y2 = y+half_size_y;
    draw_rect_in_pixels(x1, y1, x2, y2, color);
}
