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
    texture = position.vloadTexture(file_name, renderer); //  IN DEVELOPENT <- C++ TEST XDDD
}

void Texture::update(std::vector<std::string> fileNames, float duration, float deltaTime, SDL_Renderer* renderer) {
    timer += deltaTime;

    if (timer >= duration) {
        // Free the current texture before loading a new one
        if (texture != nullptr) {
            SDL_DestroyTexture(texture);
        }

        // Load the next texture in the sequence
        texture = loadTexture(fileNames[actualAnimIndex].c_str(), renderer);

        // Update the index and wrap around if necessary
        actualAnimIndex += 1;
        if (actualAnimIndex >= fileNames.size()) {
            actualAnimIndex = 0;
        }

        // Reset the timer
        timer = 0.0f;
    }
}


void Texture::render(SDL_Renderer* renderer) {
    SDL_Rect rect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
    SDL_RenderCopyEx(renderer, texture, nullptr, &rect, rotation - 90, nullptr, SDL_FLIP_NONE);
}

Texture::~Texture() {
}