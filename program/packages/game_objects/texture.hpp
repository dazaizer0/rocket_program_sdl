#ifndef YUME_TEXTURE
#define YUME_TEXTURE

#include "../../config.hpp"

class Texture {
public:
	yume::vec2<float> position;
	yume::vec2<float> size;
	float rotation;

	SDL_Texture* loadTexture(const char* file, SDL_Renderer* ren);

	Texture(yume::vec2<float> position_v, yume::vec2<float> size_v, const char* file_name, SDL_Renderer* renderer);

	void update(std::vector<std::string> file_names, float duration, float deltaTime, SDL_Renderer* renderer);
	void render(SDL_Renderer* renderer);

	~Texture();

private:
	SDL_Texture* texture{};
	float timer{ 0.0f };
	int actualAnimIndex{ 0 };
};

#endif