#include <iostream>
#include <SDL2/SDL.h>

using namespace std;

#ifndef PHYSICS_H
#define PHYSICS_H

struct Vector2D
{
	int x, y;
};
class Physics
{
public:
	Vector2D velocity, acceleration, position;
	Physics();
	~Physics();
	SDL_Rect box;
	void IncreaseVelocity(int x, int y);
	void IncreaseAcceleration(int x, int y);
	void MovePosition(int x, int y);
	void Update();

};

#endif
