#include "config.h"

#include "source/render/square.h"
#include "source/render/texture.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

const float SPEED = 0.16f;

int main(int argc, char* args[]) {
    // INITIALIZATION
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("yumesdl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

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
    render::texture tex = render::texture("assets/rouge.png", mathy::vec3<float>::ZERO(), 100, 0.0f, renderer);

    // SQUARE
    render::square sq = render::square(mathy::vec3<float>(300.0f, 300.0f, 0.0f), mathy::colorRGBA::BLUE(), 32, renderer);

    // BACKGROUND
    render::texture bgtex = render::texture("assets/sonic.png", mathy::vec3<float>::ZERO(), 80, 0.0f, renderer);

    // MAIN LOOP
    bool quit = false;
    while (!quit) {
        // EVENTS
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }
        }

        const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);

        if (currentKeyStates[SDL_SCANCODE_UP]) {
            sq.position = sq.position + mathy::vec3<float>::UPV2() * mathy::vec3<float> {SPEED, SPEED, 0};
        }
        if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            sq.position = sq.position + mathy::vec3<float>::DOWNV2() * mathy::vec3<float> {SPEED, SPEED, 0};
        }
        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            sq.position = sq.position + mathy::vec3<float>::LEFTV2() * mathy::vec3<float> {SPEED, SPEED, 0};
        }
        if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            sq.position = sq.position + mathy::vec3<float>::RIGHTV2() * mathy::vec3<float> {SPEED, SPEED, 0};
        }

        // MOVE TEXTURE
        float move_border = (float)SCREEN_WIDTH - (float)tex.size;
        if (tex.position.x < move_border) {
            tex.position.x += 0.05;
            tex.rotation_angle += 0.1;
        }

        // CLEAR SCREEN
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // BACKGROUND
        for (int x = 0; x < SCREEN_WIDTH; x += bgtex.size) {
            for (int y = 0; y < SCREEN_HEIGHT; y += bgtex.size) {
                bgtex.dynamic_position_draw(mathy::vec3<float>{(float)x, (float)y, 0.0f});
            }
        }

        // RENDER TEXTURE
        tex.render_texture();

        // DRAW SQUARE
        sq.render_square();

        // REFRESH RENDERER
        SDL_RenderPresent(renderer);
    }

    // DE-INITIALIZATION
    SDL_DestroyRenderer(renderer);
    std::cerr << "renderer destroyed successfully" << std::endl;

    SDL_DestroyWindow(window);
    std::cerr << "window destroyed successfully" << std::endl;

    SDL_DestroyTexture(tex.sdl_texture);
    SDL_DestroyTexture(bgtex.sdl_texture);

    SDL_Quit();

    return 0;
}
