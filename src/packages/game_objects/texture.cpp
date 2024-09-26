#include "texture.hpp"

Texture::Texture(yume::vec2<float> position_v, yume::vec2<float> size_v, const char* file_name, SDL_Renderer* renderer)
    : position(position_v), size(size_v), rotation(90) {
    texture = renderManager.loadTexture(file_name, renderer); //  IN DEVELOPENT <- C++ TEST XDDD
}

void Texture::update(std::vector<std::string> fileNames, float duration, float deltaTime, SDL_Renderer* renderer) {
    timer += deltaTime;

    if (timer >= duration) {
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
        }

        texture = renderManager.loadTexture(fileNames[actualAnimIndex].c_str(), renderer);

        actualAnimIndex += 1;
        if (actualAnimIndex >= fileNames.size()) {
            actualAnimIndex = 0;
        }

        timer = 0.0f;
    }
}


void Texture::render(SDL_Renderer* renderer) {
    SDL_Rect rect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
    SDL_RenderCopyEx(renderer, texture, nullptr, &rect, rotation - 90, nullptr, SDL_FLIP_NONE);
}

Texture::~Texture() {
    SDL_DestroyTexture(texture);
}