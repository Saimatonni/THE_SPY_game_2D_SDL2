#include "Enemy.h"

Enemy::Enemy(SDL_Texture* img, int x, int y, int xVel, int yVel, int num)
{
	number = num;
	image = img;
	box.x = x;
	box.y = y;
	box.w = TILE_SIZE;
	box.h = TILE_SIZE;
	xvel = xVel;
	yvel = yVel;
	ground = 0;
	for (int i = 0; i < 16; i++)
	{
		clips[i].x = i * TILE_SIZE;
		clips[i].y = 0;
		clips[i].w = TILE_SIZE;
		clips[i].h = TILE_SIZE;
	}
	frame = 0.0;

}

Enemy::~Enemy()
{

}

SDL_Rect* Enemy::GetRect()
{
	return &box;
}

void Enemy::move(std::vector<std::vector<int> >& map)
{
	int start=(Mother::coord.x-(Mother::coord.x%TILE_SIZE))/TILE_SIZE-2;
	int end=(Mother::coord.x+Mother::coord.w+(TILE_SIZE-(Mother::coord.x+Mother::coord.w)%TILE_SIZE))/TILE_SIZE+2;
	bool nc=0;
	if(end>map[0].size())
		end=map[0].size();
	if(start<0)
		start=0;


	for (int i = 0; i < map.size(); i++) {
		for (int j = start; j < end; j++)
		{
			if (map[i][j] == 0)
				continue;

			SDL_Rect destrect = { j * TILE_SIZE,i * TILE_SIZE,TILE_SIZE,TILE_SIZE };
			if (collision(&destrect, &box))
			{
				//			std::cout << box.x << std::endl;
				if (destrect.y >= box.y + box.h - 11)
				{
					ground = 1;
					yvel = 0;
					nc = 1;
				}
				if (box.x + box.w >= destrect.x - 5 && box.y + box.h >= destrect.y + 6 && box.x + box.w <= destrect.x + 20) //collision - right to the Player
				{
					xvel = -xvel;
					if (xvel < 0) frame = 8;
					else frame = 0;
				}
				else if (box.x <= destrect.x + destrect.w && box.y + box.h >= destrect.y + 6) //collision - left to the Player
				{
					xvel = -xvel;
					if (xvel < 0) frame = 8;
					else frame = 0;
				}
			}
		}
	}
	if(!nc)
		yvel=5;
	box.x+=xvel;
	box.y+=yvel;
	frame += .25;

	if (xvel <= 0 && frame > 15)
	{
		frame = 8;
	}

	else if (xvel > 0 && frame > 7)
	{
		frame = 0;
	}
}


void Enemy::Show(SDL_Renderer* renderer)
{
	SDL_Rect tmp = { box.x - Mother::coord.x,box.y,TILE_SIZE,TILE_SIZE };
	SDL_RenderCopy(renderer, image, &clips[(int)(frame)], &tmp);
}

double Enemy::getXvel()
{
	return xvel;
}

void Enemy::setXvel(double vel)
{
	xvel = vel;
}
