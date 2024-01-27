#pragma once

#include "../../config.h"

namespace render {

    class square {
    public:
        mathy::vec3<float> position = mathy::vec3<float>::ZERO();

        square(mathy::vec3<float> position_value, mathy::colorRGBA color, int size, SDL_Renderer* renderer);

        void draw();

        ~square();

    private:
        Uint8 r;
        Uint8 g;
        Uint8 b;
        Uint8 a;

        int size;

        SDL_Renderer* renderer;
    };

    square::square(mathy::vec3<float> position_value, mathy::colorRGBA color, int size_value, SDL_Renderer* renderer) {
        this->renderer = renderer;
        position = position_value;
        size = size_value;

        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    }

    void square::draw() {
        if (renderer == nullptr) {
            std::cerr << "renderer has not been initialized\n";
            return;
        }

        SDL_SetRenderDrawColor(
                renderer,
                r,
                g,
                b,
                a
        );

        SDL_Rect squareRect = {
                (int)position.x,
                (int)position.y,
                size,
                size
        };

        SDL_RenderFillRect(renderer, &squareRect);
    }

    square::~square() {
    }

}; // namespace render
