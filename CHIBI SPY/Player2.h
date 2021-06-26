/*
	The Player class creates a player that exists within the maze and 
	can navigate through using the PlayerMove function. This takes
	arrow key inputs from the user to move the player. The texture
	will change depending upon what direction the player should face
	based off the PlayerMove function

	The player will reset to the start of the maze if they encounter 
	any traps or guards and loses a life. If the player loses all of 
	their lives, the Game Over occurs. 
*/

#pragma once

#ifndef PLAYERTWO_H
#define PLAYERTWO_H

#include "MazeObject.h"
#include "Room.h"

#define IMG_PLAYER_RIGHT "res/Images/PlayerRight.png"
#define IMG_PLAYER_LEFT "res/Images/PlayerLeft.png"
#define IMG_PLAYER_UP "res/Images/PlayerUp.png"
#define IMG_PLAYER_DOWN "res/Images/PlayerDown.png"

class Player :
	public MazeObject
{
private:
	static SDL_Texture * playerTextures[4];
	std::shared_ptr<Room> startRoom;
public:
	int playerLives = 10;

	Player(std::shared_ptr<Room> setRoom, SDL_Renderer * renderer);
	Player();
	~Player();
	
	bool hasKey = false;

	//Sets the player back to start
	void SetPlayerToStart(std::shared_ptr<Room> room)
	{
		hasKey = false;
		SetStartRoom(room);
	}

	void ResetPlayer(std::shared_ptr<Room> room, int lives)
	{
		playerLives = lives;
		SetPlayerToStart(room);
	}

	void SetStartRoom(std::shared_ptr<Room> room)
	{
		startRoom = room;
		SetObjRoom(room);
	}

	//Moves the player based off arrow inputs. Returns true if move is successful, false if move is not.
	bool PlayerMove(SDL_Keycode key, SDL_Renderer * renderer)
	{
		bool successfulMove = false;
		Coordinate testPos = objPos;
		
		switch (key)
		{
		case SDLK_UP:
			testPos.yPos--;
			break;
		case SDLK_DOWN:
			testPos.yPos++;
			break;
		case SDLK_LEFT:
			testPos.xPos--;
			break;
		case SDLK_RIGHT:
			testPos.xPos++;
			break;
		default:
			std::cout << "Not a valid input!" << std::endl;
			return successfulMove;
			break;
		}

		//Sets player texture based on key input
		curObjTexture = playerTextures[key % 4];

		//Check if the room the player is trying to move to is one of the connected rooms
		auto iter = std::find_if(begin(curObjRoom->connectRooms), end(curObjRoom->connectRooms), [&testPos](std::shared_ptr<Room> nextRoom) {
			return nextRoom->roomPos == testPos;
		});

		//if it is, move to that room and increase the step count
		if (iter != std::end(curObjRoom->connectRooms))
		{
			SetObjRoom(*iter);
			successfulMove = true;
		}
		else
		{
			std::cout << "Can't Move That Way" << std::endl;
			successfulMove = false;
		}
		return successfulMove;
	}

	//Checks for items in the player's position. Returns false if it is not
	//an obstacle (Trap or Guard)
	bool CheckForObjects()
	{
		//Pick Up Key
		if (std::find(begin(curObjRoom->roomTypes), end(curObjRoom->roomTypes), Key) != end(curObjRoom->roomTypes))
		{
			hasKey = true;
			std::cout << "Picked Up Key" << std::endl;
		}
		//Reset player Position and reduce lives count if it is a trap or guard
		if (std::find(begin(curObjRoom->roomTypes ), end(curObjRoom->roomTypes), Trap) != end(curObjRoom->roomTypes))
		{
			SetObjRoom(startRoom);
			std::cout << "Hit a trap" << std::endl;
			playerLives--;
			return true;
		}
		else if (std::find(begin(curObjRoom->roomTypes), end(curObjRoom->roomTypes), Guard) != end(curObjRoom->roomTypes))
		{
			SetObjRoom(startRoom);
			std::cout << "Hit a guard" << std::endl;
			playerLives--;
			return true;
		}
		return false;
	}
	
	
};
#endif
