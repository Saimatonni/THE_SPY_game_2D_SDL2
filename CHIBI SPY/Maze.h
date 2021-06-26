/*
	The Maze class creates the maze. It first creates all of the rooms
	using the mazeXCount and mazeYCount values listed. Then, it creates 
	the maze by using a backtracking algorithm in the CarveMaze function.
	It chooses a random Room as the start point and checks to see what 
	the longest path is while the maze is built. It then places the final
	point at the Room with the longest path to the start. 

	This class also creates and places all of the obstacles and objects
	that are encountered in the maze other than the Player itself.
*/

#pragma once

#ifndef MAZE_H
#define MAZE_H

#include <random>

#include "Room.h"	
#include "MazeTrap.h"
#include "MazeKey.h"
#include "MazeDoor.h"
#include "MazeGuard.h"

//When To Start Obstacles
#define TRAPS_START 2
#define GUARDS_START 4

//Random Element Function
template <typename T, typename A>
T randomElement(std::vector<T, A> &vec)
{
	std::random_device random_device;
	std::mt19937 engine{ random_device() };
	std::uniform_int_distribution<int> dist(0, vec.size() - 1);
	T element = vec[dist(engine)];
	return element;
}

class Maze
{

private:
	//X and Y counts of the maze (total rooms = X*Y)
	int mazeX_RoomCount;
	int mazeY_RoomCount;
	
	//Information about rendering size for the maze
	int mazeX_Offset;
	int mazeY_Offset;
	int mazeX_Size;
	int mazeY_Size;

	//Determines how far apart traps are spaced
	int obstacleSpacing = 3;	

	//All rooms in the Maze
	std::vector<std::shared_ptr<Room>> allRooms;	
	
	//All rooms with objects in them 
	std::vector <std::shared_ptr<Room>> obstacleRooms;
	
	//All objects in this maze
	std::vector<std::shared_ptr<MazeObject>> objectsInMaze;

	//Rect defining maze area
	SDL_Rect mazeRect;	

	//Renderer for the maze
	SDL_Renderer * mazeRenderer = NULL;

public:	
	int mazeLevel;	//Current Game Level

	//Points to the Key Object in this maze
	std::shared_ptr<MazeKey> mazeKeyPtr;
	//Points to the Door Object in this maze
	std::shared_ptr<MazeDoor> mazeDoorPtr;	
	
	//Start and End Position of Maze
	Coordinate startPos;
	Coordinate finalPos;

	//Constructors and Destructors
	Maze(int xCount, int yCount, int xOffset, int yOffset, int xSize, int ySize, SDL_Renderer * renderer, int level, bool showCreation);
	Maze(const Maze &other);
	Maze();
	~Maze();
	
	//Resets the Maze based off current parameters
	void ResetMaze()
	{
		//Clear all previous vectors
		allRooms.clear();
		obstacleRooms.clear();
		objectsInMaze.clear();
		
		//Create Maze and Items in Maze
		CreateRooms();
		CarveMaze();
		if (mazeLevel > TRAPS_START)
		{
			CreateObjects();
		}
		CreateKey();
	}

	//Sets the maze based off of input parameters
	void SetMaze(int x, int y, int level)
	{
		mazeX_RoomCount = x;
		mazeY_RoomCount = y;
		mazeLevel = level;
		ResetMaze();
	}

	//Sets the maze up for the next level
	void NextLevelMaze()
	{
		mazeLevel++;
		mazeX_RoomCount++;
		mazeY_RoomCount++;
		ResetMaze();
	}

	//Checks what point is in the direction dir relative to the Coordinate pos
	void ChangePosition(Coordinate &pos, directions dir)
	{
		switch (dir)
		{
		case up:
			pos.yPos--;
			break;
		case left:
			pos.xPos--;
			break;
		case down:
			pos.yPos++;
			break;
		case right:
			pos.xPos++;
			break;
		}
	}

	//Creates all the rooms and connects them
	void CreateRooms()
	{
		directions allDir[] = { up, left, down, right };

		//Creates all the rooms
		for (int x = 0; x < mazeX_RoomCount; x++)
		{
			for (int y = 0; y < mazeY_RoomCount; y++)
			{
				std::shared_ptr<Room> newPtr(new Room(Coordinate(x, y), mazeRenderer));
				newPtr->MakeRoomRect(mazeX_Offset, mazeY_Offset, mazeX_RoomCount, mazeY_RoomCount, mazeX_Size, mazeY_Size);
				newPtr->AddRoomToRenderer(10);
				allRooms.push_back(newPtr);
			}
		}

		//Go through all the rooms and make sure each knows what they are adjacent to
		for_each(begin(allRooms), end(allRooms), [&](std::shared_ptr<Room> &curRoomPtr)
		{
			std::for_each(std::begin(allDir), std::end(allDir), [&](directions dir)
			{
				Coordinate testPos = curRoomPtr->roomPos;
				ChangePosition(testPos, dir);
				auto adjRoom = find_if(begin(allRooms), end(allRooms), [&](std::shared_ptr<Room> &checkRoomPtr)
				{
					return checkRoomPtr->roomPos == testPos;
				});
				if (adjRoom != end(allRooms))
				{
					curRoomPtr->adjRooms.push_back((*adjRoom));
					curRoomPtr->availRooms.push_back((*adjRoom));
				}
			});
		});
	}

	//Create a maze using a backtracking algorithm
	void CarveMaze()
	{
		//This vector acts as a stack that contains the current maze path
		std::vector<std::shared_ptr<Room>> currentPath;
		
		//Integer that tells us how long the longest path currently is
		size_t longestPath = 0;

		//Checks how many rooms have been added to the maze
		int inMazeCounter = 0;

		//Points to the current room (the top of the stack) and sets it as the starting room
		std::shared_ptr<Room> curRoomPtr = randomElement(allRooms);
		startPos = curRoomPtr->roomPos;
		curRoomPtr->roomTypes.push_back(Start);

		while (inMazeCounter < mazeX_RoomCount * mazeY_RoomCount)
		{
			//If the current room is not in the maze yet, put it into the maze and remove it from any available rooms vectors on adjacent rooms
			if (!curRoomPtr->inMaze)
			{
				//Put it into the collection of maze pointers
				currentPath.push_back(curRoomPtr);
				//Set it as in the maze
				curRoomPtr->inMaze = true;
				//Increase the maze counter
				inMazeCounter++;
				//Removes the current room from Available Rooms Vector in any room adjacent to the current one.
				for_each(begin(curRoomPtr->adjRooms), end(curRoomPtr->adjRooms), [&](std::shared_ptr<Room>  adjRoomPtr)
				{
					//Find if the current room is in the available rooms of the current adjacent room
					auto roomToRemoveIter = std::find(begin(adjRoomPtr->availRooms), end(adjRoomPtr->availRooms), curRoomPtr);
					//If the room is found, remove it from the available rooms
					if (roomToRemoveIter != end(adjRoomPtr->availRooms))
					{
						adjRoomPtr->availRooms.erase(roomToRemoveIter);
					}
				});

				//Updates the longest path if the current path (a vector of pointers to the rooms in the path) is longer
				if (longestPath < currentPath.size())
				{
					longestPath = currentPath.size();
					finalPos = curRoomPtr->roomPos;
				}

				//If the maze level passes the level at which we want to spawn traps, we start spawning traps
				if(mazeLevel > TRAPS_START)
				{
					//Places rooms that can have obstacles a distance apart based off of trap spacing
					if (currentPath.size() % obstacleSpacing == 0)
					{
						obstacleRooms.push_back(curRoomPtr);
					}
				}
			}

			//Find the next room if there is one available and connect the current and next room together
			if (!curRoomPtr->availRooms.empty())
			{
				//Sets the pointer for the next room by choosing a random element from rooms available to the current one
				std::shared_ptr <Room> nextRoomPtr = randomElement(curRoomPtr->availRooms);	
				
				//Connects the next room to the current one and vice versa
				curRoomPtr->ConnectRoom(nextRoomPtr);
				nextRoomPtr->ConnectRoom(curRoomPtr);

				//Assign each room's respective room texture then add them to the renderer
				curRoomPtr->AssignRoomTextures();
				nextRoomPtr->AssignRoomTextures();

				curRoomPtr->AddRoomToRenderer(10);
				nextRoomPtr->AddRoomToRenderer(10);

				//Removes the next room from the available rooms for the current room 
				curRoomPtr->availRooms.erase(std::find(begin(curRoomPtr->availRooms), end(curRoomPtr->availRooms), nextRoomPtr));	
				
				//Sets the next room pointer as the current room pointer																													
				curRoomPtr = nextRoomPtr;	
			}
			//If there is not a room available adjacent to the current one, go back to the previous room
			else
			{
				currentPath.pop_back();
				curRoomPtr = currentPath.back();
			}
		}

		//Places the exit to the maze
		std::shared_ptr<Room> finalRoom = FindRoomByPos(finalPos);
		mazeDoorPtr = std::shared_ptr<MazeDoor>(new MazeDoor(finalRoom));
		finalRoom->roomTypes.push_back(Final);

	}

	//Creates objects in the maze rooms designated during the maze creation. Only called if difficulty is greater than 0
	void CreateObjects()
	{
		for_each(begin(obstacleRooms), end(obstacleRooms), [&](std::shared_ptr<Room> curRoomPtr) {
			if(curRoomPtr->roomTypes.size() == 0)
			{
				//If difficulty is 2 or higher and the room has 3 connected rooms, create a guard
				if (curRoomPtr->connectRooms.size() == 3 && mazeLevel > GUARDS_START)
				{
					std::shared_ptr<MazeGuard> mazeGuardPtr = std::shared_ptr<MazeGuard>(new MazeGuard(curRoomPtr));
					objectsInMaze.push_back(std::static_pointer_cast<MazeObject>(mazeGuardPtr));
				}
				//Otherwise, if the difficulty is 1 or higher, create a Trap
				else
				{
					std::shared_ptr<MazeTrap> mazeTrapPtr = std::shared_ptr<MazeTrap>(new MazeTrap(curRoomPtr));
					objectsInMaze.push_back(std::static_pointer_cast<MazeObject>(mazeTrapPtr));
				}
			}
		});
	}
	
	//Put Key in a random position in the maze where there is currently no other object.
	void CreateKey()
	{
		std::shared_ptr<Room> curRoomPtr;
		do
		{
			curRoomPtr = randomElement(allRooms);
		} while (curRoomPtr->roomTypes.size() > 0);
		
		mazeKeyPtr = std::shared_ptr<MazeKey>(new MazeKey(curRoomPtr));
		curRoomPtr->roomTypes.push_back(Key);
	}

	//Find a room by the position
	std::shared_ptr<Room> FindRoomByPos(Coordinate pos)
	{
		auto iter = (std::find_if(begin(allRooms), end(allRooms), [&](std::shared_ptr<Room> checkRoom) {
			return checkRoom->roomPos == pos;
		}));
		if (iter != end(allRooms))
		{
			return *iter;
		}
		else
		{
			return nullptr;
		}
	}

	//Adds an outline of the maze area to the renderer
	void MazeOutline()
	{
		SDL_Color color = { 255, 255, 255, 255 };
		SDL_RenderDrawRect(mazeRenderer, &mazeRect);
	}

	//Adds each room and an outline to the renderer
	void AddMazeRoomsToRenderer(int delay)
	{
		for_each(begin(allRooms), end(allRooms), [&](std::shared_ptr<Room> curRoomPtr)
		{
			curRoomPtr->AddRoomToRenderer(delay);
		});
		MazeOutline();
	}
	
	//Renders the objects (traps and guards) that are in the maze
	void AddMazeObstaclesToRenderer()
	{
		if (mazeLevel > TRAPS_START)
		{
			for_each(begin(objectsInMaze), end(objectsInMaze), [&](std::shared_ptr<MazeObject> curObjPtr)
			{
				curObjPtr->AddObjToRenderer();
			});
		}
	}

	//Advances each item one increment in its own cycle of behavior
	void NextMazeCycle()
	{
		for_each(begin(objectsInMaze), end(objectsInMaze), [&](std::shared_ptr<MazeObject> curObjPtr)
		{
			curObjPtr->NextCycle();
		});
	}
};
#endif
