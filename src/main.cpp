#include "config.h"
#include "source/render/square.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const float SPEED = 0.2f;

int main(int argc, char* args[]) {
    // INITIALIZATION
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow(
            "yumesdl",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN
    );

    if (window == nullptr) {
        std::cerr << "Failed to initialize window: \n" << SDL_GetError() << std::endl;
        SDL_Quit();
        return EXIT_FAILURE;
    } else {
        std::cout << "Window initialized successfully!" << std::endl;
    }

    // RENDERER
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // SQUARE
    render::square sq = render::square(
            mathy::vec3<float>::CUSTOM(300.0f, 300.0f, 0.0f),
            mathy::colorRGBA::BLUE(),
            32,
            renderer
    );

    // MAIN LOOP
    bool quit = false;
    while (!quit) {
        // EVENTS
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(NULL);

        if (currentKeyStates[SDL_SCANCODE_UP]) {
            sq.position.y -= SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            sq.position.y += SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            sq.position.x -= SPEED;
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            sq.position.x += SPEED;
        }

        // CLEAR SCREEN
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // DRAW SQUARE
        sq.draw();

        // REFRESH RENDERER
        SDL_RenderPresent(renderer);
    }

    // DE-INITIALIZATION
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
