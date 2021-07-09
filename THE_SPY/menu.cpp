#include "menu.h"
#include "game.h"
#include<SDL2/SDL_mixer.h>

Mix_Music *bgsong = NULL;
bool compareScores(pair<int, string> a, pair<int, string> b)
{
	return a.first > b.first;
}

Module::Module()
{
	fullscreen = 0;
}


Module::~Module()
{
	//DestroyWindowRenderer();
}

void Module::StartNewGame()
{
	//SDL_ShowCursor(SDL_DISABLE);
	game g(window, renderer, this);
	if(isContinue)
	{
		g.isContinue = 1;
	}
	else g.isContinue = 0;
	g.start();
	SDL_ShowCursor(SDL_ENABLE);
	return;
}

void Module::StartMenu()
{
	SDL_Init(SDL_INIT_EVERYTHING);
	TTF_Init();
	CreateWindowRenderer();
	Mix_PlayMusic(bgsong, -1);
	GameRunning = 1;
	std::cout << "SPY LOADING..." << endl;
	bool onceFlag = 1;
	int TotalOptions = 5;
	int currentOption = 0;
	while (GameRunning)
	{
		if(savedGameAvailable) TotalOptions = 5;
		else TotalOptions = 4;
		HandleArrow(currentOption);
		
		SDL_RenderCopy(renderer, background, &backgroundSrcRect, NULL);
		//SDL_RenderCopy(renderer, gameTitle, NULL, &forGameTitle);
		SDL_RenderCopy(renderer, scoreButton, NULL, &forScoreButton);
		SDL_RenderCopy(renderer, playButton, NULL, &forPlayButton);
		//SDL_RenderCopy(renderer, settingsButton, NULL, &forSettingsButton);

		SDL_RenderCopy(renderer, arrowIndicator, NULL, &forArrowIndicator);
		SDL_RenderCopy(renderer, exitButton, NULL, &forExitButton);
		SDL_RenderCopy(renderer, instructionsButton, NULL, &forInstuctionButton);
		if(savedGameAvailable) SDL_RenderCopy(renderer, continueButton, NULL, &forContinueButton);
		SDL_RenderPresent(renderer);
		
		
		if (onceFlag)
		{
			std::cout << "0 - Start" << endl
				<< "1 - Highest Scores" << endl
				<< "2 - Settings" << endl
				<< "3 - Exit" << endl;
			onceFlag = 0;
			
		}
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				GameRunning = false;
				DestroyWindowRenderer();
				break;
			case SDL_KEYDOWN:
				std::cout << "Keypress detected" << endl;
				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE:
					isContinue = 0;
					StartNewGame();
					onceFlag = 1;
					break;
				
				case SDLK_h:
					HighestScoreMenu();
					onceFlag = true;
					break;
				case SDLK_ESCAPE:
					Exit();
					DestroyWindowRenderer();
					break;
				case SDLK_DOWN:
					currentOption++;
					currentOption %= TotalOptions;
					onceFlag = true;
					//cout << currentOption << endl;
					break;
				case SDLK_UP:
					currentOption--;
					currentOption += TotalOptions;
					currentOption %= TotalOptions;
					onceFlag = true;
					//cout << currentOption << endl;
					break;
				case SDLK_KP_ENTER:
					std::cout << "You Pressed Enter!" << endl;
					HandleEnterKeyPress(currentOption);
					onceFlag = 1;
					break;
				case SDLK_RETURN:
					std::cout << "You Pressed Enter!" << endl;
					HandleEnterKeyPress(currentOption);
					onceFlag = 1;
					break;
				case SDLK_p :
					savedGameAvailable = !savedGameAvailable;
					if(savedGameAvailable) TotalOptions = 6;
					else TotalOptions = 5;
					currentOption %= TotalOptions;
					HandleArrow(currentOption);
					cout << currentOption << endl;
					break;
				case SDLK_k :
					if(!Mix_PlayingMusic())
						Mix_PlayMusic(bgsong, -1);
					else if(Mix_PausedMusic())
						Mix_ResumeMusic();
					else
						Mix_PauseMusic();
					break;

				case SDLK_z :
					Mix_HaltMusic();
					break;
					
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				std::cout << "Mouse Pressed" << endl;
				SDL_GetMouseState(&xMouse, &yMouse);
				if(savedGameAvailable && isInside(xMouse, yMouse, forContinueButton))
				{
					isContinue = 1;

					StartNewGame();
					
				}
				else if (isInside(xMouse, yMouse, forScoreButton))
				{
					HighestScoreMenu();
					onceFlag = true;
				}
				else if(isInside(xMouse, yMouse, forInstuctionButton))
				{
					cout << "Instructions" << endl;
					InstructionMenu();
				}
				else if (isInside(xMouse, yMouse, forPlayButton))
				{
					isContinue = 0;
					StartNewGame();
					onceFlag = true;
				}
				
				else if (isInside(xMouse, yMouse, forExitButton))
				{
					Exit();
					DestroyWindowRenderer();
				}
				break;
			case SDL_MOUSEMOTION:
				//std::cout << "MOUSE MOVED" << endl;
				SDL_GetMouseState(&xMouse, &yMouse);
				if( savedGameAvailable && isInside(xMouse, yMouse, forContinueButton))
				{
					currentOption = 4;
				}
				if (isInside(xMouse, yMouse, forScoreButton))
				{
					currentOption = 2;
				}
				if (isInside(xMouse, yMouse, forInstuctionButton))
				{
					currentOption = 1;
				}
				else if (isInside(xMouse, yMouse, forPlayButton))
				{
					currentOption = 0;
				}
				
				else if (isInside(xMouse, yMouse, forExitButton))
				{
					currentOption = 3;
				}
				break;
			}
		}
	}
		
		
	DestroyWindowRenderer();
}

bool Module::ResumeMenu(SDL_Window *windowfromgame, SDL_Renderer *rendererfromgame)
{
	window = windowfromgame; renderer = rendererfromgame;
	TTF_Init();
	SDL_ShowCursor(SDL_ENABLE);
	Paused = 1;
	int TotalOptions = 2;
	int currentOption = 0;
	bool onceFlag = 1;
	bool returnValue;
	Mix_PlayMusic(bgsong, -1);
	LoadMedia();
	LoadResumeMedia();
	while (Paused)
	{
		HandleArrowinResume(currentOption);
		
		SDL_RenderCopy(renderer, background, &backgroundSrcRect, NULL);
		SDL_RenderCopy(renderer, gamePaused, NULL, &forGamePaused);
		SDL_RenderCopy(renderer, resumeButton, NULL, &forResumeButton);
		SDL_RenderCopy(renderer, leaveGame, NULL, &forLeaveGame);
		SDL_RenderCopy(renderer, resumeArrow, NULL, &forResumeArrow);
		
		SDL_RenderPresent(renderer);
		if (onceFlag) {
			std::cout << "You paused the game!" << endl
				<< "Press space to resume." << endl
				<< "Press escape to Quit." << endl;
			onceFlag = false;
		}
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Resume();
				returnValue = 0;
				Exit();
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_SPACE:
					Resume();
					returnValue = 1;
					break;
				case SDLK_ESCAPE:
					Resume();
					returnValue = 0;
					break;
				case SDLK_UP:
					currentOption--;
					currentOption += TotalOptions;
					currentOption %= TotalOptions;
					break;
				case SDLK_DOWN:
					currentOption++;
					currentOption %= TotalOptions;
					break;
				case SDLK_RETURN:
					returnValue = HandleEnterinResume(currentOption);
					break;
				case SDLK_KP_ENTER:
					returnValue = HandleEnterinResume(currentOption);
					break;
				case SDLK_k :
					if(!Mix_PlayingMusic())
						Mix_PlayMusic(bgsong, -1);
					else if(Mix_PausedMusic())
						Mix_ResumeMusic();
					else
						Mix_PauseMusic();
					break;

				case SDLK_z :
					Mix_HaltMusic();
					break;	
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				std::cout << "Mouse Pressed" << endl;
				SDL_GetMouseState(&xMouse, &yMouse);
				if (isInside(xMouse, yMouse, forResumeButton))
				{
					Resume();
					returnValue = 1;
				}
				else if (isInside(xMouse, yMouse, forLeaveGame))
				{
					Resume();
					returnValue = 0;
				}
				
				break;
			case SDL_MOUSEMOTION:
				
				SDL_GetMouseState(&xMouse, &yMouse);
				if (isInside(xMouse, yMouse, forResumeButton))
				{
					currentOption = 0;
				}
				else if (isInside(xMouse, yMouse, forLeaveGame))
				{
					currentOption = 1;
				}
				break;
			}
		}
		
	}
	DestroyResumeMedia();
	DestroyMedia();
	SDL_ShowCursor(SDL_DISABLE);
	return returnValue;
}


void Module::InstructionMenu()
{

	
	bool inInstructionMenu = 1;
	LoadInstructionMedia();
	while (inInstructionMenu)
	{
		
		SDL_RenderCopy(renderer, background, &backgroundSrcRect, NULL);
		SDL_RenderCopy(renderer, Instructions, NULL, &forinstruction);
		SDL_RenderCopy(renderer, backIns, NULL, &forBackIns);
		SDL_RenderPresent(renderer);
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Exit();
				inInstructionMenu = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					inInstructionMenu = false;
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				std::cout << "Mouse Pressed" << endl;
				SDL_GetMouseState(&xMouse, &yMouse);
				if (isInside(xMouse, yMouse, forBackIns))
				{
					inInstructionMenu = false;
				}
				break;
			}

		}
	}
	DestroyInstructionMedia();
}
void Module::LoadInstructionMedia()
{
	Instructions = LoadImage("instruction.png");
	SDL_Color textColor = { 255, 255, 255 };
	backIns = LoadText(font, "BACK", textColor);
	forBackIns.x = SCREEN_WIDTH - 110;
	forBackIns.h = 40;
	forBackIns.w = 90;
	forBackIns.y = 20;

	forinstruction.x = 0;
	forinstruction.y = 0;
	forinstruction.w = SCREEN_WIDTH;
	forinstruction.h = SCREEN_HEIGHT;
}

void Module::DestroyInstructionMedia()
{
	SDL_DestroyTexture(Instructions);
	SDL_DestroyTexture(backIns);
}


void Module::HighestScoreMenu()
{
	
	bool inHighScoreMenu = 1;
	bool onceFlag = true;
	LoadScoreMedia();
	while (inHighScoreMenu)
	{
		
		SDL_RenderCopy(renderer, background, &backgroundSrcRect, NULL);
		SDL_RenderCopy(renderer, woodback, NULL, &forWoodBack);
		SDL_RenderCopy(renderer, highScoreTitle, NULL, &forHighScoreTitle);
		SDL_RenderCopy(renderer, backfromScore, NULL, &forScoreBack);
		SDL_RenderCopy(renderer, resetScore, NULL, &forResetScore);
		if (onceFlag) {
			std::cout << "You entered Highest Score Menu!" << endl;
			LoadScoreData();
			std::cout << "Press Escape to return to main menu." << endl;
			onceFlag = false;
		}
		for (int i = 0; i < 5; i++)
		{
			
			SDL_RenderCopy(renderer, scoreNum[i], NULL, &rScoreNum[i]);
			SDL_RenderCopy(renderer, scoreName[i], NULL, &rScoreName[i]);
			SDL_RenderCopy(renderer, scoreValue[i], NULL, &rScoreValue[i]);
		}
		SDL_RenderPresent(renderer);
		while (SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				Exit();
				inHighScoreMenu = false;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					inHighScoreMenu = false;
					break;
				case SDLK_c:
					ClearScore();
					LoadScoreData();
					break;
				}
				break;
			case SDL_MOUSEBUTTONDOWN:
				std::cout << "Mouse Pressed" << endl;
				SDL_GetMouseState(&xMouse, &yMouse);
				if (isInside(xMouse, yMouse, forScoreBack))
				{
					inHighScoreMenu = false;
				}
				else if (isInside(xMouse, yMouse, forResetScore))
				{
					ClearScore(); LoadScoreData();
				}
				break;
			}

		}
	}
	DestroyScoreMedia();
}



void Module::GotoWindow()
{
	fullscreen = 0;
	DestroyWindowRenderer();
	CreateWindowRenderer();
}

void Module::Exit()
{
	GameRunning = 0;
}

void Module::Pause()
{
	Paused = 1;
}

void Module::Resume()
{
	Paused = 0;
}

void Module::LoadMedia()
{
	//font = TTF_OpenFont("ARCADE.TTF", 150);
	font = TTF_OpenFont("font1.ttf", 150);
	font2 = TTF_OpenFont("font2.ttf", 150);
	textColor = { 255,255,255 };
	redColor = { 255,0,0 };

	//totalScreen

	background = LoadImage("menu.png");
    bgsong = Mix_LoadMUS("bgsong.mp3");
	totalScreen.x = 0;
	totalScreen.y = 0;
	totalScreen.w = SCREEN_WIDTH;
	totalScreen.h = SCREEN_HEIGHT;

	//background animation
	backgroundSrcRect.x = 0;
	backgroundSrcRect.y = 0;
	backgroundSrcRect.w = 1300;
	backgroundSrcRect.h = 975;

	

	continueButton = LoadText(font, "Continue", textColor);

	forContinueButton.x = 80;
	forContinueButton.y = 140;
	forContinueButton.w = 200;
	forContinueButton.h = 45;

	playButton = LoadText(font, "New Game", textColor);

	forPlayButton.x = 80;
	forPlayButton.y = forContinueButton.y + 55;
	forPlayButton.w = 200;
	forPlayButton.h = 45;

	instructionsButton = LoadText(font, "Instructions", textColor);
	
	forInstuctionButton.x = 80;
	forInstuctionButton.y = forPlayButton.y + 55;
	forInstuctionButton.w = 220;
	forInstuctionButton.h = 45;


	
	
	scoreButton = LoadText(font, "Highest Scores", textColor);

	forScoreButton.x = 80;
	forScoreButton.y = forInstuctionButton.y + 55;
	forScoreButton.h = forPlayButton.h;
	forScoreButton.w = 280;
	


	exitButton = LoadText(font, "Quit", textColor);

	forExitButton.x = forPlayButton.x;
	forExitButton.y = forScoreButton.y + 55;
	forExitButton.h = forPlayButton.h;
	forExitButton.w = 95;

	arrowIndicator = LoadText(font, " >>", redColor);

	forArrowIndicator.x = 0;
	forArrowIndicator.y = forContinueButton.y;
	forArrowIndicator.w = 33;
	forArrowIndicator.h = 45;

	//wood
	woodback = LoadImage("back.png");
	greenBack = LoadImage("Green.png");

}




void Module::LoadScoreData()
{
	//Loading DATA
	highScores.clear();
	int Input;
	FILE* ScoreFile = fopen("HighestScores.dat", "r");
	int num;
	char playerName[100];
	while (fscanf(ScoreFile, "%d %s", &num, playerName) != EOF)
	{
		pair<int, string> thisInput;
		thisInput.first = num;
		string thisName = playerName;
		thisInput.second = thisName;

		highScores.push_back(thisInput);
	}
	
	cout << "Eee" << endl;
	fclose(ScoreFile);
	while (highScores.size() < 10)
	{
		highScores.push_back(make_pair(0, "N/A"));
	}
	std::sort(highScores.begin(), highScores.end(), compareScores);
	for (int i = 0; i < 5; i++)
	{
		std::cout << i + 1 << ".\t" << highScores[i].second << "\t" << highScores[i].first << endl;
	}
	SDL_Color textColor = { 200,200,0 };
	//Loading Texture
	for (int i = 0; i < 5; i++)
	{
		string s = highScores[i].second;
		int value = highScores[i].first;
		char nameP[100],name[100];
		strcpy(nameP,s.c_str());
		sprintf(name, "%-7s", nameP);
		
		char num[100]; sprintf(num, "%5d", value);
		printf("%s\n", num);
		scoreName[i] = LoadText(font2, name, textColor);
		scoreValue[i] = LoadText(font2, num, textColor);
	}
}

void Module::WinScreen()
{
	bool quit = false;
	SDL_Texture* Win = LoadText(font, "You Win", textColor);
	SDL_Rect rect = { 250, 220, 140, 40 };
	SDL_Rect bRect = { 0, 0, 640, 480 };
	int start = SDL_GetTicks(), now;
	while (!quit)
	{
		cout << "HERE" << endl;
		//AnimateBackground();
		SDL_RenderCopy(renderer, background, &backgroundSrcRect, NULL);
		SDL_RenderCopy(renderer, greenBack, NULL, &bRect);
		SDL_RenderCopy(renderer, Win, NULL, &rect);
		SDL_RenderPresent(renderer);
		now = SDL_GetTicks();
		if (now - start > 3000) quit = true;
	}
	SDL_DestroyTexture(Win);
}

void Module::ClearScore()
{
	FILE *scoreFile = fopen("HighestScores.dat", "w");
	fclose(scoreFile);
}

void Module::LoadScoreMedia()
{
	//Score Title

	SDL_Color blackColor = { 200,200,0 };

	highScoreTitle = LoadText(font, "Highest Scores", redColor);

	forHighScoreTitle.x = 180;
	forHighScoreTitle.y = 20;
	forHighScoreTitle.w = 260;
	forHighScoreTitle.h = 50;


	//Loading Numbers

	for (int i = 1; i <= 5; i++)
	{
		char s[100];
		sprintf(s, "%d.", i);
		scoreNum[i - 1] = LoadText(font2, s, blackColor);
	}

	//Loading Rectangles
	rScoreNum[0].x = 130;
	rScoreNum[0].y = 90;
	rScoreNum[0].w = 40;
	rScoreNum[0].h = 36;

	for (int i = 1; i < 5; i++)
	{
		rScoreNum[i].x = rScoreNum[i - 1].x;
		rScoreNum[i].y = rScoreNum[i - 1].y + 45;
		rScoreNum[i].w = rScoreNum[i - 1].w;
		rScoreNum[i].h = rScoreNum[i - 1].h;
	}

	rScoreName[0].x = rScoreNum[0].x + 80;
	rScoreName[0].y = rScoreNum[0].y;
	rScoreName[0].w = 120;
	rScoreName[0].h = rScoreNum[0].h;

	for (int i = 1; i < 5; i++)
	{
		rScoreName[i].x = rScoreName[i - 1].x;
		rScoreName[i].y = rScoreName[i - 1].y + 45;
		rScoreName[i].w = rScoreName[i - 1].w;
		rScoreName[i].h = rScoreName[i - 1].h;
	}

	rScoreValue[0].x = rScoreName[0].x + 150;
	rScoreValue[0].y = rScoreName[0].y;
	rScoreValue[0].w = rScoreName[0].w;
	rScoreValue[0].h = rScoreName[0].h;

	for (int i = 1; i < 5; i++)
	{
		rScoreValue[i].x = rScoreValue[i - 1].x;
		rScoreValue[i].y = rScoreValue[i - 1].y + 45;
		rScoreValue[i].w = rScoreValue[i - 1].w;
		rScoreValue[i].h = rScoreValue[i - 1].h;
	}

	//BackButton

	backfromScore = LoadText(font, "BACK", textColor);
	forScoreBack.x = SCREEN_WIDTH - 110;
	forScoreBack.h = 40;
	forScoreBack.w = 90;
	forScoreBack.y = 20;

	//resetButton

	resetScore = LoadText(font, "RESET", redColor);
	forResetScore.x = rScoreNum[4].x;
	forResetScore.y = rScoreName[4].y + 50;
	forResetScore.w = 110;
	forResetScore.h = rScoreName[4].h + 10;

	forWoodBack.x = 0;
	forWoodBack.y = 0;
	forWoodBack.w = 640;
	forWoodBack.h = 480;

}

void Module::DestroyScoreMedia()
{
	SDL_DestroyTexture(highScoreTitle);
	SDL_DestroyTexture(backfromScore);
	SDL_DestroyTexture(resetScore);
	for (int i = 0; i < 5; i++)
	{
		SDL_DestroyTexture(scoreNum[i]);
		SDL_DestroyTexture(scoreValue[i]);
		SDL_DestroyTexture(scoreName[i]);
	}
}

void Module::LoadResumeMedia()
{
	//gamePaused
	gamePaused = LoadText(font, "Game Paused", redColor);

	forGamePaused.x = 180;
	forGamePaused.y = 150;
	forGamePaused.w = 260;
	forGamePaused.h = 70;

	
	resumeButton = LoadText(font, "Resume", textColor);

	forResumeButton.x = 45;
	forResumeButton.y = 250;
	forResumeButton.w = 140;
	forResumeButton.h = 45;

	//leaveGame
	leaveGame = LoadText(font, "Leave", textColor);
	
	forLeaveGame.x = forResumeButton.x;
	forLeaveGame.y = forResumeButton.y + 55;
	forLeaveGame.w = 120;
	forLeaveGame.h = forResumeButton.h;

	//Save and Leave

	saveandLeave = LoadText(font, "Save and Leave", textColor);

	forSaveAndLeave.x = forLeaveGame.x;
	forSaveAndLeave.y = forLeaveGame.y + 55;
	forSaveAndLeave.w = 310;
	forSaveAndLeave.h = forLeaveGame.h;

	//arrow

	resumeArrow = LoadText(font, " >>", redColor);

	forResumeArrow.x = 0;
	forResumeArrow.y = forResumeButton.y;
	forResumeArrow.w = 33;
	forResumeArrow.h = 45;
}

void Module::DestroyResumeMedia()
{
	SDL_DestroyTexture(gamePaused);
	SDL_DestroyTexture(resumeButton);
	SDL_DestroyTexture(leaveGame);
	SDL_DestroyTexture(saveandLeave);
	SDL_DestroyTexture(resumeArrow);
}

int Module::HandleEnterinResume(int currentOption)
{
	switch (currentOption)
	{
	case 0:
		Resume();
		return 1;
		break;
	case 1:
		Resume();
		return 0;
		break;
	default:
		break;
	}
	return 1;
}

void Module::HandleArrowinResume(int currentOption)
{
	if (currentOption == 0) forResumeArrow.y = forResumeButton.y;
	else if (currentOption == 1) forResumeArrow.y = forLeaveGame.y;
}

void Module::DestroyMedia()
{
	SDL_DestroyTexture(background);
	Mix_FreeMusic(bgsong);
	SDL_DestroyTexture(scoreButton);
	SDL_DestroyTexture(playButton);
	SDL_DestroyTexture(exitButton);
	
	SDL_DestroyTexture(arrowIndicator);
	//SDL_DestroyTexture(gameTitle);
	SDL_DestroyTexture(instructionsButton);
	SDL_DestroyTexture(woodback);
	SDL_DestroyTexture(greenBack);
}

SDL_Texture * Module::LoadText(TTF_Font * font, const char * Text, SDL_Color color)
{
	SDL_Surface* tmp = TTF_RenderText_Solid(font, Text, color);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	return tex;
}

SDL_Texture * Module::LoadImage(const char * ImgName)
{
	SDL_Surface* tmp = IMG_Load(ImgName);
	SDL_Texture* tex = SDL_CreateTextureFromSurface(renderer, tmp);
	SDL_FreeSurface(tmp);
	return tex;
}

void Module::HandleArrow(int currentOption)
{
	//cout << currentOption << endl;
	switch (currentOption)
	{
	case 4:
		forArrowIndicator.y = forContinueButton.y;
		break;
	case 0:
		forArrowIndicator.y = forPlayButton.y;
		break;
	case 1:
		forArrowIndicator.y = forInstuctionButton.y;
		break;
	case 2:
		forArrowIndicator.y = forScoreButton.y;
		break;
	
	case 3:
		forArrowIndicator.y = forExitButton.y;
		break;
	default:
		break;
	}
}

void DeleteSaved()
{
	ofstream out("GameSave.dat");
	out << -1 << endl;
	out.close();
}

void Module::HandleEnterKeyPress(int currentOption)
{
	switch (currentOption)
	{
	case 0 :
		
		isContinue = 0;
		StartNewGame();
		break;
	case 1 :
		InstructionMenu();
		break;
	case 2 :
		HighestScoreMenu();
		break;
	
	case 3:
		Exit();
		break;
	case 4:
		isContinue = 1;
		StartNewGame();
		
		break;
	}
}

void Module::CreateWindowRenderer()
{
	if(!fullscreen) window = SDL_CreateWindow("SPY", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	else window = SDL_CreateWindow("SPY", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, fullscreen);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096);
	LoadMedia();
}

void Module::DestroyWindowRenderer()
{
	DestroyMedia();
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	Mix_CloseAudio();
}

bool Module::isInside(int x, int y, SDL_Rect box)
{
	if (x < box.x) return false;
	if (x > box.x + box.w) return false;
	if (y < box.y) return false;
	if (y > box.y + box.h) return false;
	return true;
}


void Module::AddHighScore(int x)
{
	FILE *scoreFile = fopen("HighestScores.dat", "a");
	fprintf(scoreFile, "%d SECRET\n", x);
	fclose(scoreFile);
}

string Module::HighestScoreInput(int x)
{
		SDL_SetRenderDrawColor( renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( renderer );
	//cout << "HELLO " << endl;

	SDL_StartTextInput();
	string inputText;

	bool inputTaken = false;
	char scoreValue[100];
	sprintf(scoreValue, "Your Score: %6d", x);
	SDL_Texture* textureR = NULL;
	SDL_Texture* YourScore = LoadText(font2, scoreValue, { 200,200,0 });
	SDL_Texture* InputRequired = LoadText(font2, "Please Enter Your Name: ", {200, 200, 0});
	SDL_Rect dest2 = {50, 200, 540, 50};
	SDL_Rect dest3 = { 110, 100, 420, 50 };

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
				textureR = LoadText(font, inputText.c_str(), {0, 255, 0});
			}
			else
			{
				
				//gInputTextTexture.loadFromRenderedText( " ", textColor );
				textureR = LoadText(font, " ", {0, 255, 0});
			}
		}

		//Clear screen
		SDL_SetRenderDrawColor( renderer, 0, 0, 0, 0xFF );
		SDL_RenderClear( renderer );
		SDL_RenderCopy(renderer, InputRequired, NULL, &dest2);
		SDL_RenderCopy(renderer, YourScore, NULL, &dest3);

		//Render text textures
		SDL_Rect dest = {200, 300, inputText.length() * 40, 50};
		SDL_RenderCopy(renderer, textureR, NULL, &dest);
		
		SDL_RenderPresent( renderer );
	}

	cout << inputText << endl;
	FILE *scoreFile = fopen("HighestScores.dat", "a");
	fprintf(scoreFile, "%d %s\n", x, inputText.c_str());
	fclose(scoreFile);

	return inputText;
	
}



