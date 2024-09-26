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
    bool quit;

public:
    SceneManager(SDL_Renderer* rend, SDL_Window* win)
        : renderer(rend), window(win), currentSceneIndex(0), quit(false) {}

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

            while (!quit && !scenes[currentSceneIndex]->isQuit()) {
                SDL_Event event;
                while (SDL_PollEvent(&event)) {
                    if (event.type == SDL_QUIT) {
                        quitProgram();
                    }
                    scenes[currentSceneIndex]->handleEvents(event);
                }

                scenes[currentSceneIndex]->update();
                scenes[currentSceneIndex]->render();
            }
        }
    }

    void quitProgram() {
        quit = true;
    }

    int getCurrentSceneIndex() {
        return currentSceneIndex;
    }
};

class Menu : public Scene {
protected:
    // UI Elements
    std::unique_ptr<Text> titleText;
    std::unique_ptr<Text> creatorText;
    std::unique_ptr<Text> pressText;
    std::unique_ptr<Text> startText;
    std::unique_ptr<Text> quitText;
    std::unique_ptr<Text> htpText;
    std::unique_ptr<Texture> background;
    std::unique_ptr<Texture> howToPlay;

    // State Management
    int selectedOptionIndex{ 0 };
    bool howToPlayVisible{ false };

public:
    Menu(SDL_Renderer* rend, SDL_Window* wind, SceneManager* mgr)
        : Scene(rend, wind, mgr),
        titleText(std::make_unique<Text>(yume::vec2<int>{ 150, 60 }, 50, SDL_Color{ 250, 250, 250, 255 }, "  The Rocket Program ", renderer)),
        creatorText(std::make_unique<Text>(yume::vec2<int>{ 125, 545 }, 18, SDL_Color{ 255, 255, 255, 255 }, "The game is made by dazai. Credits: Background is made by Emilia", renderer)),
        pressText(std::make_unique<Text>(yume::vec2<int>{ 125, 565 }, 18, SDL_Color{ 255, 255, 255, 255 }, "Select option by pressing space, switch options by pressing arrows.", renderer)),
        startText(std::make_unique<Text>(yume::vec2<int>{ 360, 240 }, 32, SDL_Color{ 0, 0, 0, 255 }, "Start", renderer)),
        quitText(std::make_unique<Text>(yume::vec2<int>{ 360, 300 }, 32, SDL_Color{ 0, 0, 0, 255 }, "Quit", renderer)),
        htpText(std::make_unique<Text>(yume::vec2<int>{ 310, 360 }, 32, SDL_Color{ 0, 0, 0, 255 }, "How to play", renderer)),
        background(std::make_unique<Texture>(yume::vec2<float>{ 0, 0 }, yume::vec2<float>{ 800, 600 }, "res/background.png", renderer)),
        howToPlay(std::make_unique<Texture>(yume::vec2<float>{ 0, 0 }, yume::vec2<float>{ 800, 600 }, "res/howtoplay.png", renderer)) {
    }

    virtual void start() override {
        std::cout << "THE MENU SCENE HAS BEEN STARTED\n";
        selectedOptionIndex = 0;
    }

    virtual void handleEvents(SDL_Event& event) override {
        const Uint8* state = SDL_GetKeyboardState(NULL);

        if (event.type == SDL_QUIT || state[SDL_SCANCODE_ESCAPE]) {
            quitScene();
        }

        if (state[SDL_SCANCODE_RETURN] && howToPlayVisible) {
            howToPlayVisible = false;
        }

        if (state[SDL_SCANCODE_SPACE]) {
            if (selectedOptionIndex == 0) {
                manager->switchScene(1);
            }
            else if (selectedOptionIndex == 1) {
                manager->quitProgram();
            }
            else if (selectedOptionIndex == 2) {
                howToPlayVisible = true;
            }
        }

        if (state[SDL_SCANCODE_UP] && selectedOptionIndex == 1) {
            selectedOptionIndex = 0;
        }
        else if (state[SDL_SCANCODE_DOWN] && selectedOptionIndex == 0) {
            selectedOptionIndex = 1;
        }
        else if (state[SDL_SCANCODE_DOWN] && selectedOptionIndex == 1) {
            selectedOptionIndex = 2;
        }
        if (state[SDL_SCANCODE_UP] && selectedOptionIndex == 2) {
            selectedOptionIndex = 1;
        }
    }

    virtual void update() override {
        if (selectedOptionIndex == 0) {
            startText->updateText("> Start", { 0, 0, 0, 255 }, renderer);
            quitText->updateText("Quit", { 0, 0, 0, 255 }, renderer);
            htpText->updateText("How to play", { 0, 0, 0, 255 }, renderer);
        }
        else if (selectedOptionIndex == 1) {
            quitText->updateText("> Quit", { 0, 0, 0, 255 }, renderer);
            startText->updateText("Start", { 0, 0, 0, 255 }, renderer);
            htpText->updateText("How to play", { 0, 0, 0, 255 }, renderer);
        }
        else if (selectedOptionIndex == 2) {
            quitText->updateText("Quit", { 0, 0, 0, 255 }, renderer);
            startText->updateText("Start", { 0, 0, 0, 255 }, renderer);
            htpText->updateText("> How to play", { 0, 0, 0, 255 }, renderer);
        }
    }

    virtual void render() override {
        SDL_RenderClear(renderer);

        SDL_SetRenderDrawColor(renderer, 15, 90, 45, 255);
        background->render(renderer);
        pressText->render(renderer);
        startText->render(renderer);
        quitText->render(renderer);
        htpText->render(renderer);
        creatorText->render(renderer);
        titleText->render(renderer);

        if (howToPlayVisible == true) howToPlay->render(renderer);

        SDL_RenderPresent(renderer);
    }

    ~Menu() = default;
};


class Game : public Scene {
protected:
    yume::vec2<int> mousePos{ yume::vec2<int>::ZERO() };
    Uint32 lastTime{};

    std::random_device rd;
    std::mt19937 gen{ rd() };
    std::uniform_int_distribution<> dis_x{ 100, 400 };
    std::uniform_int_distribution<> dis_y{ 100, 350 };

    // Game objects
    Rocket* rocket;
    Texture* rocketBoosterAnim;

    std::vector<std::string> rocketBoosterAnimFiles{ "res/booster1.png", "res/booster2.png", "res/booster3.png" };
    std::unique_ptr<Island> island;
    std::unique_ptr<Texture> airstrip;
    std::unique_ptr<Texture> background;

    // UI
    std::unique_ptr<Text> thrustText;
    std::unique_ptr<Text> velocityText;
    std::unique_ptr<Text> engineText;
    std::unique_ptr<Text> rotationText;
    std::unique_ptr<Text> heightText;
    std::unique_ptr<Text> winStreakText;
    std::unique_ptr<Text> stageText;
    std::unique_ptr<Text> turnOnEngineText;
    std::unique_ptr<Text> winCounterText;
    std::unique_ptr<Text> winText;
    std::unique_ptr<Text> winText2;
    std::unique_ptr<Text> winText3;
    std::unique_ptr<Text> lossText;
    std::unique_ptr<Text> lossText2;

    // Audio
    Mix_Chunk* woosh{};
    Mix_Chunk* booster{};

    // Other variables
    int islandStage{ 0 };
    float islandX2Right{};
    float islandX2Left{};
    bool movingRight{ false };
    float timer{};
    float win_timer{};
    int winStreak{ 0 };
    bool win{ false };
    bool winPredict{ false };
    bool lost{ false };
    int channel{ -1 };
    bool engineNotification{ false };
    bool uiEnabled{ true };
    bool keyPressedLastFrame{ false };
    float restartTimer = 0.0f;

public:
    Game(SDL_Renderer* rend, SDL_Window* wind, SceneManager* mgr)
        : Scene(rend, wind, mgr),
        rocket(new Rocket(yume::vec2<float>{ 575, 410 }, yume::vec2<float>{ 32, 64 }, renderer)),
        rocketBoosterAnim(new Texture(yume::vec2<float>{ rocket->position.x, rocket->position.y }, yume::vec2<float>{ 32, 64 }, "res/booster1.png", renderer)),
        island(std::make_unique<Island>(yume::vec2<float>{ 200, 320 }, yume::vec2<float>{ 100, 66 }, renderer)),
        airstrip(std::make_unique<Texture>(yume::vec2<float>{ 200, 320 }, yume::vec2<float>{ 100, 66 }, "res/airstrip.png", renderer)),
        background(std::make_unique<Texture>(yume::vec2<float>{ 0, 0 }, yume::vec2<float>{ 800, 600 }, "res/background.png", renderer)),
        thrustText(std::make_unique<Text>(yume::vec2<int>{ 5, 15 }, 24, SDL_Color{ 255, 255, 255, 255 }, "Thrust: ", renderer)),
        velocityText(std::make_unique<Text>(yume::vec2<int>{ 5, 40 }, 24, SDL_Color{ 255, 255, 255, 255 }, "Velocity: ", renderer)),
        engineText(std::make_unique<Text>(yume::vec2<int>{ 5, 65 }, 24, SDL_Color{ 255, 255, 255, 255 }, "Engine: ", renderer)),
        rotationText(std::make_unique<Text>(yume::vec2<int>{ 5, 90 }, 24, SDL_Color{ 255, 255, 255, 255 }, "Rotation: ", renderer)),
        heightText(std::make_unique<Text>(yume::vec2<int>{ 5, 115 }, 24, SDL_Color{ 255, 255, 255, 255 }, "Height: ", renderer)),
        winStreakText(std::make_unique<Text>(yume::vec2<int>{ 5, 140 }, 24, SDL_Color{ 255, 255, 255, 255 }, "Win Streak: ", renderer)),
        stageText(std::make_unique<Text>(yume::vec2<int>{ 5, 165 }, 24, SDL_Color{ 255, 255, 255, 255 }, "Stage: ", renderer)),
        turnOnEngineText(std::make_unique<Text>(yume::vec2<int>{ 260, 100 }, 32, SDL_Color{ 255, 0, 0, 255 }, "TURN ON THE ENGINE!", renderer)),
        winCounterText(std::make_unique<Text>(yume::vec2<int>{ 350, 300 }, 32, SDL_Color{ 0, 0, 0, 255 }, "3.0", renderer)),
        winText(std::make_unique<Text>(yume::vec2<int>{ 325, 300 }, 36, SDL_Color{ 0, 0, 0, 255 }, "YOU WON!", renderer)),
        winText2(std::make_unique<Text>(yume::vec2<int>{ 335, 345 }, 16, SDL_Color{ 0, 0, 0, 255 }, "press R to continue!", renderer)),
        winText3(std::make_unique<Text>(yume::vec2<int>{ 260, 360 }, 16, SDL_Color{ 0, 0, 0, 255 }, "Press R to continue and thanks for playing!", renderer)),
        lossText(std::make_unique<Text>(yume::vec2<int>{ 326, 300 }, 36, SDL_Color{ 0, 0, 0, 255 }, "YOU LOST..", renderer)),
        lossText2(std::make_unique<Text>(yume::vec2<int>{ 330, 335 }, 16, SDL_Color{ 0, 0, 0, 255 }, "press R to restart level..", renderer)) {
    }

    void restartProgress() {
        rocket->position = yume::vec2<float>{ 575, 410 };
        rocket->velocity = yume::vec2<float>::ZERO();
        rocket->previousVelocity = yume::vec2<float>::ZERO();
        rocket->rotation = 90;
        rocketBoosterAnim->position = yume::vec2<float>{ rocket->position.x, rocket->position.y };
        island->position = yume::vec2<float>{ static_cast<float>(dis_x(gen)), static_cast<float>(dis_y(gen)) };
        airstrip->size = island->size;
        airstrip->position = island->position;

        if (win) {
            island->size = yume::vec2<float>{ island->size.x - 6.5f, island->size.y - 6.5f };
            winStreak += 1;
            islandStage += 1;
        }
        if (lost) winStreak = 0;

        if (!lost && !win && winPredict) {
            winPredict = false;
        }

        win = false;
        lost = false;
        rocket->on_island = false;

        if (islandStage >= 2 && islandStage <= 4) {
            islandX2Left = island->position.x - 50.0f;
            islandX2Right = island->position.x + 50.0f;
        }
    }

    virtual void start() override {
        std::cout << "THE GAME SCENE HAS BEEN STARTED\n";
        lastTime = SDL_GetTicks();

        woosh = Mix_LoadWAV("res/woosh.wav");
        booster = Mix_LoadWAV("res/booster.wav");
    }

    virtual void handleEvents(SDL_Event& event) override {
        const Uint8* state = SDL_GetKeyboardState(NULL);
        Uint32 mouse_state = SDL_GetMouseState(&mousePos.x, &mousePos.y);

        if (event.type == SDL_QUIT) {
            quitScene();
        }

        if (state[SDL_SCANCODE_ESCAPE]) {
            manager->switchScene(0);
        }

        if (state[SDL_SCANCODE_R] && restartTimer == 1.0f) { // Restart scene
            restartProgress();
            restartTimer = 0.0f;
        }

        if (state[SDL_SCANCODE_W]) {
            rocket->increaseThrust();
        }
        else if (state[SDL_SCANCODE_S]) {
            rocket->decreaseThrust();
        }
        else if (state[SDL_SCANCODE_UP]) {
            Mix_PlayChannel(-1, woosh, 0);
            rocket->turnOnEngine();
        }
        else if (state[SDL_SCANCODE_DOWN]) {
            Mix_PlayChannel(-1, woosh, 0);
            rocket->turnOffEngine();
        }

        if (state[SDL_SCANCODE_A]) {
            rocket->rotateLeft();
        }
        else if (state[SDL_SCANCODE_D]) {
            rocket->rotateRight();
        }

        if (state[SDL_SCANCODE_W] && !rocket->engine_enable) {
            engineNotification = true;
        }
        else {
            engineNotification = false;
        }

        if (state[SDL_SCANCODE_U]) {
            if (!keyPressedLastFrame) {
                uiEnabled = !uiEnabled;
            }
            keyPressedLastFrame = true;
        }
        else {
            keyPressedLastFrame = false;
        }
    }

    virtual void update() override {
        Uint32 currentTime = SDL_GetTicks();
        float deltaTime = (currentTime - lastTime) / 1000.0f;
        lastTime = currentTime;

        restartTimer += 1 * deltaTime;
        if (restartTimer >= 1.0f) {
            restartTimer = 1.0f;
        }

        rocket->update(deltaTime);
        if (islandStage <= 9) {
            island->update(&rocket->position, &rocket->size, &rocket->velocity, &rocket->grounded, &rocket->on_island, std::bind(&Rocket::levelOut, rocket));
            airstrip->position = island->position;
        }

        if (islandStage >= 2 && islandStage <= 4) {
            if (!rocket->on_island) {
                if (island->position.x >= islandX2Right) {
                    movingRight = false;
                }
                else if (island->position.x <= islandX2Left) {
                    movingRight = true;
                }

                if (movingRight) {
                    island->position.x += deltaTime * islandStage * 5.0f;
                }
                else {
                    island->position.x -= deltaTime * islandStage * 5.0f;
                }
            }
        }

        if (winPredict) {
            win_timer += 1 * deltaTime;
            winCounterText->updateText(std::to_string(4.0f - win_timer), SDL_Color{ 0, 0, 0, 255 }, renderer);
        }
        else {
            win_timer = 0.0f;
        }

        thrustText->updateText(std::string("Thrust: ") + std::to_string(rocket->thrust), SDL_Color{ 255, 255, 255, 255 }, renderer);
        velocityText->updateText(std::string("Velocity: ") + std::to_string(rocket->velocity.length()), SDL_Color{ 255, 255, 255, 255 }, renderer);
        if (rocket->getEngineState()) {
            engineText->updateText(std::string("Engine: On"), SDL_Color{ 255, 255, 255, 255 }, renderer);
        }
        else {
            engineText->updateText(std::string("Engine: Off"), SDL_Color{ 255, 0, 100, 255 }, renderer);
        }
        rotationText->updateText(std::string("Rotation: ") + std::to_string(rocket->rotation), SDL_Color{ 255, 255, 255, 255 }, renderer);
        heightText->updateText(std::string("Height: ") + std::to_string(abs(550 - rocket->position.y) - 14), SDL_Color{ 255, 255, 255, 255 }, renderer);
        winStreakText->updateText(std::string("Win Streak: ") + std::to_string(winStreak), SDL_Color{ 255, 200, 200, 255 }, renderer);
        stageText->updateText(std::string("Stage: ") + std::to_string(islandStage), SDL_Color{ 255, 255, 255, 255 }, renderer);

        float radianRotation = (rocket->rotation - 90) * (M_PI / 180.0f);

        float boosterOffsetX = cos(radianRotation) * 0 - sin(radianRotation) * (rocket->size.y / 2.0f + rocketBoosterAnim->size.y / 2.0f - 42.0f);
        float boosterOffsetY = sin(radianRotation) * 0 + cos(radianRotation) * (rocket->size.y / 2.0f + rocketBoosterAnim->size.y / 2.0f - 42.0f);

        rocketBoosterAnim->position = yume::vec2<float>{ rocket->position.x + boosterOffsetX, rocket->position.y + boosterOffsetY };
        rocketBoosterAnim->rotation = rocket->rotation;

        rocketBoosterAnim->update(rocketBoosterAnimFiles, 0.2f, deltaTime, renderer);

        Mix_VolumeChunk(booster, rocket->thrust * 7.5f);

        if (rocket->thrust > 1.0f && rocket->getEngineState()) {
            if (channel == -1) {
                channel = Mix_PlayChannel(-1, booster, -1);
            }
        }

        if ((!rocket->engine_enable || rocket->thrust <= 1.0f) && channel != -1) {
            Mix_HaltChannel(channel);
            channel = -1;
        }

        if (rocket->position.x > 230.0f && rocket->position.x < 320.0f && rocket->position.y > 425.0f) {
            rocket->is_stable = false;
        }
        else if (rocket->position.x > 620.0f && rocket->position.x < 680.0f && rocket->position.y > 425.0f) {
            rocket->is_stable = false;
        }

        if (win && !rocket->grounded) {
            win = false;
        }

        if (rocket->grounded == true) {
            if (rocket->is_stable == true && rocket->on_island == true && rocket->previousVelocity.length() <= 40.0f) {
                winPredict = true;

                if (islandStage == 9) {
                    winText2->updateText("CONGRATULATIONS! You've completed the game! Now you can fly your rocket around without any target!", SDL_Color{ 0, 0, 0, 255 }, renderer);
                    winText2->position = yume::vec2<int>{ 10, 345 };
                }
            }
            else {
                winPredict = false;
            }

            if (rocket->is_stable == true && rocket->on_island == true && rocket->previousVelocity.length() <= 40.0f && win_timer > 3.9f) {
                win = true;
                lost = false;
            }
            else if (rocket->is_stable == false || rocket->previousVelocity.length() > 40.0f) {
                lost = true;
                win = false;
            }
        }
        else {
            winPredict = false;
        }
    }

    virtual void render() override {
        SDL_SetRenderDrawColor(renderer, 25, 10, 95, 255);
        SDL_RenderClear(renderer);

        background->render(renderer);

        if (!rocket->grounded && rocket->engine_enable && rocket->thrust >= 2.0f) {
            rocketBoosterAnim->render(renderer);
        }
        rocket->render(renderer);

        if (islandStage <= 9) {
            island->render(renderer);
            airstrip->render(renderer);
        }

        if (uiEnabled) {
            thrustText->render(renderer);
            velocityText->render(renderer);
            engineText->render(renderer);
            rotationText->render(renderer);
            heightText->render(renderer);
            winStreakText->render(renderer);
            stageText->render(renderer);
        }

        if (win && win_timer > 4.0f) {
            winText->render(renderer);
            winText2->render(renderer);
            if (islandStage >= 9) {
                winText3->render(renderer);
            }
        }

        if (winPredict && win_timer < 4.0f) {
            winCounterText->render(renderer);
        }

        if (lost) {
            lossText->render(renderer);
            lossText2->render(renderer);
        }

        if (engineNotification && !win && !lost) {
            turnOnEngineText->render(renderer);
        }

        SDL_RenderPresent(renderer);
    }

    ~Game() {
        delete rocket;
        delete rocketBoosterAnim;
         
        if (manager->getCurrentSceneIndex() == 1) {
            Mix_FreeChunk(woosh);
            Mix_FreeChunk(booster);
        }
    }
};

int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
        std::cout << "SDL_Init Error: " << SDL_GetError() << '\n';
        return 1;
    }
    TTF_Init();

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
    Mix_Music* bgm = Mix_LoadMUS("res/8bitmusic.mp3");

    Mix_VolumeMusic(96);
    Mix_PlayMusic(bgm, -1);

    SDL_Window* window = SDL_CreateWindow("Rocket Program", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    SceneManager sceneManager(renderer, window);
    sceneManager.addScene<Menu>();
    sceneManager.addScene<Game>();

    sceneManager.run();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeMusic(bgm);
    Mix_CloseAudio();
    SDL_Quit();

    return 0;
}
