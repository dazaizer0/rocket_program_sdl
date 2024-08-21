#include "earth.hpp"

SDL_Texture* Earth::loadTexture(const char* file, SDL_Renderer* ren) {
    SDL_Surface* surface = IMG_Load(file);
    if (surface == nullptr) {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);
    return texture;
}

Earth::Earth(yume::vec2<float> position_v, yume::vec2<float> size_v, SDL_Renderer* renderer)
    : position(position_v), size(yume::vec2<float>{ size_v.x, size_v.x }), rotation(90), radius(size.x / 2) {
	earthTexture = loadTexture("earth.png", renderer);
}

void Earth::update(float deltaTime) {

}

void Earth::render(SDL_Renderer* renderer) {
	SDL_Rect rocketRect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
	SDL_RenderCopyEx(renderer, earthTexture, nullptr, &rocketRect, rotation - 90, nullptr, SDL_FLIP_NONE);
}

Earth::~Earth() {
    // delete earthTexture;
} 