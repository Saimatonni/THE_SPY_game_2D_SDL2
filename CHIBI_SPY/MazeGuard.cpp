#include "MazeGuard.h"

SDL_Texture * MazeGuard::guardTexture;

MazeGuard::MazeGuard(std::shared_ptr<Room> setRoom) :
	guardCenterRoomPtr(setRoom),
	guardIter(begin(setRoom->connectRooms)),
	guardCurRoomPtr(*(begin(setRoom->connectRooms))),
	MazeObject(*(begin(setRoom->connectRooms)))
{
	if (guardTexture == NULL)
	{
		guardTexture = IMG_LoadTexture(objRenderer, IMG_GUARD);
		if (guardTexture == NULL)
		{
			printf("SDL could not load the Texture from ");
			printf(IMG_GUARD);
			return;
		}
	}
	curObjTexture = guardTexture;
}

MazeGuard::~MazeGuard()
{
}
