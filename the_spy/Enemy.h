#include <SDL2/SDL.h>
#include <vector>
#include <iostream>
#include "collision.h"
#ifndef Enemy_H
#define Enemy_H

class Enemy : public Mother{
	double xvel,yvel;
	SDL_Texture* image;
	int attack;
	bool ground;
	SDL_Rect clips[16];
	double frame;
public:
	SDL_Rect box;
	int number;
	Enemy(SDL_Texture* img, int x, int y, int xvel, int yvel, int num);
	~Enemy();
	SDL_Rect* GetRect();
	void move(std::vector<std::vector<int> >&);
	void Show(SDL_Renderer*);
	double getXvel();
	void setXvel(double);
};

#endif
