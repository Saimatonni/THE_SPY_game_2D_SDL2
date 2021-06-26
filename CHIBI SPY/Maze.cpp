#include "Maze.h"

Maze::Maze(int xCount, int yCount, int xOffset, int yOffset, int xSize, int ySize, SDL_Renderer * renderer, int level, bool showCreation) :
	mazeX_RoomCount(xCount), mazeY_RoomCount(yCount), mazeX_Offset(xOffset), mazeY_Offset(yOffset), mazeX_Size(xSize), mazeY_Size(ySize), mazeRenderer(renderer), mazeLevel(level)
{
	mazeRect.x = xOffset;    
	mazeRect.y = yOffset;
	mazeRect.w = xSize;
	mazeRect.h = ySize;
	CreateRooms();
	CarveMaze();
	if(level > TRAPS_START)
		CreateObjects();
	CreateKey();
}

Maze::Maze(const Maze &other) :
	mazeRect(other.mazeRect),
	mazeX_RoomCount(other.mazeX_RoomCount),
	mazeY_RoomCount(other.mazeY_RoomCount),
	allRooms(other.allRooms),
	objectsInMaze(other.objectsInMaze),
	mazeDoorPtr(other.mazeDoorPtr),
	mazeKeyPtr(other.mazeKeyPtr),
	startPos(other.startPos),
	finalPos(other.finalPos)
{
}

Maze::Maze()
{
}
/*
Maze& Maze::operator=(const Maze &other)
{
	allRooms = other.allRooms;
	allTraps = other.allTraps;
	mazeX_RoomCount = other.mazeX_RoomCount;
	mazeY_RoomCount = other.mazeY_RoomCount;
	mazeKeyPtr = other.mazeKeyPtr;
	mazeDoorPtr = other.mazeDoorPtr;
	startPos = other.startPos;
	finalPos = other.finalPos;
	return *this;
}*/

Maze::~Maze()
{
}
