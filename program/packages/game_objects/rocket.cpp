#include "rocket.hpp"

Rocket::Rocket(yume::vec2<float> position_v, yume::vec2<float> size_v, SDL_Renderer* renderer)
    : position(position_v), size(size_v), velocity(yume::vec2<float>(0, 0)), rotation(90), thrust(0), gravity(9.81), thrustPower(1.0), rotationalVelocity(0.0f), grounded(false), on_island(false) {
    rocketTexture = renderManager.loadTexture("res/rocket.png", renderer);
}

void Rocket::levelOut() {
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

void Rocket::update(float deltaTime) {
    float radians = rotation * M_PI / 180.0f;
    yume::vec2<float> thrustForce(cos(radians) * thrust, sin(radians) * thrust);

    if (engine_enable) {
        velocity = velocity - thrustForce * deltaTime;
    }
    velocity.y = velocity.y + gravity * deltaTime;

    position = position + velocity * deltaTime;

    rotationalVelocity = rotationalVelocity * air_resistance_factor;
    rotation = rotation + rotationalVelocity * deltaTime;

    if (position.y > 495 - size.y) {
        grounded = true;
    }
    else {
        grounded = false;
    }

    if (grounded) {
        position.y = 495 - size.y;
        velocity = yume::vec2<float>::ZERO();
        on_island = false;

        levelOut();
    }

    if (rotation > 360.0f) {
        rotation = 0.0f;
    }
    else if (rotation < 0.0f) {
        rotation = 360.0f;
    }

    if (rotation <= 105.0f && rotation >= 75.0f) {
        is_stable = true;
    }
    else {
        is_stable = false;
    }

    if (velocity.x != 0 && velocity.y != 0) {
        previousVelocity = velocity;
    }
}

void Rocket::render(SDL_Renderer* renderer) {
    SDL_Rect rocketRect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
    SDL_RenderCopyEx(renderer, rocketTexture, nullptr, &rocketRect, rotation - 90, nullptr, SDL_FLIP_NONE);
}

bool Rocket::getEngineState() {
    return engine_enable;
}

void Rocket::increaseThrust() {
    if (thrust < max_thrust) {
        thrust += 0.4f; // 0.32f
    }
}

void Rocket::decreaseThrust() {
    if (thrust > 0) {
        thrust -= 0.5f; // 0.48f
    }
}

void Rocket::rotateLeft() {
    if (!grounded) {
        rotationalVelocity -= 3.6f;
    }
}

void Rocket::rotateRight() {
    if (!grounded) {
        rotationalVelocity += 3.6f;
    }
}

void Rocket::turnOnEngine() {
    // std::cout << "The engine has been turned on.\n";
    engine_enable = true;
}

void Rocket::turnOffEngine() {
    // std::cout << "The engine has been turned off.\n";
    thrust = 0.0f;
    engine_enable = false;
}

void Rocket::printLog() {
    std::cout << "\n-> Rocket's Stats: \n";
    std::cout << "> Velocity: x: " << velocity.x << ", y: " << velocity.y << " VECTOR LENGHT: " << velocity.length() << '\n';
    std::cout << "> Previous Velocity: x: " << previousVelocity.x << ", y: " << previousVelocity.y << " VECTOR LENGHT: " << previousVelocity.length()  << '\n';
    std::cout << "> Actual Position: x:" << position.x << ", y: " << position.y << '\n';
    std::cout << "> Thrust: " << thrust << '\n';
    std::cout << "> Rotation: " << rotation << '\n';
    std::cout << "> Rotational Velocity: " << rotationalVelocity << '\n';
    std::cout << "> Engine: " << engine_enable << '\n';
    std::cout << "> Grounded: " << grounded << '\n';
    std::cout << "> Is Stable: " << is_stable << '\n';
    std::cout << "> On Island: " << on_island << '\n';
}

Rocket::~Rocket() {
    SDL_DestroyTexture(rocketTexture);
}