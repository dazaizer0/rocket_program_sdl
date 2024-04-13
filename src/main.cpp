#include "config.h"

#include "source/render/square.hpp"
#include "source/render/texture.hpp"
#include "source/render/objects.hpp"

const int SCREEN_WIDTH = 650;
const int SCREEN_HEIGHT = 500;

const float SPEED = 620.0f;

int to_gridint(float n, int grid_size) {
    return round(n / grid_size) * grid_size;
}

bool is_position_free(const mathy::vec2<int>& pos, const std::vector<std::vector<bool>>& grid, int grid_size) {
    if (pos.x < 0 || pos.x >= SCREEN_WIDTH || pos.y < 0 || pos.y >= SCREEN_HEIGHT)
        return false;

    int grid_x = pos.x / grid_size;
    int grid_y = pos.y / grid_size;
    return !grid[grid_x][grid_y];
}

int main(int argc, char* args[]) {
    // INITIALIZATION
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("yumesdl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    if (window == nullptr) {
        std::cerr << "failed to initialize window:\n" << SDL_GetError();
        SDL_Quit();
        return EXIT_FAILURE;
    }
    else {
        std::cout << "window initialized successfully\n";
    }

    // RENDERER
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    int grid_size = 50;

    mathy::vec2<int> mouse_pos = mathy::vec2<int>::ZERO();
    bool mouse_left_down = false;
    bool mouse_right_down = false;

    bool esc_pressed = false;

    // TEXTURE
    render::texture tex = render::texture("assets/rouge.png", mathy::vec2<float>::ZERO(), mathy::vec2<float>{100.0f, 100.0f}, 0.0f, renderer);

    // SQUARE
    render::square sq = render::square(mathy::vec2<float>{300.0f, 300.0f}, mathy::colorRGBA::BLUE(), mathy::vec2<float>{(float)grid_size, (float)grid_size}, renderer);

    // BACKGROUND
    render::texture bgtex = render::texture("assets/sonic.png", mathy::vec2<float>::ZERO(), mathy::vec2<float>{(float)grid_size, (float)grid_size}, 0.0f, renderer);

    std::vector<render::square> squares = {};
    std::vector<std::vector<bool>> grid(SCREEN_WIDTH / grid_size, std::vector<bool>(SCREEN_HEIGHT / grid_size, false));

    // MAIN LOOP
    Uint32 prevTime = SDL_GetTicks();

    bool quit = false;
    while (!quit) {
        Uint64 start = SDL_GetPerformanceCounter();

        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - prevTime) / 1000.0f;

        const Uint8* currentKeyStates = SDL_GetKeyboardState(nullptr);
        Uint8 mouse_state = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

        // EVENTS
        SDL_Event e;
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
            else if (e.type == SDL_KEYDOWN) {
                if (e.key.keysym.sym == SDLK_ESCAPE) {
                    quit = true;
                    esc_pressed = true;
                }
            }

            if (e.type == SDL_MOUSEBUTTONDOWN) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    mouse_left_down = true;
                }
                else if (e.button.button == SDL_BUTTON_RIGHT) {
                    mouse_right_down = true;
                }
            }
            else if (e.type == SDL_MOUSEBUTTONUP) {
                if (e.button.button == SDL_BUTTON_LEFT) {
                    mouse_left_down = false;
                }
                else if (e.button.button == SDL_BUTTON_RIGHT) {
                    mouse_right_down = false;
                }
            }
        }

        if (mouse_right_down && !esc_pressed && currentKeyStates[SDL_SCANCODE_1]) {
            mathy::vec2<int> grid_pos = mathy::vec2<int>(to_gridint(mouse_pos.x, grid_size), to_gridint(mouse_pos.y, grid_size));
            if (is_position_free(grid_pos, grid, grid_size)) {
                squares.push_back(render::square(mathy::vec2<float>{(float)grid_pos.x, (float)grid_pos.y}, mathy::colorRGBA::BLUE(), mathy::vec2<float>{(float)grid_size, (float)grid_size}, renderer));
                grid[grid_pos.x / grid_size][grid_pos.y / grid_size] = true;
                std::cout << "blue square has been placed\n";
            }
        }

        if (currentKeyStates[SDL_SCANCODE_UP]) {
            sq.position = sq.position + mathy::vec2<float>::UP() * mathy::vec2<float> {SPEED, SPEED} *mathy::vec2<float> {deltaTime, deltaTime};
        }
        else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
            sq.position = sq.position + mathy::vec2<float>::DOWN() * mathy::vec2<float> {SPEED, SPEED} *mathy::vec2<float> {deltaTime, deltaTime};
        }

        if (currentKeyStates[SDL_SCANCODE_LEFT]) {
            sq.position = sq.position + mathy::vec2<float>::LEFT() * mathy::vec2<float> {SPEED, SPEED} *mathy::vec2<float> {deltaTime, deltaTime};
        }
        else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
            sq.position = sq.position + mathy::vec2<float>::RIGHT() * mathy::vec2<float> {SPEED, SPEED} *mathy::vec2<float> {deltaTime, deltaTime};
        }

        if (currentKeyStates[SDL_SCANCODE_DELETE]) {
            int temp_size = squares.size();
            squares.clear();

            if (squares.size() == 0 && temp_size > 0)
                std::cout << "squares have been succesfully deleted\n";
        }

        if (mathy::distance(mouse_pos, mathy::vec2<int>{(int)tex.position.x, (int)tex.position.y}) < ((tex.size.x + tex.size.y) / 2) && mouse_state && SDL_BUTTON(SDL_BUTTON_LEFT)) {
            if (mouse_left_down) {
                tex.size = mathy::vec2<float>{ 120.0f, 120.0f };
                tex.position = mathy::vec2<float>{ (float)mouse_pos.x, (float)mouse_pos.y };
            }
            else {
                tex.size = mathy::vec2<float>{ 100.0f, 100.0f };
            }
        }

        // MOVE AND ROTATE TEXTURE
        // float move_border = (float)SCREEN_WIDTH - (float)tex.size.x;
        // if (tex.position.x < move_border) {
        //     tex.position.x += 50.0f * deltaTime;
        //     tex.rotation_angle += 100.0f * deltaTime;
        // }

        // CLEAR SCREEN
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        if (squares.size() >= 1) {
            for (int i = 0; i < squares.size(); i++) {
                squares[i].render_square();
            }
        }

        // RENDER TEXTURE
        tex.render_texture();

        // DRAW SQUARE
        sq.render_square();

        // REFRESH RENDERER
        SDL_RenderPresent(renderer);
        prevTime = currentTime;

        Uint64 end = SDL_GetPerformanceCounter();
        float elapsedMS = (end - start) / (float)SDL_GetPerformanceFrequency() * 1000.0f;
        SDL_Delay(std::max(0, (int)(16.666f - elapsedMS))); // cap fps to 60
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
