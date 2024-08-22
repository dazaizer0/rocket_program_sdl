#include "island.hpp"

SDL_Texture* Island::loadTexture(const char* file, SDL_Renderer* ren) {
    SDL_Surface* surface = IMG_Load(file);
    if (surface == nullptr) {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);
    return texture;
}

Island::Island(yume::vec2<float> position_v, yume::vec2<float> size_v, SDL_Renderer* renderer)
    : position(position_v), size(size_v) {
    islandTexture = loadTexture("island.png", renderer);
}

void Island::update(yume::vec2<float>* rocket_position, yume::vec2<float>* rocket_size, yume::vec2<float>* rocket_velocity, bool* rocket_grounded, bool* rocket_on_island, std::function<void()> lvlOut) {
    bool collides_from_top = rocket_position->y + rocket_size->y > position.y
        && rocket_position->y < position.y
        && rocket_position->x < position.x + size.x
        && rocket_position->x + rocket_size->x > position.x;

    if (collides_from_top && rocket_velocity->y > 0) {
        //std::cout << "Collision from top detected" << std::endl;
        rocket_position->y = position.y - rocket_size->y;
        rocket_velocity->y = 0;
        rocket_velocity->x = 0;

        *rocket_grounded = true;
        *rocket_on_island = true;

        lvlOut();

        return;
    }

    bool collides_from_bottom = rocket_position->y < position.y + size.y
        && rocket_position->y + rocket_size->y > position.y + size.y
        && rocket_position->x < position.x + size.x
        && rocket_position->x + rocket_size->x > position.x;

    if (collides_from_bottom && rocket_velocity->y < 0) {
        //std::cout << "Collision from bottom detected" << std::endl;
        rocket_position->y = position.y + size.y;
        rocket_velocity->y = 0;
        return;
    }

    bool collides_from_left = rocket_position->x + rocket_size->x > position.x
        && rocket_position->x < position.x
        && rocket_position->y < position.y + size.y
        && rocket_position->y + rocket_size->y > position.y;

    if (collides_from_left && rocket_velocity->x > 0) {
        //std::cout << "Collision from left detected" << std::endl;
        rocket_position->x = position.x - rocket_size->x;
        rocket_velocity->x = 0;
        return;
    }

    bool collides_from_right = rocket_position->x < position.x + size.x
        && rocket_position->x + rocket_size->x > position.x + size.x
        && rocket_position->y < position.y + size.y
        && rocket_position->y + rocket_size->y > position.y;

    if (collides_from_right && rocket_velocity->x < 0) {
        //std::cout << "Collision from right detected" << std::endl;
        rocket_position->x = position.x + size.x;
        rocket_velocity->x = 0;
    }
}

void Island::render(SDL_Renderer* renderer) {
    SDL_Rect islandRect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
    SDL_RenderCopyEx(renderer, islandTexture, nullptr, &islandRect, 0, nullptr, SDL_FLIP_NONE);
}

Island::~Island() {
    // delete islandTexture;
}