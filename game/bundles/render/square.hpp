#pragma once

#include "../../config.hpp"

namespace render {
    class square {
    public:
        mathy::vec2<float> position = mathy::vec2<float>::ZERO();
        mathy::vec2<float> size = mathy::vec2<float>::ZERO();

        square(mathy::vec2<float> position_value, mathy::colorRGBA color, mathy::vec2<float> size, SDL_Renderer* renderer);

        void render_square();

        ~square() = default;

    private:
        Uint8 r;
        Uint8 g;
        Uint8 b;
        Uint8 a;

        SDL_Renderer* renderer;
    };

    square::square(mathy::vec2<float> position_value, mathy::colorRGBA color, mathy::vec2<float> size_value, SDL_Renderer* renderer) {
        this->renderer = renderer;
        position = position_value;
        size = size_value;

        position.x -= size.x / 2;
        position.y -= size.y / 2;

        r = color.r;
        g = color.g;
        b = color.b;
        a = color.a;
    }

    void square::render_square() {
        if (renderer == nullptr) {
            std::cerr << "renderer has not been initialized\n";
            return;
        }

        SDL_SetRenderDrawColor(renderer, r, g, b, a);

        SDL_Rect squareRect = { (int)position.x, (int)position.y, (int)size.x, (int)size.y };

        SDL_RenderFillRect(renderer, &squareRect);
    }
}
