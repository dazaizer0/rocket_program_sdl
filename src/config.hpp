#ifndef YUME_CONFIG
#define YUME_CONFIG

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#if (WIN32)
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#else
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#endif

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <cmath>
#include <functional>
#include <random>

#include "packages/math/math.hpp"
#include "packages/render/render.hpp"
#include "packages/game_objects/rocket.hpp"
#include "packages/game_objects/island.hpp"
#include "packages/game_objects/texture.hpp"
#include "packages/ui_objects/text.hpp"

#endif