/*
	The main cpp file is where gameplay actually occurs. It includes the 
	initialization of SDL and SDL variables, the creation of maze and player 
	objects, and the game loop that waits for inputs from the user. This game 
	only updates what is rendered after player inputs. 
*/

//C++ Libraries
#include <iostream>
#include <math.h>
#include <vector>
//SDL Libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include<SDL2/SDL_mixer.h>
//Defined Classes
#include "Maze.h"
#include "Player2.h"

//Make Global variables for things that will not change
SDL_Surface * textSurface = NULL;
SDL_Texture * textTexture = NULL;


Mix_Music *bgsong = NULL;


int level = NULL;

//PNG and Text files To Use in this 
#define IMG_TITLE "res/Images/Title.png"
#define IMG_GAMEOVER "res/Images/GameOver.png"
#define TEXTFILE "KaushanScript-Regular.otf"


//Display Variables. These are kept as variables to make it easier to 
//manipulate and change as needed
int curWindowWidth = 1024;
int curWindowHeight = 768;

SDL_Rect FullScreen{ 0, 0, curWindowWidth, curWindowHeight };

//Gameplay Area Display Parameters
int GameArea_Width = 640;
int GameArea_Height = 640;
int GameArea_xOffset = 0;
int GameArea_yOffset = (curWindowHeight - GameArea_Width);

SDL_Rect GameAreaFillRect{	GameArea_xOffset, 
							GameArea_yOffset, 
							GameArea_Width, 
							GameArea_Height };

//Game Title Parameters
int GameTitle_Width = curWindowWidth;
int GameTitle_Height = curWindowHeight - GameArea_Height;
int GameTitle_xOffset = 0;
int GameTitle_yOffset = 0;
int GameTitle_FontSize = 72;

SDL_Rect GameTitleFillRect
{ 
	GameTitle_xOffset,
	GameTitle_yOffset,
	GameTitle_Width,
	GameTitle_Height 
};

//Game Info GUI Parameters
int GameInfo_Width = curWindowWidth - GameArea_Width;
int GameInfo_Height = curWindowHeight - GameTitle_Height;
int GameInfo_xOffset = GameArea_Width;
int GameInfo_yOffset = GameTitle_Height;

SDL_Rect GameInfoFillRect
{ 
	GameInfo_xOffset,
	GameInfo_yOffset,
	GameInfo_Width,
	GameInfo_Height 
};

//Level Complete Screen Information
int LevelComplete_Width = curWindowWidth * 8 / 10;
int LevelComplete_Height = curWindowHeight * 8 / 10;
int LevelComplete_xOffset = curWindowWidth / 10;
int LevelComplete_yOffset = curWindowHeight / 10;;

SDL_Rect levelCompleteFillRect
{	
	LevelComplete_xOffset,
	LevelComplete_yOffset,
	LevelComplete_Width,
	LevelComplete_Height
};

SDL_Rect levelCompleteTitleRect
{	
	levelCompleteFillRect.x,
	levelCompleteFillRect.y,
	levelCompleteFillRect.w,
	levelCompleteFillRect.h / 3 
};

SDL_Rect levelCompleteInfoRect
{
	levelCompleteTitleRect.x,
	levelCompleteTitleRect.y + levelCompleteTitleRect.h,
	levelCompleteTitleRect.w,
	levelCompleteFillRect.h - levelCompleteTitleRect.h
};
//Game Window
SDL_Window* window = NULL;

//Game Renderer
SDL_Renderer* renderer = NULL;

enum GameState{Menu, InGame, LevelComplete, GameOver};
GameState curGameState = Menu;

//Initialize the SDL2
bool initialization()
{
	bool success = true;
	//Initialize SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		success = false;
	}
	else
	{	
		//Create window
		window = SDL_CreateWindow("Mini Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, curWindowWidth, curWindowHeight, SDL_WINDOW_SHOWN);
		if (window == NULL)
		{
			printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
			success = false;
			SDL_Quit();
		}
		else
		{
			Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
			bgsong = Mix_LoadMUS("black swan-bts.mp3");

			//Create renderer for window
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

			if (renderer == NULL)
				{
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				success = false;
				SDL_DestroyWindow(window);
 				SDL_Quit();
				}
				//Initialize Fonts
				else if (TTF_Init() == -1)
				{
				printf("SDL TTF could not initialize");
				success = false;
				SDL_DestroyWindow(window);
				TTF_Quit();
 				SDL_Quit();
				}
			
		}

		

	}
	return success;
}

//Function to Display a Full Screen Texture based off the file name
void FullScreenTexture(const char* fileName)
{
	SDL_SetRenderDrawColor(renderer, 38, 19, 111, 255);
	SDL_RenderClear(renderer);

	SDL_Texture* screenTexture = IMG_LoadTexture(renderer, fileName);
	if (screenTexture == NULL)
	{
		printf("SDL could not load the Texture from ");
		printf(fileName);
		SDL_DestroyTexture(screenTexture);
		SDL_DestroyWindow(window);
 		SDL_Quit();
		return;
	}
	SDL_RenderCopy(renderer, screenTexture, NULL, &FullScreen);
	SDL_RenderPresent(renderer);
}

//Draw Text On Screen in a set, boxed area
void DrawTextFromRect(std::string text, SDL_Rect * textRect, int fontSize)
{
	//Select Font and Color
	TTF_Font *font = TTF_OpenFont(TEXTFILE, 40);
	if (font == NULL)
	{
		printf("SDL could not load the Font from ");
		printf(TEXTFILE);
		printf("Oh My Goodness, an error : %s", TTF_GetError());
		return;
	}
	SDL_Color fontColor = { 255, 255, 255, 255 };

	SDL_DestroyTexture(textTexture);        // Free up texture
    SDL_FreeSurface(textSurface);		// Free up surface

	textSurface = TTF_RenderText_Solid(font, text.c_str(), fontColor);

	//Offsets required to make the text the proper size and scale on screen
	int text_xOffset = textRect->x + (textRect->w - textSurface->w) / 2;
	int text_yOffset = textRect->y + (textRect->h - textSurface->h) / 2;

	//Creates Texture and Rect for displaying texture
	textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
	SDL_Rect DisplayTextRect = { text_xOffset, text_yOffset, textSurface->w, textSurface->h };

	//Adds Texture and Outline to Renderer
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, textRect);
	SDL_RenderCopy(renderer, textTexture, NULL, &DisplayTextRect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, textRect);

    TTF_CloseFont( font );
}

//Draw Text based off parameters passed
void DrawTextFromRectParams(std::string text, int xOffset, int yOffset, int width, int height, int fontSize)
{
	//Create Rect where Text will go
	SDL_Rect textRect = { 
		xOffset,
		yOffset,
		width,
		height 
	};
	DrawTextFromRect(text, &textRect, fontSize);
}

//Draw Text on Screen that has a number next to it. This is more for visual 
//effect of the data string being centered in a small box next to its associated text.
void DrawTextWithAdjCenteredNumber(std::string text, std::string count, int xOffset, int yOffset, int width, int height, int fontSize)
{
	//Write Text Portion
	int textXOffset = xOffset;
	int textYOffset = yOffset;
	int textWidth = width - height;
	int textHeight = height;
	DrawTextFromRectParams(text, textXOffset, textYOffset, textWidth, textHeight, fontSize);
	
	//WriteIntPortion
	int intXOffset = textXOffset + textWidth;
	int intYOffset = yOffset;
	int intWidth = height;
	int intHeight = height;
	DrawTextFromRectParams(count, intXOffset, intYOffset, intWidth, intHeight, fontSize);	
}

//Adds Multiple Rows of Text to the Renderer. 
void DrawTextMultipleRows(std::vector<std::string> &text, std::vector<std::string> &data, int xOffset, int yOffset, int width, int height, int fontSize)
{
	int rowCount = text.size();
	int rowHeight = height / rowCount;
	for (int i = 0; i < rowCount; i++)
	{
		//If the text has an associated data component to display, display that data
		if(i < static_cast<int>( data.size()))
		{
			DrawTextWithAdjCenteredNumber(text[i], data[i], xOffset, yOffset + rowHeight * i, width, rowHeight, fontSize);
		}
		else
		{
			DrawTextFromRectParams(text[i], xOffset, yOffset + rowHeight * i, width, rowHeight, fontSize);
		}
	}
}

//Draws in-game information
void DrawInGameGUI(std::string level, std::string steps, std::string lives)
{
	std::vector<std::string> infoTexts = {"Level", "Steps", "Lives", "R to Reset", "ESC to Menu" }; 
	std::vector<std::string> dataTexts =  { level, steps, lives };
	DrawTextFromRect("CHIBI SPY @ - @", &GameTitleFillRect, GameTitle_FontSize);
	DrawTextMultipleRows(infoTexts, dataTexts, GameInfo_xOffset, GameInfo_yOffset, GameInfo_Width, GameInfo_Height, 42);
}

//Draw in-game information with ints instead of Strings
void DrawInGameGUI(int level, int steps, int lives)
{
	DrawInGameGUI(std::to_string(level), std::to_string(steps), std::to_string(lives));
}

//Draw in-game information using "-" for blank counts of particular data
void DrawInGameGUI()
{
	DrawInGameGUI("-", "-", "-");
}

//Front Screen of the game. Returns true if you press the Escape key or close out, 
//and returns false if you press any other key to continue the game.
bool FrontScreen()
{
	//Create Front End Screen
	FullScreenTexture(IMG_TITLE);
	
	SDL_Event * startEvent = new SDL_Event;
	while (startEvent->type != SDL_KEYDOWN && startEvent->type != SDL_QUIT)
	{
		//Do something when there is an event
		SDL_WaitEvent(startEvent);
	}
	//If the player hits the ESC key or hits the Close Window button
	return startEvent->key.keysym.sym == SDLK_ESCAPE || startEvent->type == SDL_QUIT;
}

//Game Over Screen.
bool GameOverScreen()
{
	FullScreenTexture(IMG_GAMEOVER);
	SDL_Event * event = new SDL_Event;

	while (event->type != SDL_KEYDOWN && event->type != SDL_QUIT)
	{
		SDL_WaitEvent(event);
	}
	//Quits the Game
	return event->type == SDL_QUIT;
}

//Pop up after a level is complete. Returns True if Quitting, returns false if staying
bool LevelCompleteScreen(int level, int deaths)
{
	int LevelComplete_FontSize = 64;

	//Add Background Rect and Outline to Renderer
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderFillRect(renderer, &levelCompleteFillRect);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderDrawRect(renderer, &levelCompleteFillRect);

	//Add Title To Renderer
	DrawTextFromRect("Level Complete!", &levelCompleteTitleRect, LevelComplete_FontSize);

	//Information in Level Complete Panel
	std::vector<std::string> levelComplete_TextRows = { "Current Score", "Lives Lost", "Press Space To Continue" };
	std::vector<std::string> levelComplete_DataRows = { std::to_string(level*100), std::to_string(deaths) };

	//Add Information To Renderer in Multiple Rows
	DrawTextMultipleRows(levelComplete_TextRows, levelComplete_DataRows, levelCompleteInfoRect.x, levelCompleteInfoRect.y, levelCompleteInfoRect.w, levelCompleteInfoRect.h, 42);
	SDL_RenderPresent(renderer);
	SDL_Event * continueEvent = new SDL_Event;
	
	//Check for User Input to see what to do next
	while (continueEvent->key.keysym.sym != SDLK_SPACE && continueEvent->type != SDL_QUIT)
	{
		//Do something when there is an event
		SDL_WaitEvent(continueEvent);
	}

	//Quits the Game
	return continueEvent->type == SDL_QUIT;
}

//Renders all necessary objects to the screen
void RenderAllGameObjects(std::shared_ptr<Maze> & maze, std::shared_ptr<Player> & player, int &steps)
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);
	DrawInGameGUI(maze->mazeLevel, steps, player->playerLives);
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	maze->AddMazeRoomsToRenderer(0);
	player->AddObjToRenderer();
	//Only start rendering obstacles after a particular level
	if (maze->mazeLevel > TRAPS_START)
		maze->AddMazeObstaclesToRenderer();
	//Check if the player has the key
	if (!player->hasKey)
		maze->mazeKeyPtr->AddObjToRenderer();
	maze->mazeDoorPtr->AddObjToRenderer();
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderPresent(renderer);
}

//Reset the maze to the Initial Conditions
void ResetToStart(std::shared_ptr<Maze> & maze, std::shared_ptr<Player> & player, int *level, int* steps, int* lives)
{
	*level = 1;
	*steps = 0;
	maze->SetMaze(3, 3, 1);
	player->ResetPlayer(maze->FindRoomByPos(maze->startPos), 10);
	*lives = player->playerLives;
	RenderAllGameObjects(maze, player, *steps);
}

int main(int argc, char *argv[])
{
	//Initialize SDL2
	if (!initialization())
	{
		printf("Failed to initialize!\n");
	}
	else 
	{
		
		Mix_PlayMusic(bgsong, -1);

		//Sets the Level, step count, and starting lives
		int level = 1 , steps = 0;
		int levelStartLives = 0;

		//Determines Starting Size of Maze 
		int mazeX = 3, mazeY = 3;

		bool firstLoad = true;
		bool quit = false;
		
		std::shared_ptr<Maze> curMaze;
		std::shared_ptr<Player> curPlayer;
		
		//Get the SDL Starting Event
		SDL_Event * gameEvent = new SDL_Event;

		//Game Running Loop
		while (!quit && gameEvent->type != SDL_QUIT && curGameState >= Menu && level<=7)
		{
			//Front End Menu Loop
			while(!quit && gameEvent->type != SDL_QUIT && curGameState == Menu && level<=7)
			{
				//Show the Front Screen
				quit = FrontScreen();

				//If Close Window or ESC key is pressed, quit the game
				if (quit )
				{
					break;
				}
				else 
				{
					curGameState = InGame;
					DrawInGameGUI();

					//First time the game is loaded
					if (firstLoad)
					{
						curMaze = std::shared_ptr<Maze>(new Maze(mazeX, mazeY, 
							GameArea_xOffset, GameArea_yOffset, GameArea_Width, 
							GameArea_Height, renderer, level, false));
						curPlayer = std::shared_ptr<Player>
							(new Player(curMaze->FindRoomByPos(curMaze->startPos), 
										renderer));
						levelStartLives = curPlayer->playerLives;
						firstLoad = false;
					}

					//Every subsequent time
					else
					{
						ResetToStart(curMaze, curPlayer, &level, &steps, 
							&levelStartLives);
					}
				}
				RenderAllGameObjects(curMaze, curPlayer, steps);
			}
			
			//Gameplay Loop
			while (!quit && gameEvent->type != SDL_QUIT && curGameState == InGame && level<=7) 
			{
				//Do something when there is an event
				SDL_WaitEvent(gameEvent);

				//If the Close Window button is pressed, quit the game
				if (gameEvent->type == SDL_KEYDOWN)
				{
					//Gets the Key Input
					auto keyInput = gameEvent->key.keysym.sym;

					//If "ESC" was pressed, leave the game loop
					if (keyInput == SDLK_ESCAPE )
					{
						//curGameState = Menu;		
						curGameState = GameOver;	
						break; 
					}

					//If "R" was pressed, reset the maze, reset the player position, and reduce the player life count by 1
					else if (keyInput == SDLK_r && curPlayer->playerLives > 1)
					{
						curPlayer->playerLives--;
						curMaze->ResetMaze();
						curPlayer->SetPlayerToStart(curMaze->FindRoomByPos(curMaze->startPos));
					}
				
					//If the player presses an arrow key, check for a movement
					else if(keyInput == SDLK_DOWN  ||
							keyInput == SDLK_LEFT  ||
							keyInput == SDLK_RIGHT ||
							keyInput == SDLK_UP)
					{
						//Moves the Player According to the Key Input
						if (curPlayer->PlayerMove(keyInput, renderer))
						{
							steps++;
							//Update the Maze
							curMaze->NextMazeCycle();

							if(curPlayer->CheckForObjects())
							{
								//If the player had damage dealt, check if 
								//they still have enough lives
								if (curPlayer->playerLives == 0)
								{
									curGameState = GameOver;
								}
							}
							if ((curPlayer->objPos == curMaze->finalPos &&
								curPlayer->hasKey))
							{
								curGameState = LevelComplete;
							}
						}
					}
					//sound on,off
					else if(keyInput == SDLK_k)
					{
						if(!Mix_PlayingMusic())
							Mix_PlayMusic(bgsong, -1);
						else if(Mix_PausedMusic())
							Mix_ResumeMusic();
						else
							Mix_PauseMusic();
						break;
					}
					else if(keyInput == SDLK_z)
					{
						Mix_HaltMusic();
						break;
					}

					else {
						std::cout << "Not a valid Key input!" << std::endl;
					}
					RenderAllGameObjects(curMaze, curPlayer, steps);
				}
			}
			
			//Level Complete Loop
			while (!quit && gameEvent->type != SDL_QUIT && curGameState == LevelComplete && level<8)
			{
				//Display level complete stats
				quit = LevelCompleteScreen(level, std::max(0, levelStartLives - curPlayer->playerLives));
				if (quit )
				{
					break;
				}
				curGameState = InGame;

				//Next Level Changes
				SDL_RenderClear(renderer);
				DrawInGameGUI();

				//Store how many lives the player started with
				levelStartLives = curPlayer->playerLives;
				//Increment level count
				level++;

				if(level==8)
				{
					curGameState == GameOver;
					quit = GameOverScreen();

				if (quit)
				{
					curGameState = Menu;	
				}
				else
				{
					curGameState = Menu;
				}					
				break;
				}

				//Reset Step Count
				steps = 0;
				//Increment Maze Size
				mazeX += 1;
				mazeY += 1;

				curMaze->NextLevelMaze();

				curPlayer->SetPlayerToStart(curMaze->FindRoomByPos(curMaze->startPos));

				RenderAllGameObjects(curMaze, curPlayer, steps);
			}
			
			//Game Over Loop
			while (!quit && gameEvent->type != SDL_QUIT && curGameState == GameOver && level<=8)		
			{
				quit = GameOverScreen();
				if (quit)
				{
					curGameState = Menu;	
				}
				else
				{
					curGameState = Menu;
				}
			}
		}
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_FreeMusic(bgsong);

	//SDL_DestroyTexture(screenTexture);
	SDL_DestroyWindow(window);

	TTF_Quit();		

	Mix_Quit();

	SDL_Quit();
	return 0;
}
