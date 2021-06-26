#include "Player2.h"

SDL_Texture * Player::playerTextures[];

Player::Player(std::shared_ptr<Room> setRoom, SDL_Renderer * renderer) :
	MazeObject::MazeObject(setRoom), startRoom(setRoom)
{
	if (playerTextures[0] == NULL)
	{
		playerTextures[0] = IMG_LoadTexture(objRenderer, IMG_PLAYER_LEFT);
		playerTextures[1] = IMG_LoadTexture(objRenderer, IMG_PLAYER_DOWN);
		playerTextures[2] = IMG_LoadTexture(objRenderer, IMG_PLAYER_UP);
		playerTextures[3] = IMG_LoadTexture(objRenderer, IMG_PLAYER_RIGHT);
	}
	curObjTexture = playerTextures[0];
}
Player::Player()
{
}

Player::~Player()
{
}
