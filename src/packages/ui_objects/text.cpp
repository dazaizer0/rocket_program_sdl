#include "text.hpp"
#include <string>

Text::Text(yume::vec2<int> position_v, int font_size, SDL_Color color, std::string text_v, SDL_Renderer* renderer)
	: position(position_v), font(TTF_OpenFont("res/fonts/IBMPlexSans-Medium.ttf", font_size)), textColor(color), text(text_v) {
	textSurface = TTF_RenderText_Solid(font, text.c_str(), textColor);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
}

void Text::render(SDL_Renderer* renderer) {
	int textWidth, textHeight;
	SDL_QueryTexture(textTexture, NULL, NULL, &textWidth, &textHeight);

	SDL_Rect renderQuad = { position.x, position.y, textWidth, textHeight };

	SDL_RenderCopy(renderer, textTexture, NULL, &renderQuad);
}

void Text::updateText(std::string new_text, SDL_Color new_color, SDL_Renderer* renderer) {
	text = new_text;

	if (textTexture != nullptr) {
		SDL_DestroyTexture(textTexture);
	}

	textSurface = TTF_RenderText_Solid(font, text.c_str(), new_color);
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_FreeSurface(textSurface);
}

Text::~Text() {
	if (textTexture != nullptr) {
		SDL_DestroyTexture(textTexture);
	}
	if (font != nullptr) {
		TTF_CloseFont(font);
	}
}
