#ifndef __GAME_H__
#define __GAME_H__

#include "main.h"

void game_init(SDL_Surface *screen);
void game_frame(SDL_Surface *screen);
void game_handle_keys();
int game_get_key(int key);
int game_get_key_DAS(int key);

typedef struct drop
{
	tetromino *t;
	int x, y, rot, lock;
} drop;

#define TK_UP 0
#define TK_DOWN 1
#define TK_LEFT 2
#define TK_RIGHT 3
#define TK_A 4
#define TK_B 5
#define TK_C 6
#define TK_HOLD 7

#define TS_FALLING 0 
#define TS_LINECLEAR 1
#define TS_ENTRY 2

#define T_ENTRYDELAY 30
#define T_LOCKDELAY 30
#define T_CLEARDELAY 41
#define T_DASDELAY 14
#endif
