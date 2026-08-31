#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>

void* buffer_memory;
SDL_Renderer* renderer;
SDL_Texture* texture;
int win_width  {800};
int win_height {600};

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

    while (process_events()) {
        // Render goes here.
        unsigned int* pixel = (unsigned int*)buffer_memory;
        for (int y = 0; y < win_height; y++)
            for (int x = 0; x < win_width; x++)
                *pixel++ = x * y;
        SDL_UpdateTexture(texture, nullptr, buffer_memory, win_width * sizeof(unsigned int));
        SDL_RenderTextureRotated(renderer, texture, nullptr, nullptr, 0.0, nullptr, SDL_FLIP_VERTICAL);
        SDL_RenderPresent(renderer);
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
    SDL_Window* win = SDL_CreateWindow("Simple Game", win_width, win_height,
                                       SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP);
    if (!win) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error",
                                 "Error creating window", nullptr);
        return nullptr;
    }

    renderer = SDL_CreateRenderer(win, nullptr);

  
    int pixel_width, pixel_height;
    SDL_GetWindowSizeInPixels(win, &pixel_width, &pixel_height);
    resize_buffer(pixel_width, pixel_height);

    return win;
}

static void resize_buffer(int width, int height) {
    win_width = width;
    win_height = height;

    int buffer_size = win_width * win_height * sizeof(unsigned int);

    if (buffer_memory) free(buffer_memory);
    buffer_memory = malloc(buffer_size);

    if(texture) SDL_DestroyTexture(texture);
    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_XRGB8888, SDL_TEXTUREACCESS_STREAMING, win_width, win_height);
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
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    free(buffer_memory);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
