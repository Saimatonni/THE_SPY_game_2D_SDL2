#ifndef BASE_H
#define BASE_H
#include <SDL2/SDL.h>
class Mother{
	protected:
		bool collision(SDL_Rect*, SDL_Rect*, int x = 0); 
	public:
		static SDL_Rect coord;
		static const int TILE_SIZE=50;
};
#endif
