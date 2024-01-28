#pragma once

#include "../../config.h"

#include "square.h"

namespace render
{
	class texture
	{
	public:
        mathy::vec3<float> position = mathy::vec3<float>::ZERO();
        SDL_Texture* sdl_texture;
        double angle {};
        int size {};

		texture(
                mathy::vec3<float> position_value,
                float size_value,
                float angle_value,
                const char *path,
                SDL_Renderer* renderer
        );

        void draw();

		~texture();

	private:
        SDL_Renderer* renderer;
        SDL_Surface* surface;

        // SDL_Rect destinationRect = { (int)position.x, (int)position.y, (int)size, (int)size }; <- final
        SDL_Rect destinationRect;
        SDL_Point center; // center

        std::string texture_path;
	};

	texture::texture(
            mathy::vec3<float> position_value,
            float size_value,
            float angle_value,
            const char *path,
            SDL_Renderer *renderer) {

        this->renderer = renderer;

        position = position_value;
        size = size_value;
        angle = angle_value;

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

        destinationRect = { (int)position.x, (int)position.y, (int)size, (int)size };
        center = { destinationRect.w / 2, destinationRect.h / 2 };
	}

    void texture::draw() {
        // RENDER TEXTURE
        if ((int)position.x != destinationRect.x || (int)position.y != destinationRect.y) {
            destinationRect = {(int) position.x, (int) position.y, (int) size, (int) size};
        }

        SDL_RenderCopyEx(renderer, sdl_texture, NULL, &destinationRect, angle, &center, SDL_FLIP_NONE);
    }

	texture::~texture() {
        SDL_DestroyTexture(sdl_texture);
        std::cerr << "texture destroyed successfully: " << texture_path << std::endl;
	}
};
