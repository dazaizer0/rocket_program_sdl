#include "config.hpp"

class SceneManager;

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

    virtual ~Scene() {

    }
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
    Text* titleText = new Text(yume::vec2<int>{ 150, 60 }, 50, SDL_Color{ 255, 255, 255, 255 }, "  The Rocket Program ", renderer);
    Text* creatorText = new Text(yume::vec2<int>{ 125, 545 }, 18, SDL_Color{ 255, 255, 255, 255 }, " >      The game is made by dazai. Credits -> Art: Emilia Piekarska ", renderer);
    Text* pressText = new Text(yume::vec2<int>{ 125, 565 }, 18, SDL_Color{ 255, 255, 255, 255 }, "Select option by pressing space, switch options by pressing arrows.", renderer);
    Text* startText = new Text(yume::vec2<int>{ 360, 240 }, 32, SDL_Color{ 255, 255, 255, 255 }, "Start", renderer);
    Text* quitText = new Text(yume::vec2<int>{ 360, 300 }, 32, SDL_Color{ 255, 255, 255, 255 }, "Quit", renderer);
    int selectedOptionIndex = 0;

public:
    Menu(SDL_Renderer* rend, SDL_Window* wind, SceneManager* mgr)
        : Scene(rend, wind, mgr) {}

    virtual void start() override {
        std::cout << "THE MENU SCENE HAS BEEN STARTED\n";
        selectedOptionIndex = 0;
    }

    virtual void handleEvents(SDL_Event& event) override {
        const Uint8* state_1 = SDL_GetKeyboardState(NULL);
        Uint32 mouse_state_1 = SDL_GetMouseState(&mousePos.x, &mousePos.y);

        if (event.type == SDL_QUIT || state_1[SDL_SCANCODE_ESCAPE]) {
            quitScene();
        }

        if (state_1[SDL_SCANCODE_SPACE] && selectedOptionIndex == 0) {
            manager->switchScene(1);
        }
        else if (state_1[SDL_SCANCODE_SPACE] && selectedOptionIndex == 1) {
            quitScene();
        }

        if (state_1[SDL_SCANCODE_UP] && selectedOptionIndex == 1) {
            selectedOptionIndex = 0;
        }
        else if (state_1[SDL_SCANCODE_DOWN] && selectedOptionIndex == 0) {
            selectedOptionIndex = 1;
        }
    }

    virtual void update() override {
        if (selectedOptionIndex == 0) {
            startText->updateText("> Start", { 255, 255, 255, 255 },  renderer);
            quitText->updateText("Quit", { 255, 255, 255, 255 }, renderer);
        }
        else if (selectedOptionIndex == 1) {
            quitText->updateText("> Quit", { 255, 255, 255, 255 },  renderer);
            startText->updateText("Start", { 255, 255, 255, 255 }, renderer);
        }
    }

    virtual void render() override {
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 15, 90, 45, 255);

        pressText->render(renderer);
        startText->render(renderer);
        quitText->render(renderer);
        creatorText->render(renderer);
        titleText->render(renderer);

        SDL_RenderPresent(renderer);
    }

    ~Menu() {
        delete pressText;
        delete startText;
        delete quitText;
        delete creatorText;
        delete titleText;
    }
};

class Game : public Scene {
protected:
    yume::vec2<int> mousePos{ yume::vec2<int>::ZERO() };
    Uint32 lastTime{};

    std::random_device rd;
    std::mt19937 gen{ rd() };
    std::uniform_int_distribution<> dis{ 100, 350 };
    std::uniform_int_distribution<> dis2{ 100, 600 };


    Rocket* rocket = new Rocket(yume::vec2<float>{ 500, 400 }, yume::vec2<float>{ 32, 64 }, renderer);
    Earth* earth = new Earth(yume::vec2<float>{ 0, 500 }, yume::vec2<float>{ 1000, 1000 }, renderer);
    Island* island = new Island(yume::vec2<float>{ 200, 320 }, yume::vec2<float>{ 100, 66 }, renderer);
    Texture* background = new Texture(yume::vec2<float>{ 0, 0 }, yume::vec2<float>{ 800, 600 }, "background.png", renderer);

    Text* thrustText = new Text(yume::vec2<int>{ 5, 15 }, 24, { 255, 255, 255, 255 }, "Thrust: ", renderer);
    Text* velocityText = new Text(yume::vec2<int>{ 5, 40 }, 24, { 255, 255, 255, 255 }, "Velocity: ", renderer);
    Text* engineText = new Text(yume::vec2<int>{ 5, 65 }, 24, { 255, 255, 255, 255 }, "Engine: ", renderer);
    Text* rotationText = new Text(yume::vec2<int>{ 5, 90 }, 24, { 255, 255, 255, 255 }, "Rotation: ", renderer);
    Text* heightText = new Text(yume::vec2<int>{ 5, 115 }, 24, { 255, 255, 255, 255 }, "Height: ", renderer);
    Text* winStreakText = new Text(yume::vec2<int>{ 5, 150 }, 24, { 255, 255, 255, 255 }, "Win Streak: ", renderer);

    Text* winCounterText = new Text(yume::vec2<int>{ 350, 300 }, 32, { 0, 0, 0, 255 }, "3.0", renderer);
    Text* winText = new Text(yume::vec2<int>{ 325, 300 }, 36, { 0, 0, 0, 255 }, "YOU WON!", renderer);
    Text* winText2 = new Text(yume::vec2<int>{ 325, 340 }, 16, { 0, 0, 0, 255 }, "press R to restart level", renderer);

    Text* lossText = new Text(yume::vec2<int>{ 326, 300 }, 36, { 0, 0, 0, 255 }, "YOU LOST..", renderer);
    Text* lossText2 = new Text(yume::vec2<int>{ 330, 340 }, 16, { 0, 0, 0, 255 }, "press R to restart level", renderer);

    float timer{};
    float win_timer{};
    int winStreak = 0;
    bool win = false;
    bool lost = false;

public:
    Game(SDL_Renderer* rend, SDL_Window* wind, SceneManager* mgr)
        : Scene(rend, wind, mgr) {
    }

    void restartProgress() {
        rocket->position = yume::vec2<float>{ 500, 400 };
        rocket->rotation = 90;
        island->position = yume::vec2<float>{ static_cast<float>(dis(gen)), static_cast<float>(dis(gen)) };
        if (win) {
            island->size = yume::vec2<float>{ island->size.x - 6.5f, island->size.y - 6.5f };
            winStreak += 1;
        }

        if (lost) winStreak = 0;

        win = false;
        lost = false;
    }

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

        if (event.button.button == SDL_BUTTON_LEFT) {
            rocket->position = yume::vec2<float>{ (float)mousePos.x, (float)mousePos.y };
        }

        if (state_1[SDL_SCANCODE_ESCAPE]) {
            manager->switchScene(0);
        }

        if (state_1[SDL_SCANCODE_R]) { // RESTART SCENE
            restartProgress();
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
        island->update(&rocket->position, &rocket->size, &rocket->velocity, &rocket->grounded, &rocket->on_island, std::bind(&Rocket::levelOut, rocket));

        if (rocket->grounded && rocket->on_island && rocket->is_stable) {
            win_timer += 1 * deltaTime;
            winCounterText->updateText(std::to_string(4.0f - win_timer), { 0, 0, 0, 255 }, renderer);
        }
        else {
            win_timer = 0.0f;
        }
        // earth->update(deltaTime);

        thrustText->updateText(std::string("Thrust: ") + std::to_string(rocket->thrust), { 255, 255, 255, 255 },  renderer);
        velocityText->updateText(std::string("Velocity: ") + std::to_string(rocket->velocity.length()), { 255, 255, 255, 255 }, renderer);
        if (rocket->getEngineState()) {
            engineText->updateText(std::string("Engine: On"), { 255, 255, 255, 255 }, renderer);
        }
        else {
            engineText->updateText(std::string("Engine: Off"), { 255, 0, 100, 255 }, renderer);
        }
        rotationText->updateText(std::string("Rotation: ") + std::to_string(rocket->rotation), { 255, 255, 255, 255 }, renderer);
        heightText->updateText(std::string("Height: ") + std::to_string(abs(550 - rocket->position.y) - 14), { 255, 255, 255, 255 }, renderer);
        winStreakText->updateText(std::string("Win Streak: ") + std::to_string(winStreak), { 255, 200, 200, 255 }, renderer);

        timer += 1.0f * deltaTime;
        if (timer >= 2.0f) {
            rocket->printLog();
            timer = 0.0f;

            std::cout << "\n WIN: " << win << '\n';
            std::cout << " LOSS: " << lost << '\n';
        }

        if (rocket->position.x > 230.0f && rocket->position.x < 320.0f) {
            rocket->is_stable = false;
        }
        else if (rocket->position.x > 620.0f && rocket->position.x < 680.0f) {
            rocket->is_stable = false;
        }
    }

    virtual void render() override {
        SDL_SetRenderDrawColor(renderer, 25, 10, 95, 255);
        SDL_RenderClear(renderer);

        background->render(renderer);

        rocket->render(renderer);
        island->render(renderer);
        // earth->render(renderer);
        thrustText->render(renderer);
        velocityText->render(renderer);
        engineText->render(renderer);
        rotationText->render(renderer);
        heightText->render(renderer);
        winStreakText->render(renderer);

        if (win_timer >= 4.0f) {
            winText->render(renderer);
            winText2->render(renderer);
            win = true;
        }

        if (rocket->grounded && rocket->on_island && rocket->is_stable && win_timer < 4.0f) {
            winCounterText->render(renderer);
        }

        if (rocket->grounded && !rocket->is_stable) {
            lossText->render(renderer);
            lossText2->render(renderer);
            lost = true;
        }

        SDL_RenderPresent(renderer);
    }

    ~Game() {
        delete rocket;
        delete earth;
        delete island;
        delete background;
        delete thrustText;
        delete velocityText;
        delete engineText;
        delete rotationText;
        delete heightText;
        delete winText;
        delete winText2;
        delete lossText;
        delete lossText2;
        delete winStreakText;
        delete winCounterText;
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
