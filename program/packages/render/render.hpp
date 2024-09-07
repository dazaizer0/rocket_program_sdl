#ifndef YUME_REDNER_FUNCS
#define YUME_REDNER_FUNCS

#include "../../config.hpp"

namespace yume {

	class RenderManager {
    public:
		RenderManager() = default;

        SDL_Texture* loadTexture(const char* file, SDL_Renderer* ren) {
            SDL_Surface* surface = IMG_Load(file);
            if (surface == nullptr) {
                printf("IMG_Load Error: %s\n", IMG_GetError());
                return nullptr;
            }
            SDL_Texture* texture = SDL_CreateTextureFromSurface(ren, surface);
            SDL_FreeSurface(surface);
            return texture;
        }

		~RenderManager() = default;
	};
}

#endif