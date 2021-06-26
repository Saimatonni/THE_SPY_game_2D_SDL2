/*
	The MazeKey class defines the key the player needs to get to reach 
	the next level. It is represented by a key texture and is put into 
	a random room other than the first or last. 
*/

#pragma once

#ifndef MAZEKEY_H
#define MAZEKEY_H

#include "MazeObject.h"

#define IMG_MAZEKEY "res/Images/Key.png"
class MazeKey :
	public MazeObject
{
private:
	static SDL_Texture * keyTexture;
public:
	MazeKey(std::shared_ptr<Room> room);
	~MazeKey();
};
#endif
