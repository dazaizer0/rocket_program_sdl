#include "config.h"

#include "source/render/square.hpp"
#include "source/render/texture.hpp"
#include "source/render/objects.hpp"

const int SCREEN_WIDTH = 650;
const int SCREEN_HEIGHT = 500;

const float SPEED = 620.0f;

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
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED );

    int grid_size = 50;
    mathy::vec2<int> mouse_pos = mathy::vec2<int>::ZERO();
    bool mouse_clicked = false;

    // TEXTURE
    render::texture tex = render::texture("assets/rouge.png", mathy::vec2<float>::ZERO(), mathy::vec2<float>{100.0f, 100.0f}, 0.0f, renderer);

    // SQUARE
    render::square sq = render::square(mathy::vec2<float>{300.0f, 300.0f}, mathy::colorRGBA::BLUE(), mathy::vec2<float>{(float)grid_size, (float)grid_size}, renderer);
    render::square mouse = render::square(mathy::vec2<float>::ZERO(), mathy::colorRGBA::RED(), mathy::vec2<float>{(float)grid_size, (float)grid_size}, renderer);

    // BACKGROUND
    render::texture bgtex = render::texture("assets/sonic.png", mathy::vec2<float>::ZERO(), mathy::vec2<float>{(float)grid_size, (float)grid_size}, 0.0f, renderer);

    // MAIN LOOP
    Uint32 prevTime = SDL_GetTicks();

    bool quit = false;
    while (!quit) {
        Uint64 start = SDL_GetPerformanceCounter();

        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - prevTime) / 1000.0f;

        // EVENTS
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) {
                quit = true;
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                mouse_clicked = true;
            }
            if (e.type == SDL_MOUSEBUTTONUP) {
                mouse_clicked = false;
            }
        }

        SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

        // mouse.position = mathy::vec2<float>{(float)((int)(mouse_pos.x / grid_size) * grid_size), (float)((int)(mouse_pos.y / grid_size) * grid_size)};
        
         // MOVEMENT
        const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);

        if (currentKeyStates[SDL_SCANCODE_UP]) {
            sq.position = sq.position + mathy::vec2<float>::UP() * mathy::vec2<float> {SPEED, SPEED} * mathy::vec2<float> {deltaTime, deltaTime};
        } else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            sq.position = sq.position + mathy::vec2<float>::DOWN() * mathy::vec2<float> {SPEED, SPEED} * mathy::vec2<float> {deltaTime, deltaTime};
        }

        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            sq.position = sq.position + mathy::vec2<float>::LEFT() * mathy::vec2<float> {SPEED, SPEED} * mathy::vec2<float> {deltaTime, deltaTime};
        } else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            sq.position = sq.position + mathy::vec2<float>::RIGHT() * mathy::vec2<float> {SPEED, SPEED} * mathy::vec2<float> {deltaTime, deltaTime};
        }

        // MOVE AND ROTATE TEXTURE
        // float move_border = (float)SCREEN_WIDTH - (float)tex.size.x;
        // if (tex.position.x < move_border) {
        //     tex.position.x += 50.0f * deltaTime;
        //     tex.rotation_angle += 100.0f * deltaTime;
        // }

        if (mathy::distance(mouse_pos, mathy::vec2<int>{(int)tex.position.x, (int)tex.position.y}) < ((tex.size.x + tex.size.y) / 2)) {
            tex.size = mathy::vec2<float>{120.0f, 120.0f};

            if (mouse_clicked) {
                tex.position = mathy::vec2<float>{(float)mouse_pos.x, (float)mouse_pos.y};
            }
        }
        else {
            tex.size = mathy::vec2<float>{100.0f, 100.0f};
        }
        
        
        // CLEAR SCREEN
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // BACKGROUND
        //for (int x = 0; x < SCREEN_WIDTH; x += bgtex.size) {
        //    for (int y = 0; y < SCREEN_HEIGHT; y += bgtex.size) {
        //        bgtex.dynamic_position_draw(mathy::vec3<float>{(float)x, (float)y, 0.0f});
        //    }
        //}

        // RENDER TEXTURE
        tex.render_texture();   

        // DRAW SQUARE
        sq.render_square();
        // mouse.render_square();

        // REFRESH RENDERER
        SDL_RenderPresent(renderer);
        prevTime = currentTime;

        Uint64 end = SDL_GetPerformanceCounter();
	    float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(floor(16.666f - elapsedMS)); // cap fps to 60
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
