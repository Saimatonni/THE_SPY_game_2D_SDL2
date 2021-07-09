#include "Player.h"
#include "collision.h"


Player::Player(SDL_Texture* img)
{
	hasgun = false;
	image=img;
	physics.box.x = 30;
	physics.box.y = 20;
	physics.box.w = TILE_SIZE;
	physics.box.h = TILE_SIZE;
	ground=0;

	physics.acceleration.x = physics.acceleration.y = 0;
	physics.velocity.x = physics.velocity.y = 0;
	frame=0.0;
	for(int i=0;i<22;i++)
	{
		clips[i].x=i*TILE_SIZE;
		clips[i].y=0;
		clips[i].w=clips[i].h=TILE_SIZE;
	}
	jump=0;
	health=1;
	direction='r';
	moving=false;
	nextLevel = false;
}

Player::~Player()
{
}


void Player::setMoving(bool b)
{
	moving=b;
}

char Player::getDirection()
{
	return direction;
}

void Player::Show(SDL_Renderer *renderer)
{
	this->renderer = renderer;
	SDL_RenderCopy(renderer, image, &clips[(int)(frame)], &physics.box);
}

void Player::move(vector<vector<int> >& map)
{
	int start=(Mother::coord.x-(Mother::coord.x%TILE_SIZE))/TILE_SIZE;
	int end=(Mother::coord.x+Mother::coord.w+(TILE_SIZE-(Mother::coord.x+Mother::coord.w)%TILE_SIZE))/TILE_SIZE;
	bool nc=0;
	if (end > map[0].size())
		end = map[0].size();
	if(start < 0)
		start=0;

	for (int i = 0; i < map.size(); i++)
	{
		for (int j = start; j < end; j++)
		{
			if (map[i][j] == 0)
				continue;

			SDL_Rect destrect = { j * TILE_SIZE - Mother::coord.x,i * TILE_SIZE,TILE_SIZE,TILE_SIZE };
			SDL_Rect srcRect = physics.box;
			srcRect.x += 10; srcRect.w -= 15;
			//SDL_RenderDrawRect(renderer, &srcRect);

			if (collision(&destrect, &srcRect))
			{

				if (map[i][j] == -4)
				{
					if (hasgun)
					{
						nextLevel = true;
						hasgun = false;
						SDL_Delay(1000);
						cout << "Go To Next Level" << endl;
					}

					else
					{
						hud.RenderText("Find A Gun", NULL, NULL, renderer, true);
						//SDL_RenderPresent(renderer);

						//cout << "Find a key" << endl;
					}

				}
				else if (map[i][j] == -3)
				{
					hasgun = 1;
					score += 30;
					map[i][j] = 0;
				}
				else if (map[i][j] == -2)
				{
					hud.score += 10;

					map[i][j] = 0;
				}
				//nc = 1;
				else if (destrect.y >= physics.box.y + physics.box.h - 10)
				{
					//std::cout << "collision type 1" << endl;
					nc = 1;
					ground = 1;
					physics.velocity.y = 0;
					physics.acceleration.y = 0;
					//physics.box.y = destrect.y - physics.box.h;
				}
				else if (destrect.y + destrect.h <= physics.box.y + 15)
				{
					//std::cout << "collision type 2" << endl;
					nc = 1;
					physics.box.y++;
					physics.acceleration.y = 1;
				}
				else if (physics.box.x + physics.box.w >= destrect.x - 5 && physics.box.y + physics.box.h >= destrect.y + 10 && physics.box.x + physics.box.w <= destrect.x + 20)
				{
					//cout << physics.box.x + physics.box.w << "  " << destrect.x << endl;
					//std::cout << "collision type 3" << endl;
					nc = 1;
					physics.velocity.x = 0;
					physics.box.x--;
					//moving = false;
				}
				else if (physics.box.x <= destrect.x + destrect.w && physics.box.y + physics.box.h >= destrect.y + 10)
				{

					//std::cout << "collision type 4" << endl;
					nc = 1;
					physics.velocity.x = 0;
					physics.box.x++;


					physics.velocity.y = 0;
					//moving = false;

				}

				else
				{
					nc = 1;
					physics.velocity.y = 0;
				}

			}
		}
	}

	if(!nc && !jump)
	{
		physics.acceleration.y = 1;
		ground=0;
	}

	if (jump && !ground){
		physics.acceleration.y = 1;
	}
	else if (jump && ground) {
		jump = false;
	}
	else if(!jump && ground) {
		physics.acceleration.y = 0;
		jump = 0;
	}
	physics.Update();

	
	if (isFiring)
	{
		if (direction == 'r')
			frame = 19;
		else
			frame = 20;
	}
	else if(moving)
	{

		if (direction == 'r')
		{
			frame += .4;
			if (frame >= 9) frame = 0;
		}
		if (direction == 'l')
		{
			frame -= .4;
			if (frame < 9 || frame > 17) frame = 17;
		}
	}

	else
	{
		if (direction == 'r')
			frame = 18;
		else
			frame = 21;
	}

}

void Player::Jump()
{
	if(ground && !jump)
	{
		ground=0;
		jump=1;
		physics.velocity.y = -15;
		physics.box.y-=12;
	}
}

SDL_Rect* Player::GetRect()
{
	return &physics.box;
}

int Player::getHealth()
{
	return health;
}

void Player::setHealth(int heal)
{
	health = heal;
}

void Player::setDirection(const char dir)
{
	if((dir=='r' || dir=='l') && direction!=dir)
	{
		direction=dir;
		if (direction == 'r')
			frame = 0;
		else
			frame = 9;
	}
}
