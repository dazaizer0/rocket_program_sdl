#include "config.hpp"

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
    const float air_resistance_factor = 0.98f;
    bool engine_enable = true;
    bool grounded{};

public:
    yume::vec2<float> position;
    yume::vec2<float> size;
    yume::vec2<float> velocity;
    SDL_Texture* rocketTexture;
    float rotation;
    float thrust;
    float gravity;
    float thrustPower;
    float rotationalVelocity;

    Rocket(yume::vec2<float> position_v, yume::vec2<float> size_v, SDL_Renderer* renderer)
        : position(position_v), size(size_v), velocity(yume::vec2<float>(0, 0)), rotation(90), thrust(0), gravity(9.81), thrustPower(1.0), rotationalVelocity(0.0f) {
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

        rotationalVelocity = rotationalVelocity * air_resistance_factor;
        rotation = rotation + rotationalVelocity * deltaTime;

        if (position.y > 600 - size.y) {
            position.y = 600 - size.y;
            velocity = yume::vec2<float>::ZERO();
            grounded = true;
        }
        else {
            grounded = false;
        }

        if (grounded) {
            if (rotation > 105 && rotation < 180) {
                rotationalVelocity += 0.6f;
            }
            else if (rotation < 75 && rotation > 0) {
                rotationalVelocity -= 0.6f;
            }
            else if (rotation >= 75 && rotation <= 105) {
                if (rotation > 90) {
                    rotationalVelocity -= 0.2f;
                }
                else if (rotation < 90) {
                    rotationalVelocity += 0.2f;
                }
            }
        }
    }

    void render(SDL_Renderer* renderer) {
        SDL_Rect rocketRect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
        SDL_RenderCopyEx(renderer, rocketTexture, nullptr, &rocketRect, rotation - 90, nullptr, SDL_FLIP_NONE);
    }

    void increaseThrust() {
        if (thrust < max_thrust) {
            thrust += 0.9f;
        }
    }

    void decreaseThrust() {
        if (thrust > 0) {
            thrust -= 1.0f;
        }
    }

    void rotateLeft() {
        if (!grounded) {
            rotationalVelocity -= 3.6f;
        }
    }

    void rotateRight() {
        if (!grounded) {
            rotationalVelocity += 3.6f;
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
        std::cout << "\n-> Rocket's Stats: \n";
        std::cout << "> Velocity: x: " << velocity.x << ", y: " << velocity.y << '\n';
        std::cout << "> Actual Position: x:" << position.x << ", y: " << position.y << '\n';
        std::cout << "> Thrust: " << thrust << '\n';
        std::cout << "> Rotation: " << rotation << '\n';
        std::cout << "> Rotational Velocity: " << rotationalVelocity << '\n';
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
    TTF_Font* font = TTF_OpenFont("IBMPlexSans-Medium.ttf", 24);
    SDL_Color textColor = { 255, 255, 255, 255 };
    SDL_Surface* textSurface = TTF_RenderText_Solid(font, "Press SPACE to continue!", textColor);
    SDL_Texture* textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

public:
    Menu(SDL_Renderer* rend, SDL_Window* wind, SceneManager* mgr)
        : Scene(rend, wind, mgr) {}

    virtual void start() override {
        std::cout << "THE MENU SCENE HAS BEEN STARTED\n";
        SDL_FreeSurface(textSurface);
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

        SDL_SetRenderDrawColor(renderer, 15, 90, 45, 255);

        int textWidth, textHeight;
        SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

        SDL_Rect renderQuad = { (640 - textWidth) / 2, (480 - textHeight) / 2, textWidth, textHeight };

        SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);

        SDL_RenderPresent(renderer);
    }
};

class Game : public Scene {
protected:
    yume::vec2<int> mousePos{ yume::vec2<int>::ZERO() };
    Uint32 lastTime{};

    Rocket* rocket = new Rocket(yume::vec2<float>{ 500, 500 }, yume::vec2<float>{ 32, 64 }, renderer);

    float timer{};

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
        if (timer >= 1.0f) {
            rocket->printLog();
            timer = 0.0f;
        }
    }

    virtual void render() override {
        SDL_SetRenderDrawColor(renderer, 25, 10, 95, 255);
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
    TTF_Init();

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
