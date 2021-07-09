/*
	The MazeDoor class defines the final point that the player is trying 
	to reach. It is represented by a Lock texture and is placed into the 
	room that is furthest from the starting room in the maze (done when 
	creating the maze in the Maze class). 
*/

#pragma once

#ifndef MAZEDOOR_H
#define MAZEDOOR_H

#include "MazeObject.h"
#define IMG_MAZEDOOR "res/Images/Lock.png"
class MazeDoor :
	public MazeObject
{
private:
	static SDL_Texture * doorTexture;
public:
	MazeDoor(std::shared_ptr<Room> setRoom);
	~MazeDoor();
};
#endif
