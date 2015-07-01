#include "main.h"
#include "game.h"
#include <math.h>

static inline int min(int a, int b) { if(a>b) return b; else return a; }
int main(int argc, char **argv)
{
	#ifdef CALC_FRAMERATE
	float framerate = 0;
	#endif
	bool cont = true;
	int frames = 0;
	int time = 0;

	SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH,SCREEN_HEIGHT,32,0);
	Uint32 black = SDL_MapRGB(screen->format, 0, 0, 0);

	game_init(screen);

	do
	{
		time = SDL_GetTicks();

		SDL_Event e;
		while(SDL_PollEvent(&e))
		{
			switch(e.type)
			{
				case SDL_QUIT:
					cont = false;
			}
		}

		SDL_Rect fs = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };
		SDL_FillRect(screen, &fs, black);
		game_frame(screen);
		SDL_Flip(screen);

		int temp=SDL_GetTicks();
		if(temp-time < 17) SDL_Delay(16+min(frames%3,1) - (temp-time));

		#ifdef CALC_FRAMERATE
		if(time > 0)
		{
			int frametime = SDL_GetTicks()-time;
			if(frametime > 0)
				framerate = (1000./frametime)*0.05 + framerate*0.95;
		}
		#endif

		++frames;
	} while(cont);
}
