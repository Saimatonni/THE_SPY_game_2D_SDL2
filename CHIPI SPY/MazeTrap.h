/*
	The MazeTrap class describes an object the player may encounter. Its
	activity is based off of the TrapTime variables. It begins with a 
	random maxTrapTime between 3 and 6, and random curTrapTime between 0
	and the maxTrapTime. Every cycle, the curTrapTime is incremented. When 
	the curTrapTime equals 0, it will be active, and will kill the player 
	if they are in the same position.
*/

#pragma once

#ifndef MAZETRAP_H
#define MAZETRAP_H

#include "MazeObject.h"

#define IMG_TRAP_INACTIVE "res/Images/SpikeTrapInactive.png"
#define IMG_TRAP_ACTIVE "res/Images/SpikeTrapActive.png"

class MazeTrap :
	public MazeObject
{
private:
	int maxTrapTime;
	int curTrapTime;
	static SDL_Texture * trapActiveTexture;
	static SDL_Texture * trapInactiveTexture;
public:
	
	MazeTrap(std::shared_ptr<Room> room);
	~MazeTrap();

	//Increments trap time and checks if trap state must change each cycle
	void NextCycle()
	{
		curTrapTime++;
		if (curTrapTime > maxTrapTime)
		{ 
			curTrapTime = 0;
		}
		SetTrapTexture();
	}
	
	//Sets trap texture based off of current trap state
	void SetTrapTexture()
	{
		if(curTrapTime == 0)
		{
			curObjRoom->roomTypes.push_back(Trap);
			curObjTexture = trapActiveTexture;
		}
		else 
		{
			auto roomTypeIter = std::find(begin((curObjRoom)->roomTypes), end((curObjRoom)->roomTypes), Trap);
			if (roomTypeIter != end(curObjRoom->roomTypes))
			{
				curObjRoom->roomTypes.erase(roomTypeIter);
				curObjTexture = trapInactiveTexture;
			}
		}
	}
};
#endif
