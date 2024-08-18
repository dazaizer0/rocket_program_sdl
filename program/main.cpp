#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#if (WIN32)
#include <SDL_image.h>
#include <SDL_mixer.h>
#else
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#endif

#include <iostream>
#include <vector>
#include <memory>

#include "packages/math/math.hpp"

class SceneManager;

SDL_Texture* LoadTexture(const char* file, SDL_Renderer* ren) {
    SDL_Surface* surface = IMG_Load(file);
    if (surface == nullptr) {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);
    return texture;
}

class Rocket {
private:
    const float max_thrust = 14.0;
    bool engine_enable = true;
    bool grounded;

public:
    yume::vec2<float> position;
    yume::vec2<float> size;
    yume::vec2<float> velocity;
    SDL_Texture* rocketTexture;
    float rotation;
    float thrust;
    float gravity;
    float thrustPower;

    Rocket(yume::vec2<float> position_v, yume::vec2<float> size_v, SDL_Renderer* renderer)
        : position(position_v), size(size_v), velocity(yume::vec2<float>(0, 0)), rotation(90), thrust(0), gravity(9.81), thrustPower(1.0) {
        rocketTexture = LoadTexture("rocket.png", renderer);
    }

    void update(float deltaTime) {
        float radians = rotation * M_PI / 180.0f;
        yume::vec2<float> thrustForce(cos(radians) * thrust, sin(radians) * thrust);

        if (engine_enable) {
            velocity = velocity - thrustForce * deltaTime;
        }
        velocity.y = velocity.y + gravity * deltaTime;

        position = position + velocity * deltaTime;

        if (position.y > 600 - size.y) {
            position.y = 600 - size.y;
            velocity = yume::vec2<float>::ZERO();
            grounded = true;
        }
        else {
            grounded = false;
        }
    }


    void render(SDL_Renderer* renderer) {
        SDL_Rect rocketRect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
        SDL_RenderCopyEx(renderer, rocketTexture, nullptr, &rocketRect, rotation - 90, nullptr, SDL_FLIP_NONE);
    }

    void increaseThrust() {
        if (thrust < max_thrust) {
            thrust += 0.065f;
        }
    }

    void decreaseThrust() {
        if (thrust > 0) {
            thrust -= 0.065f;
        }
    }

    void rotateLeft() {
        if (!grounded) {
            rotation -= 3.6f;
        }
    }

    void rotateRight() {
        if (!grounded) {
            rotation += 3.6f;
        }
    }

    void turnOnEngine() {
        std::cout << "The engine has been turned on.\n";
        engine_enable = true;
    }

    void turnOffEngine() {
        std::cout << "The engine has been turned off.\n";
        thrust = 0.0f;
        engine_enable = false;
    }

    void printLog() {
        std::cout << "Rocket's Stats: \n";
        std::cout << "> Velocity: x: " << velocity.x << ", y: " << velocity.y << '\n';
        std::cout << "> Actual Position: x:" << position.x << ", y: " << position.y << '\n';
        std::cout << "> Thrust: " << thrust << '\n';
        std::cout << "> Rotation: " << rotation << '\n';
        std::cout << "> Engine: " << engine_enable << '\n';
    }
};


class Scene {
protected:
    SDL_Renderer* renderer;
    SDL_Window* window;
    bool quit;
    SceneManager* manager;

public:
    Scene(SDL_Renderer* rend, SDL_Window* win, SceneManager* mgr)
        : renderer(rend), window(win), quit(false), manager(mgr) {}

    virtual void start() {}
    virtual void handleEvents(SDL_Event& event) {}
    virtual void update() {}
    virtual void render() {}

    virtual bool isQuit() const {
        return quit;
    }

    void quitScene() {
        quit = true;
    }

    virtual ~Scene() {}
};

class SceneManager {
private:
    SDL_Renderer* renderer;
    SDL_Window* window;
    std::vector<std::unique_ptr<Scene>> scenes;
    int currentSceneIndex;

public:
    SceneManager(SDL_Renderer* rend, SDL_Window* win)
        : renderer(rend), window(win), currentSceneIndex(0) {}

    template<typename T, typename... Args>
    void addScene(Args&&... args) {
        scenes.push_back(std::make_unique<T>(renderer, window, this, std::forward<Args>(args)...));
    }

    void switchScene(int index) {
        if (index >= 0 && index < scenes.size()) {
            currentSceneIndex = index;
            scenes[currentSceneIndex]->start();
        }
    }

    void run() {
        if (!scenes.empty()) {
            scenes[currentSceneIndex]->start();

            while (!scenes[currentSceneIndex]->isQuit()) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    scenes[currentSceneIndex]->handleEvents(event);
                }

                scenes[currentSceneIndex]->update();

                scenes[currentSceneIndex]->render();
            }
        }

        std::cout << "THE PROGRAM HAS BEEN ENDED\n";
    }
};

class Menu : public Scene {
protected:
    yume::vec2<int> mousePos{ yume::vec2<int>::ZERO() };

public:
    Menu(SDL_Renderer* rend, SDL_Window* wind, SceneManager* mgr)
        : Scene(rend, wind, mgr) {}

    virtual void start() override {
        std::cout << "THE MENU SCENE HAS BEEN STARTED\n";
    }

    virtual void handleEvents(SDL_Event& event) override {
        const Uint8* state_1 = SDL_GetKeyboardState(NULL);
        Uint32 mouse_state_1 = SDL_GetMouseState(&mousePos.x, &mousePos.y);

        if (event.type == SDL_QUIT || state_1[SDL_SCANCODE_ESCAPE]) {
            quitScene();
        }

        if (state_1[SDL_SCANCODE_SPACE]) {
            manager->switchScene(1);
        }
    }

    virtual void update() override {

    }

    virtual void render() override {
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 100, 0, 0, 255);

        SDL_RenderPresent(renderer);
    }
};

class Game : public Scene {
protected:
    yume::vec2<int> mousePos{ yume::vec2<int>::ZERO() };
    Uint32 lastTime;

    Rocket* rocket = new Rocket(yume::vec2<float>{ 500, 500 }, yume::vec2<float>{ 20, 50 }, renderer);

    float timer;

public:
    Game(SDL_Renderer* rend, SDL_Window* wind, SceneManager* mgr)
        : Scene(rend, wind, mgr) {}

    virtual void start() override {
        std::cout << "THE GAME SCENE HAS BEEN STARTED\n";
        lastTime = SDL_GetTicks();
    }

    virtual void handleEvents(SDL_Event& event) override {
        const Uint8* state_1 = SDL_GetKeyboardState(NULL);
        const Uint8* state_2 = SDL_GetKeyboardState(NULL);

        Uint32 mouse_state_1 = SDL_GetMouseState(&mousePos.x, &mousePos.y);

        if (event.type == SDL_QUIT || state_1[SDL_SCANCODE_ESCAPE]) {
            quitScene();
        }

        if (state_1[SDL_SCANCODE_W]) { 
            rocket->increaseThrust(); 
        }
        else if (state_1[SDL_SCANCODE_S]) {
            rocket->decreaseThrust(); 
        }
        else if (state_1[SDL_SCANCODE_UP]) {
            rocket->turnOnEngine();
        }
        else if (state_1[SDL_SCANCODE_DOWN]) {
            rocket->turnOffEngine();
        }

        if (state_2[SDL_SCANCODE_A]) {
            rocket->rotateLeft(); 
        }
        else if (state_2[SDL_SCANCODE_D]) {
            rocket->rotateRight(); 
        }
    }

    virtual void update() override {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        rocket->update(deltaTime);

        timer += 1.0f * deltaTime;
        if (timer >= 3.0f) {
            rocket->printLog();
            timer = 0.0f;
        }
    }

    virtual void render() override {
        SDL_SetRenderDrawColor(renderer, 0, 100, 0, 255);
        SDL_RenderClear(renderer);

        rocket->render(renderer);

        SDL_RenderPresent(renderer);
    }

    ~Game() {
        delete rocket;
    }
};

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_Window* window = SDL_CreateWindow("yume", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SceneManager sceneManager(renderer, window);
    sceneManager.addScene<Menu>();
    sceneManager.addScene<Game>();

    sceneManager.run();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
