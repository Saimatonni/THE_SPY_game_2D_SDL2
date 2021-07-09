#include "MazeKey.h"

SDL_Texture * MazeKey::keyTexture;

MazeKey::MazeKey(std::shared_ptr<Room> setRoom) :
	MazeObject::MazeObject(setRoom)
{
	if(keyTexture == NULL)
	{
		keyTexture = IMG_LoadTexture(objRenderer, IMG_MAZEKEY);
		if (keyTexture == NULL)
		{
			printf("SDL could not load the Texture from ");
			printf(IMG_MAZEKEY);
			return;
		}
	}
	curObjTexture = keyTexture;
}

MazeKey::~MazeKey()
{
}
