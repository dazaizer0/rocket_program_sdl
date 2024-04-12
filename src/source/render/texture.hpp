#pragma once

#include "../../config.h"
#include "square.hpp"

namespace render {
	class texture {
	public:
        mathy::vec2<float> position = mathy::vec2<float>::ZERO();
        mathy::vec2<float> size = mathy::vec2<float>::ZERO();
        SDL_Texture* sdl_texture;
        double rotation_angle{};

		texture(const char *path, mathy::vec2<float> position_value, mathy::vec2<float> size_value, float angle_value, SDL_Renderer* renderer);

        void render_texture();
        void dynamic_position_draw(mathy::vec2<float> new_position);

		~texture();

	private:
        SDL_Renderer* renderer;
        SDL_Surface* surface;

        // SDL_Rect destinationRect = { (int)position.x, (int)position.y, (int)size, (int)size }; <- final
        SDL_Rect destinationRect{};
        SDL_Point center{}; // center

        std::string texture_path;
	};

	texture::texture(const char *path, mathy::vec2<float> position_value, mathy::vec2<float> size_value, float angle_value, SDL_Renderer *renderer) {
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

        destinationRect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };
        center = { destinationRect.w / 2, destinationRect.h / 2 };
	}

    void texture::render_texture() {
        // RENDER TEXTURE
        if ((int)position.x != destinationRect.x || (int)position.y != destinationRect.y) {
            destinationRect = {(int) position.x, (int) position.y, (int) size.x, (int) size.y};
        }

        SDL_RenderCopyEx(renderer, sdl_texture, nullptr, &destinationRect, rotation_angle, &center, SDL_FLIP_NONE);
    }

    void texture::dynamic_position_draw(mathy::vec2<float> new_position) {
        // RENDER TEXTURE
        SDL_Rect newDestinationRect = { (int)new_position.x, (int)new_position.y, (int)size.x, (int)size.y };
        SDL_RenderCopyEx(renderer, sdl_texture, nullptr, &newDestinationRect, rotation_angle, &center, SDL_FLIP_NONE);
    }

	texture::~texture() {
        SDL_DestroyTexture(sdl_texture);
        std::cerr << "texture destroyed successfully: " << texture_path << std::endl;
	}
}
