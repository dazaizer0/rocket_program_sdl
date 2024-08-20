#ifndef YUME_TEXT
#define YUME_TEXT

#include "../../config.hpp"

class Text {
private:
	TTF_Font* font;
	SDL_Color textColor;
	SDL_Texture* textTexture;
public:
	SDL_Surface* textSurface;
	yume::vec2<int> position;
	std::string text;

	Text(yume::vec2<int> position_v, int font_size, SDL_Color color, std::string text_v, SDL_Renderer* renderer);
	void render(SDL_Renderer* renderer);
	void updateText(std::string new_text, SDL_Renderer* renderer);
	~Text();
};

#endif