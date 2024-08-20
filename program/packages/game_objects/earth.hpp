#ifndef YUME_EARTH
#define YUME_EARTH

#include "../../config.hpp"

class Earth {
private:
	SDL_Texture* earthTexture;

	SDL_Texture* loadTexture(const char* file, SDL_Renderer* ren);

public:
	yume::vec2<float> position;
	yume::vec2<float> size;
	float rotation;
	float radius;

	Earth (yume::vec2<float> position_v, yume::vec2<float> size_v, SDL_Renderer* renderer);
	void update(float deltaTime);
	void render(SDL_Renderer* renderer);
	~Earth ();

};

#endif