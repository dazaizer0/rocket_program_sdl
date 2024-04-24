#include "config.h"


const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SQUARE_SIZE = 50;

class Scene {
protected:
    SDL_Renderer* renderer;
    bool quit;

public:
    Scene(SDL_Renderer* rend) : renderer(rend), quit(false) {}

    virtual void handleEvents(SDL_Event& event) {};

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
            render();
        }
    }

    virtual ~Scene() {}
};

class GreenScene : public Scene {
public:
    GreenScene(SDL_Renderer* rend) : Scene(rend) {}

    virtual void handleEvents(SDL_Event& event) override {
        if (event.type == SDL_QUIT) {
            quitScene();
        }
        else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            quitScene();
        }
    }

    virtual void render() override {
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderClear(renderer);

        SDL_Rect square = { (SCREEN_WIDTH - SQUARE_SIZE) / 2, (SCREEN_HEIGHT - SQUARE_SIZE) / 2, SQUARE_SIZE, SQUARE_SIZE };
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
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

    SDL_Window* window = SDL_CreateWindow("YUMESDL", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    GreenScene greenScene(renderer);
    RedScene redScene(renderer);

    Scene* currentScene = &greenScene;
    currentScene->run();

    currentScene = &redScene;
    currentScene->run();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
