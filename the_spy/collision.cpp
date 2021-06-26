#include "collision.h"

bool Mother::collision(SDL_Rect* rec1,SDL_Rect* rec2, int x)
{
	if(rec2->y+rec2->h - x <= rec1->y)
		return 0;
	if(rec2->x + rec2->w - x <= rec1->x)
		return 0;
	if(rec1->y + rec1->h - x <= rec2->y)
		return 0;
	if(rec1->x + rec1->w - x <= rec2->x)
		return 0;
	return 1;
}
