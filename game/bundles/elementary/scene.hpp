#ifndef YUMESDL_SCENE_HPP
#define YUMESDL_SCENE_HPP

#include "../../config.hpp"

class Scene {
protected:
    SDL_Renderer* renderer;
    SDL_Window* window;
    bool fullscreen;
    bool quit;

public:
    Scene(SDL_Renderer* rend, SDL_Window* win) : renderer(rend), window(win), quit(false) {}

    virtual void start() {}
    virtual void handleEvents(SDL_Event& event) {}
    virtual void update() {}
    virtual void render() {}

    void quitScene() {
        fullscreen = false;
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

#endif