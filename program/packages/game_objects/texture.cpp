#include "texture.hpp"

SDL_Texture* Texture::loadTexture(const char* file, SDL_Renderer* ren) {
    SDL_Surface* surface = IMG_Load(file);
    if (surface == nullptr) {
        printf("IMG_Load Error: %s\n", IMG_GetError());
        return nullptr;
    }
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
    SDL_FreeSurface(surface);
    return texture;
}

Texture::Texture(yume::vec2<float> position_v, yume::vec2<float> size_v, const char* file_name, SDL_Renderer* renderer)
    : position(position_v), size(size_v), rotation(90) {
    texture = loadTexture(file_name, renderer);
}

void Texture::update(float deltaTime) {

}

void Texture::render(SDL_Renderer* renderer) {
    SDL_Rect rect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
    SDL_RenderCopyEx(renderer, texture, nullptr, &rect, rotation - 90, nullptr, SDL_FLIP_NONE);
}

Texture::~Texture() {
}