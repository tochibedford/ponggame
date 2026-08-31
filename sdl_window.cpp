#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <stdlib.h>

void cleanup(SDL_Window *win);
void* buffer_memory;
int win_width {800};
int win_height {600};

int main(int argc, char *argv[]){
    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error Initializing SDL3", nullptr);
        return EXIT_FAILURE;
    }

    
    SDL_Window *win = SDL_CreateWindow("Simple Game", win_width, win_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALWAYS_ON_TOP);
    if(!win){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating window", win);
        cleanup(win);
        return EXIT_FAILURE;
    }

    SDL_GetWindowSizeInPixels(win, &win_width, &win_height);

    bool running = true;

    while (running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_EVENT_QUIT: {
                    running = false;
                }break;
                case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED: {
                    win_width = event.window.data1;
                    win_height = event.window.data2;

                    int buffer_size = win_width * win_height * sizeof(unsigned int);
                    if(buffer_memory) free(buffer_memory);
                    buffer_memory = malloc(buffer_size);

                }break;
            }
        }
    }
    
    cleanup(win);
    return EXIT_SUCCESS;
}

void cleanup(SDL_Window *win){
    free(buffer_memory);
    SDL_DestroyWindow(win);
    SDL_Quit();
}
