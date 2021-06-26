#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
using namespace std;

#ifndef HUD_H
#define HUD_H

class Hud
{
public:
	int scorePosX, scorePosY;
	TTF_Font *font;
	SDL_Color color;
	int score;
	
	void RenderText(const char* string, SDL_Rect *src, SDL_Rect *dest, SDL_Renderer* renderer, bool x);
	void RenderText(const char* string, SDL_Rect* src, SDL_Rect* dest, SDL_Renderer* renderer);
	void DrawScore();
	int GetScore();
	Hud();
};

#endif
