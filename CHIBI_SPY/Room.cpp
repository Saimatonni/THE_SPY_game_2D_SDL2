#include "Room.h"

SDL_Texture *  Room::roomTextures[16];

Room::Room(Coordinate pos, SDL_Renderer * renderer) : roomPos(pos)
{
	roomRenderer = renderer;

	if(roomTextures[0] == NULL)
	{
		roomTextures[0b0000] = IMG_LoadTexture(roomRenderer, IMG_0BORDER);		//None
		
		roomTextures[0b0001] = IMG_LoadTexture(roomRenderer, IMG_1BORDER_D);	//Down
		roomTextures[0b0010] = IMG_LoadTexture(roomRenderer, IMG_1BORDER_L);	//Left
		roomTextures[0b0100] = IMG_LoadTexture(roomRenderer, IMG_1BORDER_U);	//Up
		roomTextures[0b1000] = IMG_LoadTexture(roomRenderer, IMG_1BORDER_R);	//Right

		roomTextures[0b0011] = IMG_LoadTexture(roomRenderer, IMG_2BORDER_DL);
		roomTextures[0b0101] = IMG_LoadTexture(roomRenderer, IMG_2BORDER_DU);
		roomTextures[0b1001] = IMG_LoadTexture(roomRenderer, IMG_2BORDER_DR);
		roomTextures[0b0110] = IMG_LoadTexture(roomRenderer, IMG_2BORDER_LU);
		roomTextures[0b1010] = IMG_LoadTexture(roomRenderer, IMG_2BORDER_LR);
		roomTextures[0b1100] = IMG_LoadTexture(roomRenderer, IMG_2BORDER_UR);

		roomTextures[0b0111] = IMG_LoadTexture(roomRenderer, IMG_3BORDER_DLU);
		roomTextures[0b1011] = IMG_LoadTexture(roomRenderer, IMG_3BORDER_DLR);
		roomTextures[0b1101] = IMG_LoadTexture(roomRenderer, IMG_3BORDER_DUR);
		roomTextures[0b1110] = IMG_LoadTexture(roomRenderer, IMG_3BORDER_LUR);
		
		roomTextures[0b1111] = IMG_LoadTexture(roomRenderer, IMG_4BORDER);
	}
	curRoomTexture = roomTextures[0b1111];
}

Room::Room(const Room &other) :
	roomPos(other.roomPos),
	roomRect(other.roomRect), 
	adjRooms(other.adjRooms), 
	availRooms(other.availRooms), 
	connectRooms(other.connectRooms)
{
}

Room::~Room()
{
}



