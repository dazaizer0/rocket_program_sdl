#include "config.hpp"

const int SCREEN_WIDTH = 1024;
const int SCREEN_HEIGHT = 768;
bool END = false;

class Game : public Scene {
protected:
    render::texture dude = render::texture("res/dude.png", { 512, 384 }, { 256, 256 }, 0.0f, renderer);
    float moveSpeed = 5.0f;

public:
    Game(SDL_Renderer* rend, SDL_Window* wind) : Scene(rend, wind) {}

    virtual void handleEvents(SDL_Event& event) override {
        if (event.type == SDL_QUIT || event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) {
            quitScene();
            END = true;
        }
    }

    virtual void update() {
        const Uint8* keystate = SDL_GetKeyboardState(NULL);

        if (keystate[SDL_SCANCODE_W]) {
            dude.position.y -= moveSpeed;
        }
        if (keystate[SDL_SCANCODE_S]) {
            dude.position.y += moveSpeed;
        }
        if (keystate[SDL_SCANCODE_A]) {
            dude.position.x -= moveSpeed;
        }
        if (keystate[SDL_SCANCODE_D]) {
            dude.position.x += moveSpeed;
        }
        dude.update_destination_rect();
    }

    virtual void render() override {
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        dude.render_texture();

        SDL_RenderPresent(renderer);
    }
};

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        printf("SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) == -1) {
        printf("Mix_OpenAudio Error: %s\n", Mix_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("yumesdl", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    Scene* currentScene;

    while (!END) {
        Game gameScene(renderer, window);
        currentScene = &gameScene;
        currentScene->run();
    }

    Mix_CloseAudio();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

