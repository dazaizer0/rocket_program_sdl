#if (WIN32)
#define SDL_MAIN_HANDLED
#endif

#include <SDL2/SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>

#include "packages/core/scene.hpp"

int main(int argc, char* argv[]) {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("YUME", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            const Uint8* state = SDL_GetKeyboardState(NULL);
            if (state[SDL_SCANCODE_ESCAPE]) {
                quit = true;
            }

            int x, y;
            Uint32 buttons = SDL_GetMouseState(&x, &y);
            if (buttons & SDL_BUTTON(SDL_BUTTON_LEFT)) {
                std::cout << "Left mouse button is pressed at position: " << x << ", " << y << '\n';
            }
            else if (buttons & SDL_BUTTON(SDL_BUTTON_RIGHT)) {
                std::cout << "Right mouse button is pressed at position: " << x << ", " << y << '\n';
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);

        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
