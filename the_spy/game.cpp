#include "game.h"
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <cstdio>
#include <string>
using namespace std;

#define framerate 40

SDL_Rect Mother::coord;


game::game(SDL_Window* windowfrommenu, SDL_Renderer* rendererfrommenu, Module *menu)
{
	memset(arr, 0, sizeof(arr));
	if (TTF_Init() == -1)
	{
		cout << "Error" << endl;
	}
	currentMap = 0;
	window = windowfrommenu;
	renderer = rendererfrommenu;
	Menu = new Module;
	MainMenu = menu;
	heart = LoadImage("Heart.png");
	blocks = LoadImage("blocks.png");
	background = LoadImage("background.png");
	coinImg = LoadImage("coin.png");
	gunImg = LoadImage("gun.png");
	bossenemyImg = LoadImage("bossenemy.png");

	coin = new Collectables(coinImg, 6, 30);
	gun = new Collectables(gunImg, 1, 40);
	bossenemy = new Collectables(bossenemyImg, 1, 50);

	Mother::coord.x = camera.x = 0;
	Mother::coord.y = camera.y = 0;
	Mother::coord.w = camera.w = SCREEN_WIDTH;
	Mother::coord.h = camera.h = SCREEN_HEIGHT;
	font = TTF_OpenFont("font2.ttf", 150);

	isRunning = true;
	direction[0] = 0;
	direction[1] = 0;
	Player1 = new Player(LoadImage("Player.png"));
}

game::~game()
{
	SDL_DestroyTexture(blocks);
	SDL_DestroyTexture(background);
	SDL_DestroyTexture(coinImg);
	SDL_DestroyTexture(gunImg);
	SDL_DestroyTexture(bossenemyImg);

	blocks = NULL;
	background = NULL;
	blocks = NULL;
	background = NULL;
	delete Menu;
	MainMenu = NULL;
}

SDL_Texture* game::LoadImage(const char* ImgName)
{
	SDL_Surface* tmp = IMG_Load(ImgName);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tmp);
	return tex;
}

void game::Loadmap(const char* c)
{
	Mother::coord.x = 0;
	cout << "loading Level " << c << endl;
	ifstream in(c);

	int width;
	int height;
	int current;

	in >> width;
	in >> height;

	for(int i=0;i<height;i++)
	{
		vector<int> vec;
		for (int j = 0; j < width; j++)
		{
			if (in.eof())
				return;
			in >> current;
			if (current >= -4 && current <= 10)
			{
				if (current == -1) {
					//cout << ind << ": " << arr[ind] << endl;
					enemies.push_back(new Enemy(LoadImage("Enemy.png"), j * TILE_SIZE, i * TILE_SIZE - 10, 1, 0, ind++));
					vec.push_back(0);
					continue;
				}
				else if (current == 3) {
					finish.x = j * TILE_SIZE;
					finish.y = i * TILE_SIZE;
					vec.push_back(current);
				}
				else {
					vec.push_back(current);
				}
			}
			else {
				vec.push_back(0);
			}
		}

		map.push_back(vec);
	}
	in.close();
	if(finish.x==0 && finish.y==0)
	{
		finish.x=width*TILE_SIZE-TILE_SIZE;
	}
}

void game::LoadmapEX()
{
	int x = ShowOptions();

	if(x == 1)
	{
		str = "GameSave.dat";
	}

	else if(x == 2) str = "GameSave1.dat";
	else if(x == 3) str = "GameSave2.dat";
	else {
		isRunning = 0;
		return;
	}
	LoadMap();
}

void game::Showmap()
{
	int start = (Mother::coord.x - (Mother::coord.x % TILE_SIZE)) / TILE_SIZE;
	int end = (Mother::coord.x + Mother::coord.w + (TILE_SIZE - (Mother::coord.x + Mother::coord.w) % TILE_SIZE)) / TILE_SIZE;

	if(end>map[0].size())
		end=map[0].size();
	if(start<0)
		start=0;

	for (int i = 0; i < map.size(); i++) {
		for (int j = start; j < end; j++)
			if (map[i][j] != 0)
			{
				if (map[i][j] == -1)
				{
					continue;
				}

				if (map[i][j] == -2)
				{
					coin->Show(j * TILE_SIZE - Mother::coord.x, i * TILE_SIZE, renderer);
					continue;
				}

				if (map[i][j] == -3)
				{
					if(Player1->hasgun)
					{
						map[i][j] = 0;
						continue;
					}
					gun->Show(j * TILE_SIZE - Mother::coord.x, i * TILE_SIZE, renderer);
					continue;
				}

				if (map[i][j] == -4)
				{
					bossenemy->Show(j * TILE_SIZE - Mother::coord.x, i * TILE_SIZE, renderer);
					continue;
				}
				SDL_Rect blockRect = { (map[i][j] - 1) * TILE_SIZE,0,50,50};
				SDL_Rect destRect = { j * TILE_SIZE - Mother::coord.x,i * TILE_SIZE, 50, 50};
				SDL_RenderCopy(renderer, blocks, &blockRect, &destRect);
			}
	}
}

void game::HandleEvents()
{
	SDL_Event event;

	while(SDL_PollEvent(&event))
	{
		switch(event.type)
		{
			case SDL_QUIT:
				isRunning=false;
				MainMenu->Exit();
				if (Menu->GameRunning) std::cout << "LOL" << endl;
				std::cout << "You Pressed Quit!" << endl;
				return;

			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_s:
					SaveMap();
					break;
				case SDLK_l:
					LoadMap();
					break;
				case SDLK_F6:
					enemies.clear();
					bullets.clear();
					Player1->hasgun = false;
					Player1->nextLevel = true;
					break;
				case SDLK_ESCAPE:
					isRunning = Menu->ResumeMenu(window, renderer);
					if(!isRunning) SaveMap();
					break;
				case SDLK_LEFT:
					direction[0] = 1;
					Player1->setMoving(1);
					break;
				case SDLK_RIGHT:
					direction[1] = 1;
					Player1->setMoving(1);
					break;
				case SDLK_SPACE:
					Player1->Jump();
					break;
				case SDLK_UP:
					Player1->Jump();
					break;
				case SDLK_f:
					if (SDL_GetTicks() - fireTime > 500) {
						if (Player1->getDirection() == 'r') {

							fireTime = SDL_GetTicks();
							Player1->SetFiring(true);
							bullets.push_back(new bullet(LoadImage("bullet.png"),
								Player1->GetRect()->x + Player1->GetRect()->w, Player1->GetRect()->y+30, 5, 0));
						}
						else {
							Player1->SetFiring(true);
							bullets.push_back(new bullet(LoadImage("bullet.png"), Player1->GetRect()->x, Player1->GetRect()->y+30, -5, 0));
						}
					}
					break;
				}
				break;
			case SDL_KEYUP:
				switch (event.key.keysym.sym)
				{
				case SDLK_LEFT:
					direction[0] = 0;
					Player1->setMoving(0);
					break;
				case SDLK_RIGHT:
					Player1->setMoving(0);
					direction[1] = 0;
					break;
				case SDLK_f:
					Player1->SetFiring(false);
				}
				break;
		}
	}
}

void game::CheckFrameRate(Uint32 start)
{
	if (1000 / framerate > (SDL_GetTicks() - start))
		SDL_Delay(1000 / framerate - (SDL_GetTicks() - start));
	SDL_RenderPresent(renderer);
}

void game::DisplacePlayer()
{

	if (direction[0])
	{
		Player1->setDirection('l');
		if (Player1->physics.box.x > 200)
			Player1->physics.velocity.x = -3;
		else {
			Player1->physics.velocity.x = 0;
			camera.x--;
			Mother::coord.x -= 3;
		}
		if (camera.x < 0)
			camera.x = 2000 - SCREEN_WIDTH;
	}
	else if (direction[1])
	{
		Player1->setDirection('r');
		if (Player1->physics.box.x < 350)
			Player1->physics.velocity.x = 3;
		else {
			Player1->physics.velocity.x = 0;
			camera.x++;
			Mother::coord.x += 3;
		}
		if (camera.x >= 2000 - SCREEN_WIDTH)
			camera.x = 0;
	}
	else {
		Player1->physics.velocity.x = 0;
	}
	Player1->move(map);

}

void game::BulletView()
{

	for (int i = 0; i < bullets.size(); i++)
	{
		if (bullets[i]->GetRect()->x > SCREEN_WIDTH || bullets[i]->GetRect()->x < 0)
		{
			delete bullets[i];
			bullets.erase(bullets.begin() + i);
		}
	}

	for (int i = 0; i < bullets.size(); i++)
	{
		bullets[i]->Show(renderer);
		bullets[i]->move();
	}
}

void game::LoadLevels()
{
	ifstream in("Levels.dat");

	string str;
	for(int i = 0; i < 4; i++)
	{
		in >> str;
		maps.push_back(str);
	}
	totalMap = maps.size();

	in.close();

}


SDL_Texture * game::LoadText(TTF_Font * font, const char * Text, SDL_Color color)
{
	SDL_Surface* tmp = TTF_RenderText_Solid(font, Text, color);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	return tex;
}

int game::ShowOptions()
{
	ifstream in("SaveNames.dat");

	string slot[3];
	in >> slot[0] >> slot[1] >> slot[2];
	saveOptionButtons[0] = LoadText(font, slot[0].c_str(), {200, 200, 0});
	saveOptionButtons[1] = LoadText(font, slot[1].c_str(), {200, 200, 0});
	saveOptionButtons[2] = LoadText(font, slot[2].c_str(), {200, 200, 0});
	in.close();
	saveOptionRects[0] = {270, 200, 100, 30};
	saveOptionRects[1] = {270, 250, 100, 30};
	saveOptionRects[2] = {270, 300, 100, 30};

	saveOptionBacks[0] = { 260, 195, 120, 45 };
	saveOptionBacks[1] = { 260, 245, 120, 45 };
	saveOptionBacks[2] = { 260, 295, 120, 45 };
	forBlueBack = { 0,0,640,480 };
	int currentChoice = 0;
	///HighestScoreInput(500);
	bool inSaveGameMenu = 1;
	bool onceFlag = true;
	SDL_Event e;
	while (inSaveGameMenu)
	{
		currentChoice %= 3;

		//MainMenu->AnimateBackground();
		SDL_RenderCopy(renderer, MainMenu->background, &MainMenu->backgroundSrcRect, NULL);
		SDL_RenderCopy(renderer, MainMenu->woodback, NULL, &forBlueBack);
		SDL_RenderCopy(renderer, MainMenu->greenBack, NULL, &saveOptionBacks[currentChoice]);
		SDL_RenderCopy(renderer, saveOptionButtons[0], NULL, &saveOptionRects[0]);
		SDL_RenderCopy(renderer, saveOptionButtons[1], NULL, &saveOptionRects[1]);
		SDL_RenderCopy(renderer, saveOptionButtons[2], NULL, &saveOptionRects[2]);
		//SDL_RenderPresent(renderer);
		//SDL_RenderCopy(renderer, backfromScore, NULL, &forScoreBack);
		//SDL_RenderCopy(renderer, resetScore, NULL, &forResetScore);

		SDL_RenderPresent(renderer);
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				inSaveGameMenu = false;
				isRunning = false;
				MainMenu->Exit();
				return -1;
				break;
			case SDL_KEYDOWN:
				switch (e.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					inSaveGameMenu = false;
					isRunning = false;
					return -1;
					break;
				case SDLK_RETURN:
					inSaveGameMenu = false;
					return currentChoice + 1;
					break;
				case SDLK_UP:
					currentChoice--;
					currentChoice += 3;
					break;
				case SDLK_DOWN:
					currentChoice++;
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				int x, y;
				SDL_GetMouseState(&x, &y);
				if (isInside(x, y, saveOptionRects[0]))
				{
					return 1;
				}
				else if (isInside(x, y, saveOptionRects[1]))
				{
					return 2;
				}
				else if (isInside(x, y, saveOptionRects[2]))
				{
					return 3;
				}
				break;
			case SDL_MOUSEMOTION:
				//std::cout << "MOUSE MOVED" << endl;
				SDL_GetMouseState(&x, &y);
				if (isInside(x, y, saveOptionRects[0]))
				{
					currentChoice = 0;
				}
				else if (isInside(x, y, saveOptionRects[1]))
				{
					currentChoice = 1;
				}
				else if (isInside(x, y, saveOptionRects[2]))
				{
					currentChoice = 2;
				}
				break;
			}
		}
	}
}


bool game::isInside(int x, int y, SDL_Rect box)
{
	if (x < box.x) return false;
	if (x > box.x + box.w) return false;
	if (y < box.y) return false;
	if (y > box.y + box.h) return false;
	return true;
}

string game::InputName(int x)
{
	string saveNames[3];
	ifstream in("SaveNames.dat");
	in >> saveNames[0] >> saveNames[1] >> saveNames[2];


	SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( renderer );
	//cout << "HELLO " << endl;

	SDL_StartTextInput();
	string inputText;

	bool inputTaken = false;
	SDL_Texture* textureG = NULL;
	SDL_Texture* InputRequired = LoadText(font, "Please Enter Your Name: ", {200, 200, 0});
	SDL_Rect dest2 = {50, 200, 540, 50};
	SDL_Event event;

	while( !inputTaken )
	{
		//SDL_RenderPresent(renderer);
		bool renderText = false;

		while( SDL_PollEvent( &event ) != 0 )
		{
			if( event.type == SDL_QUIT )
			{
				inputTaken = true;
			}

			else if( event.type == SDL_KEYDOWN )
			{
				if((event.key.keysym.sym == SDLK_RETURN || event.key.keysym.sym == SDLK_SPACE) && inputText.length() > 0)
				{
					inputTaken = true;
				}
				if( event.key.keysym.sym == SDLK_BACKSPACE && inputText.length() > 0 )
				{
					inputText.pop_back();
					renderText = true;
				}

				else if( event.key.keysym.sym == SDLK_c && SDL_GetModState() & KMOD_CTRL )
				{
					SDL_SetClipboardText( inputText.c_str() );
				}

				else if( event.key.keysym.sym == SDLK_v && SDL_GetModState() & KMOD_CTRL )
				{
					inputText = SDL_GetClipboardText();
					renderText = true;
				}
			}
			//Special text input event
			else if( event.type == SDL_TEXTINPUT )
			{
				//Not copy or pasting
				if( !( SDL_GetModState() & KMOD_CTRL && ( event.text.text[ 0 ] == 'c' || event.text.text[ 0 ] == 'C' || event.text.text[ 0 ] == 'v' || event.text.text[ 0 ] == 'V' ) ) )
				{
					//Append character
					inputText += event.text.text;
					renderText = true;
				}
			}
		}

		if( renderText )
		{
			if( inputText != "" )
			{
				//gInputTextTexture.loadFromRenderedText( inputText.c_str(), textColor );
				textureG = LoadText(font, inputText.c_str(), {0, 255, 0});
			}
			else
			{

				//gInputTextTexture.loadFromRenderedText( " ", textColor );
				textureG = LoadText(font, " ", {0, 255, 0});
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0xFF );
		SDL_RenderClear( renderer );
		SDL_RenderCopy(renderer, InputRequired, NULL, &dest2);

		//Render text textures
		SDL_Rect dest = {200, 300, inputText.length() * 40, 50};
		SDL_RenderCopy(renderer, textureG, NULL, &dest);
		//gPromptTextTexture.render( ( SCREEN_WIDTH - gPromptTextTexture.getWidth() ) / 2, 0 );
		//gInputTextTexture.render( ( SCREEN_WIDTH - gInputTextTexture.getWidth() ) / 2, gPromptTextTexture.getHeight() );

		//Update screen
		SDL_RenderPresent( renderer );
	}

	saveNames[x] = inputText;
	in.close();

	ofstream out("SaveNames.dat");
	out << saveNames[0] << endl << saveNames[1] << endl << saveNames[2] << endl;
	out.close();


	return inputText;
}

void game::SaveMapNew()
{
	int x = ShowOptions();
	if(x == 1)
	{
		str = "GameSave.dat";
	}
	else if(x == 2)
	{
		str = "GameSave1.dat";
	}

	else if(x == 3)
	{
		str = "GameSave2.dat";
	}
	else {
		isRunning = 0;
		return;
	}

	InputName(x - 1);
	ofstream out(str.c_str());
	out << map.size() << " " << map[0].size() << endl;
	for(int i = 0; i < map.size(); i++)
	{
		for(int j = 0; j < map[i].size(); j++)
		{
			out << map[i][j] << ' ';
		}

		out << endl;
	}
	out << enemies.size() << endl;

	for(int i = 0; i < enemies.size(); i++)
	{
		out << enemies[i]->box.x << " " << enemies[i]->box.y << endl;
	}

	out <<  Player1->hud.score << endl;
	out <<  Mother::coord.x<< endl;
	out <<  camera.x<< endl;
	out << Player1->hasgun << endl;

	//out <<  camera.x << endl;
	out <<  Player1->physics.box.x<< endl;
	out <<  currentMap << endl;
	out.close();
}

void game::SaveMap()
{
	ofstream out(str.c_str());
	out << map.size() << " " << map[0].size() << endl;
	for(int i = 0; i < map.size(); i++)
	{
		for(int j = 0; j < map[i].size(); j++)
		{
			out << map[i][j] << ' ';
		}

		out << endl;
	}
	out << enemies.size() << endl;

	for(int i = 0; i < enemies.size(); i++)
	{
		out << enemies[i]->box.x << " " << enemies[i]->box.y << endl;
	}

	out <<  Player1->hud.score << endl;
	out <<  Mother::coord.x<< endl;
	out <<  camera.x<< endl;
	out << Player1->hasgun << endl;

	//out <<  camera.x << endl;
	out <<  Player1->physics.box.x<< endl;
	out <<  currentMap << endl;
	out << lives << endl;
	out.close();
}
void game::LoadMap()
{
	ifstream in(str.c_str());
	map.clear();
	int width, height, current;
	in >> height;
	in >> width;

	for(int i=0;i<height;i++)
	{
		vector<int> vec;
		for (int j = 0; j < width; j++)
		{
			in >> current;

			if (current >= -4 && current <= 10)
			{
				if (current == -1) {
					vec.push_back(0);
					continue;
				}
				else if (current == 3) {
					finish.x = j * TILE_SIZE;
					finish.y = i * TILE_SIZE;
					vec.push_back(current);
				}
				else {
					vec.push_back(current);
				}
			}
			else {
				vec.push_back(0);
			}

			//cout << current << " ";
		}
		//cout << endl;

		map.push_back(vec);
	}
	int x;

	in >> x;

	while(x--)
	{
		int a, b;
		in >> a >> b;

		enemies.push_back(new Enemy(LoadImage("Enemy.png"), a, b, 1, 0, 0));
	}

	int score;
	//LoadmapEX(maps[currentMap].c_str());

	in >> Player1->hud.score;
	in >> Mother::coord.x;
	in >> camera.x;
	in >> Player1->hasgun;

	//cout << camera.x << endl;
	in >> Player1->physics.box.x;
	Player1->physics.box.y = 0;
	in >> currentMap;
	in >> lives;
	//cout << currentMap << endl;
	//cout << score << endl;
	in.close();
}

void game::start()
{
	Uint32 start;
	if(!isContinue)
	{
		Loadmap("Level1.map");
		SaveMapNew();
	}
	LoadLevels();

	if(isContinue)
	{

		//LoadMap();
		LoadmapEX();
		//cout << Player1->physics.box.x << endl;
	}
	while(isRunning)
	{
		if (Player1->nextLevel)
		{
			//cout << "SCORE : " << Player1->hud.score << endl;
			map.clear();
			Player1->nextLevel = false;
			camera.x = 0;
			camera.y = 0;
			///cout << Mother::coord.x << endl;
			Player1->physics.box.x = 0;
			Player1->physics.box.y = 0;
			if(currentMap + 1 < totalMap){
				Loadmap(maps[++currentMap].c_str());
				SaveMap();
				Showmap();
			}

			else
			{
				MainMenu->WinScreen();
				MainMenu->HighestScoreInput(Player1->hud.score);
				cout << "YOU WIN" << endl;
				cout << Player1->hud.score << endl;
				isRunning = 0;
				MainMenu->savedGameAvailable = 1;
				break;
			}

		}
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		start=SDL_GetTicks();
		SDL_RenderCopy(renderer, background, &camera, NULL);


		Showmap();

		DisplacePlayer();



		int str = (Mother::coord.x - (Mother::coord.x % TILE_SIZE)) / TILE_SIZE;
		int end = (Mother::coord.x + Mother::coord.w + (TILE_SIZE - (Mother::coord.x + Mother::coord.w) % TILE_SIZE)) / TILE_SIZE;
		bool nc = 0;
		if (end > map[0].size())
			end = map[0].size();
		if(str<0)
			str=0;
		for (int i = 0; i < map.size(); i++)
		{
			for (int j = str; j < end; j++)
			{
				if (map[i][j] == 0)
					continue;
				SDL_Rect destrect = { j * TILE_SIZE - Mother::coord.x,i * TILE_SIZE,TILE_SIZE,TILE_SIZE };
				for (int g = 0; g < bullets.size(); g++)
					if (collision(&destrect, bullets[g]->GetRect()))
					{
						delete bullets[g];
						bullets.erase(bullets.begin() + g);
					}
			}
		}

		for(int i=0;i<enemies.size();i++){
			for (int j = 0; j < bullets.size(); j++)
			{
				if (enemies.size() <= i)
					break;
				SDL_Rect tmprec = { enemies[i]->GetRect()->x - Mother::coord.x,enemies[i]->GetRect()->y,40,40 };

				if (collision(&tmprec, bullets[j]->GetRect()))
				{
					arr[enemies[i]->number] = 1;
					Player1->hud.score += 50;
					delete enemies[i];
					enemies.erase(enemies.begin() + i);
					bullets.erase(bullets.begin() + j);
				}
			}
		}


		HandleEvents();


		if(Player1->getHealth()<=0 || Player1->GetRect()->y > SCREEN_HEIGHT)
		{
			if (lives <= 1)
			{
				MainMenu->HighestScoreInput(Player1->hud.score);
				isRunning = 0;
			}
			map.clear();
			enemies.clear();
			LoadMap();
			lives--;
			SaveMap();

			//LoadmapEX();
		}


		coin->Update();
		// Rendering Everything

		///Player1->hud.DrawScore();
		string strScore = to_string(Player1->hud.GetScore());
		SDL_Rect rect = { 300, 50, strScore.length() * 20, 0 };

		Player1->hud.RenderText(strScore.c_str(), NULL, &rect, renderer);
		for (int i = 0; i < enemies.size(); i++)
		{
			if (collision(&Mother::coord, enemies[i]->GetRect()))
			{
				SDL_Rect tmprect = { enemies[i]->GetRect()->x - Mother::coord.x + 20,enemies[i]->GetRect()->y + 5,enemies[i]->GetRect()->w - 30,enemies[i]->GetRect()->h - 5 };
				//SDL_RenderCopy(renderer, doorImg, NULL, &tmprect);
				if (collision(&tmprect, Player1->GetRect()))
				{
					if (lives <= 1)
					{
						MainMenu->HighestScoreInput(Player1->hud.score);
						isRunning = 0;
					}
					map.clear();
					enemies.clear();
					LoadMap();
					lives--;

					SaveMap();
					//LoadmapEX();

					// cout << Player1->getHealth() << endl;

				}
				enemies[i]->move(map);
				enemies[i]->Show(renderer);
			}
		}

		Player1->Show(renderer);
		BulletView();

		if (Player1->hasgun)
		{
			gun->Show(600, 20, renderer);
		}
		SDL_Rect rectScore = { 470, 20, 25, 25 };
		for (int i = 0; i < lives; i++) {
			rectScore.x += 30;
			SDL_RenderCopy(renderer, heart, NULL, &rectScore);

		}

		CheckFrameRate(start);
		if (Player1->nextLevel) cout << "SCORE = " << Player1->hud.score << endl;
	}
}
