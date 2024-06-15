#pragma once

#include "../config.h"

enum State {
    empty,
    selected,
    confirmed
};

class Skarabeusz {
public:
    mathy::vec2<int> position;
    mathy::vec2<int> size;
    float rotation;

    bool enabled{ true };
    bool can_select{ false };
    bool was_confirmed{ false };

    render::texture gold;
    render::texture blue;
    render::texture brown;

    State state = empty;

    std::vector<int> neighbours_indexes;

    Skarabeusz(SDL_Renderer* renderer, mathy::vec2<int> posv, mathy::vec2<int> sizev, float rotv);
    void update();
    void render();
    ~Skarabeusz();

private:

};

Skarabeusz::Skarabeusz(SDL_Renderer* renderer, mathy::vec2<int> posv, mathy::vec2<int> sizev, float rotv)
    : position(posv), size(sizev), rotation(rotv),
    gold("res/gold.png", posv, sizev, rotv, renderer),
    blue("res/blue.png", posv, sizev, rotv, renderer),
    brown("res/brown.png", posv, sizev, rotv, renderer)
{
}

void Skarabeusz::render() {
    if (enabled) {
        if (state == empty)
            brown.render_texture();
        else if (state == selected)
            blue.render_texture();
        else if (state == confirmed)
            gold.render_texture();
    }
}

Skarabeusz::~Skarabeusz() {
}
