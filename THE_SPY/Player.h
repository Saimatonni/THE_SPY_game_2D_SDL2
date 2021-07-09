#ifndef Player_H
#define Player_H
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include <cmath>
#include "bullet.h"
#include "collision.h"
#include "Physics.h"
#include "score.h"
class Player : public Mother
{
	SDL_Texture* image;
	int xvel,yvel;
	SDL_Rect clips[22];
	bool ground;
	bool jump;
	int health;
	bool moving;
	char direction;
	bool isFiring;

public:
	int lives = 3;
	Hud hud;
	double frame;
	Player(SDL_Texture*);
	~Player();
	void Show(SDL_Renderer*);
	void move(vector<vector<int> >&);
	void Jump();
	int score = 0;
	SDL_Rect* GetRect();
	int getHealth();
	void setHealth(int);
	void setDirection(const char dir);
	void setMoving(bool);
	char getDirection();
	void SetFiring(bool b) { isFiring = b; };
	Physics physics;
	bool hasgun;
	bool nextLevel;
	SDL_Renderer* renderer;
};
#endif
