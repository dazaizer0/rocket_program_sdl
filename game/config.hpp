#ifndef YUMESDL_CONFIG_HPP
#define YUMESDL_CONFIG_HPP

#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#if (WIN32)
	#include <SDL_image.h>
	#include <SDL_mixer.h>
	#include <Windows.h>
#else
	#include <SDL2/SDL_image.h>
	#include <SDL2/SDL_mixer.h>
#endif

#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <string>

#include "bundles/math/mathy.hpp"
#include "bundles/render/texture.hpp"
#include "bundles/elementary/scene.hpp"

#endif
