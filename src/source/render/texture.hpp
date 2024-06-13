#pragma once

#include "../../config.h"
#include "square.hpp"

namespace render {
    class texture {
    public:
        mathy::vec2<int> position = mathy::vec2<int>::ZERO();
        mathy::vec2<int> size = mathy::vec2<int>::ZERO();
        SDL_Texture* sdl_texture;
        double rotation_angle{};
        bool enabled{ true };

        texture(const char* path, mathy::vec2<int> position_value, mathy::vec2<int> size_value, float angle_value, SDL_Renderer* renderer);

        void render_texture();
        void dynamic_position_draw(mathy::vec2<int> new_position);

        ~texture();

    private:
        SDL_Renderer* renderer;
        SDL_Surface* surface;

        SDL_Rect destinationRect{};
        SDL_Point center{}; // center

        std::string texture_path;

        void update_destination_rect();
    };

    texture::texture(const char* path, mathy::vec2<int> position_value, mathy::vec2<int> size_value, float angle_value, SDL_Renderer* renderer) {
        this->renderer = renderer;
        position = position_value;
        size = size_value;
        rotation_angle = angle_value;

        texture_path = path;

        surface = IMG_Load(path);

        if (surface == nullptr) {
            std::cerr << "IMG_Load: Failed to load image: " << IMG_GetError() << std::endl;
        }
        else {
            std::cout << "texture loaded successfully: " << texture_path << "\n";
        }

        sdl_texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (sdl_texture == nullptr) { // creating texture on surface
            std::cerr << "SDL_CreateTextureFromSurface: Failed to create texture: " << SDL_GetError() << std::endl;
        }
        else {
            std::cout << "texture created successfully\n";
        }

        update_destination_rect();
    }

    void texture::update_destination_rect() {
        destinationRect = { position.x - size.x / 2, position.y - size.y / 2, size.x, size.y };
        center = { size.x / 2, size.y / 2 };
    }

    void texture::render_texture() {
        if (enabled) {
            SDL_RenderCopyEx(renderer, sdl_texture, nullptr, &destinationRect, rotation_angle, &center, SDL_FLIP_NONE);
        }
    }

    void texture::dynamic_position_draw(mathy::vec2<int> new_position) {
        SDL_Rect newDestinationRect = { new_position.x - size.x / 2, new_position.y - size.y / 2, size.x, size.y };
        SDL_RenderCopyEx(renderer, sdl_texture, nullptr, &newDestinationRect, rotation_angle, &center, SDL_FLIP_NONE);
    }

    texture::~texture() {
        SDL_DestroyTexture(sdl_texture);
        std::cerr << "texture destroyed successfully: " << texture_path << std::endl;
    }
}
