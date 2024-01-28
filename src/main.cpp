#include "config.h"
#include "source/render/square.h"

#include "source/render/texture.h"

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
        std::cerr << "failed to initialize window:\n" << SDL_GetError();
        SDL_Quit();
        return EXIT_FAILURE;
    } else {
        std::cout << "window initialized successfully\n";
    }

    // RENDERER
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // TEXTURE
    SDL_Rect destinationRect = { 100, 100, 200, 200 }; // rect
    SDL_Point center = { destinationRect.w / 2, destinationRect.h / 2 }; // center
    double angle = 45.0; // angle of rotation

    SDL_Surface* surface = IMG_Load( // loading surface and tex
        "C:/Users/mydat/Documents/_active_c/_cpp/yumesdl/src/sonic.png");

    if (surface == nullptr) {
        std::cerr << "IMG_Load: Failed to load image: " << IMG_GetError() << std::endl;
    }
    else {
        std::cout << "texture loaded successfully\n";
    }

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr) { // creating texture on surface
        std::cerr << "SDL_CreateTextureFromSurface: Failed to create texture: " << SDL_GetError() << std::endl;
    }
    else {
        std::cout << "texture created successfully\n";
    }

    // TEXTURE 2
    render::texture tex = render::texture(mathy::vec3<float>::ONE(), 100, 45.0f, "C:/Users/mydat/Documents/_active_c/_cpp/yumesdl/src/sonic.png", renderer);

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
        if (tex.position.x < SCREEN_WIDTH) {
            tex.position.x += 0.05;
        }

        // CLEAR SCREEN
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // RENDER TEXTURE
        SDL_RenderCopyEx(renderer, texture, NULL, &destinationRect, angle, &center, SDL_FLIP_NONE);

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

    SDL_DestroyTexture(texture);
    SDL_DestroyTexture(tex.sdl_texture);
    std::cerr << "texture destroyed successfully" << std::endl;

    SDL_Quit();

    return 0;
}
