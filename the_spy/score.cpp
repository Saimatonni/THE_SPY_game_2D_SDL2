#include "score.h"
#include <iostream>
#include <cstring>
using namespace std;

void Hud::RenderText(const char* string, SDL_Rect *src, SDL_Rect *dest, SDL_Renderer* renderer)
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, string, color);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (tex == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}

		//Get rid of old surface
		SDL_FreeSurface(textSurface);
		SDL_Rect rect = { 20, 20, strlen(string) * 20, 30 };
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
		SDL_RenderFillRect(renderer, &rect);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderCopy(renderer, tex, NULL, &rect);
	}
}

void Hud::RenderText(const char* string, SDL_Rect *src, SDL_Rect *dest, SDL_Renderer* renderer, bool x)
{
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, string, color);
	if (textSurface == NULL)
	{
		printf("Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
	}
	else
	{
		//Create texture from surface pixels
		SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, textSurface);
		if (tex == NULL)
		{
			printf("Unable to create texture from rendered text! SDL Error: %s\n", SDL_GetError());
		}

		//cout << "HEllo" << endl;

		//Get rid of old surface
		SDL_Rect rectFindKey = {220, 30, 200, 20};
		SDL_FreeSurface(textSurface);
		//SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		//SDL_RenderFillRect(renderer, &rectFindKey);
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
		SDL_RenderCopy(renderer, tex, NULL, &rectFindKey);
	}
}


void Hud::DrawScore()
{

}

int Hud::GetScore()
{
	return score;
}

Hud::Hud()
{
	font = TTF_OpenFont("font2.ttf", 28);
	score = 0;
	color = { 255, 0, 0 };
	scorePosX = 20;
	scorePosY = 20;
}
