#include "MazeDoor.h"

SDL_Texture * MazeDoor::doorTexture;

MazeDoor::MazeDoor(std::shared_ptr<Room> setRoom) :
	MazeObject::MazeObject(setRoom)
{
	if (doorTexture == NULL)
	{
		doorTexture = IMG_LoadTexture(objRenderer, IMG_MAZEDOOR);
		if (doorTexture == NULL)
		{
			printf("SDL could not load the Texture from ");
			printf(IMG_MAZEDOOR);
			return;
		}
	}
	curObjTexture = doorTexture;
}

MazeDoor::~MazeDoor()
{
}
