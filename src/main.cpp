#include "config.h"

#include "source/render/square.hpp"
#include "source/render/texture.hpp"
#include "source/render/objects.hpp"

const int SCREEN_WIDTH = 650;
const int SCREEN_HEIGHT = 500;

const float SPEED = 620.0f;

double distance(int x1, int y1, int x2, int y2) {
    return std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
}

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
    int mouse_pos_x, mouse_pos_y = 0;

    // TEXTURE
    render::texture tex = render::texture("assets/rouge.png", mathy::vec3<float>::ZERO(), 100, 0.0f, renderer);

    // SQUARE
    render::square sq = render::square(mathy::vec3<float>(300.0f, 300.0f, 0.0f), mathy::colorRGBA::BLUE(), grid_size, renderer);
    render::square mouse = render::square(mathy::vec3<float>::ZERO(), mathy::colorRGBA::RED(), grid_size, renderer);

    // BACKGROUND
    render::texture bgtex = render::texture("assets/sonic.png", mathy::vec3<float>::ZERO(), 80, 0.0f, renderer);

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
        }

        SDL_GetMouseState(&mouse_pos_x, &mouse_pos_y);

        mouse.position = mathy::vec3<float>{(float)((int)(mouse_pos_x / grid_size) * grid_size), (float)((int)(mouse_pos_y / grid_size) * grid_size), 0};
        
         // MOVEMENT
        const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);

        if (currentKeyStates[SDL_SCANCODE_UP]) {
            sq.position = sq.position + mathy::vec3<float>::UPV2() * mathy::vec3<float> {SPEED, SPEED, 0} * mathy::vec3<float> {deltaTime, deltaTime, 0};
        } else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            sq.position = sq.position + mathy::vec3<float>::DOWNV2() * mathy::vec3<float> {SPEED, SPEED, 0} * mathy::vec3<float> {deltaTime, deltaTime, 0};
        }

        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            sq.position = sq.position + mathy::vec3<float>::LEFTV2() * mathy::vec3<float> {SPEED, SPEED, 0} * mathy::vec3<float> {deltaTime, deltaTime, 0};
        } else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            sq.position = sq.position + mathy::vec3<float>::RIGHTV2() * mathy::vec3<float> {SPEED, SPEED, 0} * mathy::vec3<float> {deltaTime, deltaTime, 0};
        }

        // MOVE TEXTURE
        float move_border = (float)SCREEN_WIDTH - (float)tex.size;
        if (tex.position.x < move_border) {
            tex.position.x += 50.0f * deltaTime;
            tex.rotation_angle += 100.0f * deltaTime;
        }

        int mouseX, mouseY;
        Uint32 mouseState = SDL_GetMouseState(&mouseX, &mouseY);

        if (distance(mouseX, mouseY, tex.position.x, tex.position.y) < tex.size) {
            tex.size = 120;
        }
        else {
            tex.size = 80;
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
        mouse.render_square();

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
