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
    int startX = x1 <= x2 ? x1 : x2;
    int endX = x1 <= x2 ? x2 : x1;
    int startY = y1 <= y2 ? y1 : y2;
    int endY = y1 <= y2 ? y2 : y1;
    for(int y=y1; y>0 && y<render_state.win_height && y<y2; y++)
        for (int x = x1; x>0 && x<render_state.win_width && x<x2; x++){
            *(pixel + coordinateTo1d(x, y, render_state.win_width)) = color;
        }
}

void draw_rect(float x, float y, float half_size_x, float half_size_y, unsigned int color){
    x *= render_state.win_height;
    y *= render_state.win_height;
    half_size_x *= render_state.win_height;
    half_size_y *= render_state.win_height;

    x += render_state.win_width / 2.f;
    y += render_state.win_height / 2.f;
    int x1 = x-half_size_x;
    int x2 = x+half_size_x;
    int y1 = y-half_size_y;
    int y2 = y+half_size_y;
    draw_rect_in_pixels(x1, y1, x2, y2, color);
}
