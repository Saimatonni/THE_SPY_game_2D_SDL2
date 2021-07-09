#include <iostream>
#include <fstream>
#include <vector>
#include <SDL2/SDL.h>
#include "collision.h"
#include "Player.h"
#include "bullet.h"
#include "Enemy.h"
#include <SDL2/SDL_image.h>
#include "Collect.h"
#include "menu.h"
using namespace std;
#ifndef GAME_H
#define GAME_H


class game : public Mother{

	SDL_Window* window;
	SDL_Renderer* renderer;
	SDL_Rect camera;	//the relative coordinate of the game
	std::vector<std::vector<int> > map;
	std::vector<Enemy*> enemies;
	SDL_Texture *blocks, *screen, *background, *coinImg, *gunImg, *bossenemyImg, *heart;
	bool isRunning;
	bool direction[2];
	Player* Player1;
	std::vector<bullet*> bullets;
	SDL_Rect finish;
	static const int SCREEN_WIDTH=640;
	static const int SCREEN_HEIGHT=480;
	Collectables *coin, *bossenemy;
	Collectables* gun;
	bool isMenuOn;
	vector<string> maps;
	int globalX, camX;
	int currentMap;
	int totalMap;
	int arr[100];
	int lastInd = 0;
	int ind = 0;
	 int lives = 3;
	SDL_Texture* saveOptionButtons[3];
	SDL_Rect saveOptionRects[3], saveOptionBacks[3], forBlueBack;
	TTF_Font* font2;
public:

	string str;
	Uint32 fireTime = 0;
	SDL_Texture * LoadText(TTF_Font * font, const char * Text, SDL_Color color);
	int isContinue;
	bool isInside(int x, int y, SDL_Rect box);
	void SaveMapNew();
	void LoadmapEX();
	string InputName(int x);
	int ShowOptions();
	void SaveMap();
	void LoadMap();
	void Loadmap(const char* c);
	void Showmap();
	void HandleEvents();
	void CheckFrameRate(Uint32 start);
	void LoadLevels();
	void DisplacePlayer();
	Module *Menu, *MainMenu;
	SDL_Texture* LoadImage(const char* ImgName);
	game();
	game(SDL_Window* windowfrommenu, SDL_Renderer* rendererfrommenu, Module *menu);
	~game();
	void start();
	void BulletView();
};

#endif
