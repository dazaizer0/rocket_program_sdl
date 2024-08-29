#ifndef YUME_ROCKET
#define YUME_ROCKET

#include "../../config.hpp"

class Rocket {
private:
    const float max_thrust{ 16.0 };
    const float air_resistance_factor{ 0.98f };

    SDL_Texture* loadTexture(const char* file, SDL_Renderer* ren);

public:
    yume::vec2<float> position;
    yume::vec2<float> size;
    yume::vec2<float> velocity;
    yume::vec2<float> previousVelocity;
    SDL_Texture* rocketTexture;
    bool grounded;
    bool on_island;
    bool is_stable;
    float rotation;
    float thrust;
    float gravity;
    float thrustPower;
    float rotationalVelocity;
    bool engine_enable = true;

    Rocket(yume::vec2<float> position_v, yume::vec2<float> size_v, SDL_Renderer* renderer);
    void levelOut();
    void update(float deltaTime);
    void render(SDL_Renderer* renderer);
    bool getEngineState();
    void increaseThrust();
    void decreaseThrust();
    void rotateLeft();
    void rotateRight();
    void turnOnEngine();
    void turnOffEngine();
    void printLog();
    ~Rocket();
};

#endif