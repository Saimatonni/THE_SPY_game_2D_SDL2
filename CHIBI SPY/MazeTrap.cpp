#include "MazeTrap.h"

SDL_Texture * MazeTrap::trapActiveTexture;
SDL_Texture * MazeTrap::trapInactiveTexture;

MazeTrap::MazeTrap(std::shared_ptr<Room> room) :
	MazeObject(room), maxTrapTime(rand() % 4 + 3), curTrapTime(rand() % maxTrapTime)
{
	if (trapActiveTexture == NULL)
	{
		trapActiveTexture = IMG_LoadTexture(objRenderer, IMG_TRAP_ACTIVE);
		if (trapActiveTexture == NULL)
		{
			printf("SDL could not load the Texture from ");
			printf(IMG_TRAP_ACTIVE);
			return;
		}
	}
	if (trapInactiveTexture == NULL)
	{
		trapInactiveTexture = IMG_LoadTexture(objRenderer, IMG_TRAP_INACTIVE);
		if (trapInactiveTexture == NULL)
		{
			printf("SDL could not load the Texture from ");
			printf(IMG_TRAP_INACTIVE);
			return;
		}
	}
	curObjTexture = trapInactiveTexture;
	SetTrapTexture();
}

MazeTrap::~MazeTrap()
{
}
