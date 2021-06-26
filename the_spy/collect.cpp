#include "Collect.h"

Collectables::Collectables(SDL_Texture* text, int n, int w)
{
	tex = text;

	for (int i = 0; i < n; i++)
	{
		clips[i].x = i * w;
		clips[i].y = 0;
		clips[i].w = w;
		clips[i].h = w;
		
	}

	size = w;

	totalFrames = n;
}

void Collectables::Update()
{
	frame += .3334;
	if (frame > totalFrames - 1) frame = 0;
}

void Collectables::Show(int x, int y, SDL_Renderer* renderer)
{
	SDL_Rect tmp = { x, y, size, size};
	SDL_RenderCopy(renderer, tex, &clips[(int)(frame)], &tmp);
	///frame += .5;
}
