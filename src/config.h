#pragma once

// SDL
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#if (WIN32)
	#include <SDL_image.h>
    #include <SDL_mixer.h>
	// #include <SDL_mixer.h>
#else
	#include <SDL2/SDL_image.h>
    #include <SDL2/SDL_mixer.h>

#endif // (WIN32)

// BASIC
#include <iostream>
#include <cmath>

// SOURCE
#include "source/math/mathy.hpp"