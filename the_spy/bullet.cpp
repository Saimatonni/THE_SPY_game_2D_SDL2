#include "bullet.h"

bullet::bullet(SDL_Texture* img,int x,int y,int xVel,int yVel)
{
	image=img;
	box.x=x;
	box.y=y;
	box.w=10;
	box.h=10;
	xvel=xVel;
	yvel=yVel;
}

bullet::~bullet()
{

}

SDL_Rect* bullet::GetRect()
{
	return &box;
}

void bullet::move()
{
	box.x+=xvel;
	box.y+=yvel;
}

void bullet::Show(SDL_Renderer* renderer)
{
	SDL_RenderCopy(renderer, image, NULL, &box);
}
