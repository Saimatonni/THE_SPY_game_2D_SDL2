#include <SDL2/SDL.h>
#include "collision.h"
using namespace std;

#ifndef COLLECTABLES_H
#define COLLECTABLES_H

class Collectables : Mother
{
public:
	SDL_Rect box;
	SDL_Rect clips[6];
	Collectables() {};
	Collectables(SDL_Texture *tex, int, int);
	void Update();
	void Show(int x, int y, SDL_Renderer*);
	SDL_Texture* tex;
	int xPos, yPos;
	double frame;

private:
	int totalFrames;
	int size;

};

#endif
