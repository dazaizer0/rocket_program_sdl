#include "config.h"


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int SQUARE_SIZE = 50;

class Scene {
protected:
    SDL_Renderer* renderer;
    bool quit;

public:
    Scene(SDL_Renderer* rend) : renderer(rend), quit(false) {}

    virtual void handleEvents(SDL_Event& event) {};

    virtual void update() {};
    virtual void render() {};

    void quitScene() {
        quit = true;
    }

    virtual void run() {
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

class GreenScene : public Scene {
protected:
    mathy::vec2<int> mouse_pos = mathy::vec2<int>::ZERO();
    mathy::vec2<int> square_pos = mathy::vec2<int>{ 300, 300 };
    bool mouse_left_down{ false };
    Uint8 mouse_state{};

public:
    GreenScene(SDL_Renderer* rend) : Scene(rend) {}

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

        if (mathy::distance(mouse_pos, square_pos)) {
            if (mouse_left_down) {
                square_pos = mathy::vec2<int>{ mouse_pos.x - SQUARE_SIZE / 2, mouse_pos.y - SQUARE_SIZE / 2 };
                SQUARE_SIZE = 55;
            }
            else {
                SQUARE_SIZE = 50;
            }
        }
    }

    virtual void render() override {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect square = { square_pos.x, square_pos.y, SQUARE_SIZE, SQUARE_SIZE };
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderFillRect(renderer, &square);

        SDL_RenderPresent(renderer);
    }
};

class RedScene : public Scene {
public:
    RedScene(SDL_Renderer* rend) : Scene(rend) {}

    virtual void handleEvents(SDL_Event& event) override {
        if (event.type == SDL_QUIT) {
            quitScene();
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            quitScene();
        }
    }

    virtual void render() override {
        SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect square = { (SCREEN_WIDTH - SQUARE_SIZE) / 2, (SCREEN_HEIGHT - SQUARE_SIZE) / 2, SQUARE_SIZE, SQUARE_SIZE };
        SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
        SDL_RenderFillRect(renderer, &square);

        SDL_RenderPresent(renderer);
    }
};

int main(int argc, char* args[]) {
    SDL_Init(SDL_INIT_VIDEO);

    SDL_Window* window = SDL_CreateWindow("YUMESDL2", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    GreenScene greenScene(renderer);

    Scene* currentScene = &greenScene;
    currentScene->run();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
