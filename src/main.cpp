#include "config.h"
#include "source/render/square.h"

#include "source/render/texture.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const float SPEED = 0.16f;

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
        std::cerr << "failed to initialize window:\n" << SDL_GetError();
        SDL_Quit();
        return EXIT_FAILURE;
    } else {
        std::cout << "window initialized successfully\n";
    }

    // RENDERER
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // TEXTURE
    render::texture tex = render::texture(
            mathy::vec3<float>::ZERO(),
            100,
            45.0f,
            "C:/Users/mydat/Documents/_active_c/_cpp/yumesdl/src/sonic.png",
            renderer
    );

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

        // MOVE TEXTURE
        int x = SCREEN_WIDTH - tex.size;
        if (tex.position.x < x) {
            tex.position.x += 0.05;
        }

        // CLEAR SCREEN
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // RENDER TEXTURE
        tex.draw();

        // DRAW SQUARE
        sq.draw();

        // REFRESH RENDERER
        SDL_RenderPresent(renderer);
    }

    // DE-INITIALIZATION
    std::cerr << std::endl;

    SDL_DestroyRenderer(renderer);
    std::cerr << "renderer destroyed successfully" << std::endl;

    SDL_DestroyWindow(window);
    std::cerr << "window destroyed successfully" << std::endl;

    SDL_DestroyTexture(tex.sdl_texture);

    SDL_Quit();

    return 0;
}
