#pragma once

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#if (WIN32)
	#include <SDL_image.h>
	#include <SDL_mixer.h>
#else
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_mixer.h>
#endif

#include <iostream>
#include <cmath>
#include <vector>
#include <string>

#include "source/math/mathy.hpp"
#include "source/render/texture.hpp"
