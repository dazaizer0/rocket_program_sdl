#ifndef YUME_ISLAND
#define YUME_ISLAND

#include "../../config.hpp"
#include "rocket.hpp"

class Island {
public:
	yume::vec2<float> position;
	yume::vec2<float> size;

	SDL_Texture* loadTexture(const char* file, SDL_Renderer* ren);

	Island(yume::vec2<float> position_v, yume::vec2<float> size_v, SDL_Renderer* renderer);

	void update(yume::vec2<float>* rocket_position, yume::vec2<float>* rocket_size, yume::vec2<float>* rocket_velocity, bool* rocket_grounded, bool* rocket_on_island);
	void render(SDL_Renderer* renderer);

	~Island();

private:
	SDL_Texture* islandTexture{};
};

#endif