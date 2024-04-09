#pragma once

// SDL
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#if (WIN32)
	#include <SDL_image.h>
	// #include <SDL_mixer.h>
#else
	#include <SDL2/SDL_image.h>
#endif // (WIN32)

// BASIC
#include <iostream>

// SOURCE
#include "source/math/mathy.hpp"