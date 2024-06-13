#include "config.h"

#include "source/render/texture.hpp"
#include "skarabeusz.hpp"


const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;

class Scene {
protected:
    SDL_Renderer* renderer;
    bool quit;

public:
    Scene(SDL_Renderer* rend) : renderer(rend), quit(false) {}

    virtual void start() {};

    virtual void handleEvents(SDL_Event& event) {};

    virtual void update() {};
    virtual void render() {};

    void quitScene() {
        quit = true;
    }

    virtual void run() {
        start();

        while (!quit) {
            SDL_Event event;
            while (SDL_PollEvent(&event)) {
                handleEvents(event);
            }
            update();

            render();
        }
    }

    virtual ~Scene() {}
};

class Game : public Scene {
protected:
    mathy::vec2<int> mouse_pos = mathy::vec2<int>::ZERO();
    bool mouse_left_down{ false };
    Uint8 mouse_state{};

    bool first_move{ true };

    render::texture bg = render::texture("res/bg.png", mathy::vec2<int>{512, 384}, mathy::vec2<int>{1024, 768}, 0.0f, renderer);

    render::texture flap = render::texture("res/x.png", mathy::vec2<int>{512, 384}, mathy::vec2<int>{1024, 1024}, 0.0f, renderer);
    bool flap_open = false;

    Skarabeusz skarabeusze[25] = {
        Skarabeusz(renderer, mathy::vec2<int>{198, 168}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{334, 168}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{511, 130}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{690, 166}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{130, 246}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{265, 243}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{410, 248}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{510, 235}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{614, 248}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{758, 248}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{245, 393}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{440, 394}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{509, 330}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{582, 398}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{782, 393}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{275, 565}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{414, 562}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{509, 486}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{513, 613}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{612, 557}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{749, 562}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{880, 566}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{341, 642}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{682, 636}, mathy::vec2<int>{80, 95}, 0.0f),
        Skarabeusz(renderer, mathy::vec2<int>{824, 642}, mathy::vec2<int>{80, 95}, 0.0f)
    };

    size_t skarabeusze_size = sizeof(skarabeusze) / sizeof(skarabeusze[0]);

    int actual_skarabeusz_index{};

public:
    Game(SDL_Renderer* rend) : Scene(rend) {}

    virtual void start() override {
        flap.enabled = false;

        skarabeusze[0].neighbours_indexes = { 4, 5 };
    }

    virtual void handleEvents(SDL_Event& event) override {
        if (event.type == SDL_QUIT) {
            quitScene();
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            quitScene();
        }

        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouse_left_down = true;
            }
        }
        else if (event.type == SDL_MOUSEBUTTONUP) {
            if (event.button.button == SDL_BUTTON_LEFT) {
                mouse_left_down = false;
            }
        }
    }

    virtual void update() override {
        mouse_state = SDL_GetMouseState(&mouse_pos.x, &mouse_pos.y);

        for (int i = 0; i < skarabeusze_size; i++) {
            if (mathy::distance(mouse_pos, skarabeusze[i].position) < ((skarabeusze[i].size.x + skarabeusze[i].size.y) / 8) && mouse_state && mouse_left_down) {
                if (skarabeusze[i].can_select || first_move) {
                    skarabeusze[i].state = selected;
                    actual_skarabeusz_index = i;
                    std::cout << i << '\n';
                    first_move = false;
                    
                    for (int j = 0; j < skarabeusze_size; j++) {
                        skarabeusze[j].can_select = false;
                    }
                }
            }
        }

        for (int i = 0; i < skarabeusze[actual_skarabeusz_index].neighbours_indexes.size(); i++) {
            skarabeusze[skarabeusze[actual_skarabeusz_index].neighbours_indexes[i]].can_select = true;
        }

        if (mouse_left_down)
            std::cout << "x: " << mouse_pos.x << " y : " << mouse_pos.y << '\n';

        for (int i = 0; i < skarabeusze_size; i++) {
            if (skarabeusze[i].state == empty) {
                flap_open = false;
            }
            else {
                flap_open = true;
            }
        }

        if (flap_open) {
            flap.enabled = true;
            skarabeusze[12].enabled = false;
        }
    }

    virtual void render() override {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        bg.render_texture();
        flap.render_texture();

        for (int i = 0; i < skarabeusze_size; i++) {
            skarabeusze[i].render();
        }

        SDL_RenderPresent(renderer);
    }
};

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("YUMESDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    Game greenScene(renderer);

    Scene* currentScene = &greenScene;
    currentScene->run();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
