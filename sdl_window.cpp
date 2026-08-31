#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <iostream>
#include <unistd.h>

void cleanup(SDL_Window *win);

int main(int argc, char *argv[]){
    if(!SDL_Init(SDL_INIT_VIDEO)){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error Initializing SDL3", nullptr);
        return EXIT_FAILURE;
    }

    int width {800};
    int height {600};
    SDL_Window *win = SDL_CreateWindow("Simple Game", width, height, 0);

    if(!win){
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", "Error creating window", win);
        cleanup(win);
        return EXIT_FAILURE;
    }

    bool running = true;

    while (running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_EVENT_QUIT: {
                    running = false;
                } break;
            }
        }
    }

    cleanup(win);
    return EXIT_SUCCESS;
}

void cleanup(SDL_Window *win){
    SDL_DestroyWindow(win);
    SDL_Quit();
}
