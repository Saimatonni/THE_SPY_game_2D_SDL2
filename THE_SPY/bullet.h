#include <SDL2/SDL.h>
#include <iostream>
#include <vector>

#ifndef BULLET_H
#define BULLET_H

class bullet{
	SDL_Rect box;
	int xvel,yvel;
	SDL_Texture* image;
	public:
	bullet(SDL_Texture* img,int x,int y,int xVel,int yVel);
	~bullet();
	SDL_Rect* GetRect();
	void move();
	void Show(SDL_Renderer*);
};

#endif
