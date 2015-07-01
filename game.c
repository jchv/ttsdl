#include "game.h"
#include "mt19937.h"
#include <time.h>

Uint32 block_color[256] = {0};
field *f = NULL;

rotation_system *rs = NULL;
tetromino *next_piece = NULL;
drop curr_piece;
tetromino *pieces[7];
int history[4];
int key_time[8];
int game_state = TS_ENTRY;
int are_time = T_ENTRYDELAY;
int clear_time = T_CLEARDELAY;

int _py(int y) { return ((y-0xFF)>>8); }
int _bx(int y) { return ((y<<8)+0xFF); }

void lock_piece()
{
	int cx,cy;
	int w=curr_piece.t->mask_size,h=curr_piece.t->mask_size;
	int fw=f->w, fh=f->h;
	char *frame = &curr_piece.t->mask[curr_piece.t->mask_size * curr_piece.t->mask_size * curr_piece.rot];
	
	for(cy=h-1;cy>=0;--cy)
	{
		for(cx=w-1;cx>=0;--cx)
		{
			if(frame[cy*w+cx] != 0)
				f->field[(cy+_py(curr_piece.y))*fw+curr_piece.x+cx] = frame[cy*w+cx];
		}
	}
}
void array_draw(char *f, int fw, int fh, SDL_Surface *s, int x, int y, int xoff, int yclip, int zerodraw)
{
	x += xoff * BLOCK_SIZE;
	int cx,cy;
	for(cy=fh-1;cy>=yclip;--cy)
	{
		for(cx=fw-1;cx>=0;--cx)
		{
			SDL_Rect rect = { BLOCK_SIZE*cx+x+1, BLOCK_SIZE*cy+y+1, BLOCK_SIZE-1, BLOCK_SIZE-1 };
			if(f[(cy-yclip)*fw+cx] != 0 || zerodraw)
				SDL_FillRect(s, &rect, block_color[(unsigned char)f[(cy-yclip)*fw+cx]]);
		}
	}
}
void field_draw(field *f, SDL_Surface *s, int x, int y) { array_draw(f->field, f->w, f->h, s, x, y, 0, 0, 1); }
void drop_draw(drop d, SDL_Surface *s, int x, int y) { array_draw(d.t->mask+(d.t->mask_size*d.t->mask_size)*d.rot, d.t->mask_size, d.t->mask_size, s, x+BLOCK_SIZE*d.x, y+BLOCK_SIZE*_py(d.y), 0, 0, 0); }
void next_draw(tetromino *t, SDL_Surface *s, int x, int y) { array_draw(t->mask, t->mask_size, t->mask_size, s, x, y, t->x_init_off, t->y_init_clip, 0); }


static inline int min(int a, int b) { if(a>b) return b; else return a; }
void game_next_piece()
{
	curr_piece.t = next_piece;
	curr_piece.x = 3 + next_piece->x_init_off;
	curr_piece.y = -1;
	curr_piece.lock = 0;
	curr_piece.rot = 0;
	if(key_time[TK_A]||key_time[TK_C]) curr_piece.rot = curr_piece.t->frames-1;
	else if(key_time[TK_B]) curr_piece.rot = min(1, curr_piece.t->frames-1);

	int i,j,next;
	for(i=3; i>=0; --i)
	{
		next = genrand_int31()%7;
		for(j=3; j>=0; --j)
			if(history[j] == next)
				j=-1;
		if(j==0) break;
	}
	history[3] = history[2];
	history[2] = history[1];
	history[1] = history[0];
	history[0] = next;

	next_piece = pieces[next];
}

void game_init(SDL_Surface *screen)
{
	init_genrand(time(0));
	f = new_field(10, 20);
	rs = get_ars_rotation_system();
	memset(key_time, 0, sizeof(int[8]));

	if(f == NULL)
		exit(-1);

	block_color[0x00] = SDL_MapRGB(screen->format, 0x00, 0x00, 0x00);
	block_color[0x01] = SDL_MapRGB(screen->format, 0xFF, 0x00, 0x00);
	block_color[0x02] = SDL_MapRGB(screen->format, 0x00, 0xFF, 0xFF);
	block_color[0x03] = SDL_MapRGB(screen->format, 0xFF, 0x80, 0x00);
	block_color[0x04] = SDL_MapRGB(screen->format, 0x00, 0x00, 0xFF);
	block_color[0x05] = SDL_MapRGB(screen->format, 0xFF, 0x00, 0xFF);
	block_color[0x06] = SDL_MapRGB(screen->format, 0x00, 0xFF, 0x00);
	block_color[0x07] = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0x00);
	block_color[0xFE] = SDL_MapRGB(screen->format, 0xFF, 0xFF, 0xFF);
	block_color[0xFF] = SDL_MapRGB(screen->format, 0x40, 0x40, 0x40);
	
	pieces[0] = &rs->I;
	pieces[1] = &rs->T;
	pieces[2] = &rs->L;
	pieces[3] = &rs->J;
	pieces[4] = &rs->S;
	pieces[5] = &rs->Z;
	pieces[6] = &rs->O;
	
	history[0] = 4;
	history[1] = 5;
	history[2] = 4;
	history[3] = 5;

	next_piece = pieces[genrand_int31()%4];
	game_next_piece();
}

void game_frame(SDL_Surface *screen)
{
	game_handle_keys();
	switch(game_state)
	{
	case TS_FALLING:
		curr_piece.y += 4;
		if(collision_check(curr_piece.t, f, curr_piece.x, _py(curr_piece.y), curr_piece.rot))
		{
			curr_piece.y = _bx(_py(curr_piece.y)-1);
			++curr_piece.lock;
		}
		if(game_get_key_DAS(TK_DOWN)) curr_piece.y += 0xFF;
		if(collision_check(curr_piece.t, f, curr_piece.x, _py(curr_piece.y), curr_piece.rot))
		{
			curr_piece.y = _bx(_py(curr_piece.y)-1);
			curr_piece.lock = T_LOCKDELAY+1;
		}
		if(game_get_key(TK_UP))
		{
			while(!collision_check(curr_piece.t, f, curr_piece.x, _py(curr_piece.y), curr_piece.rot))
				curr_piece.y += 0xFF;
			curr_piece.y -= 0xFF;
		}	
		if(game_get_key_DAS(TK_RIGHT))
		{
			curr_piece.x += 1;
			if(collision_check(curr_piece.t, f, curr_piece.x, _py(curr_piece.y), curr_piece.rot))
				curr_piece.x -= 1;
		}
		if(game_get_key_DAS(TK_LEFT))
		{
			curr_piece.x -= 1;
			if(collision_check(curr_piece.t, f, curr_piece.x, _py(curr_piece.y), curr_piece.rot))
				curr_piece.x += 1;
		}
		int init_rot = curr_piece.rot;
		if(game_get_key(TK_A)||game_get_key(TK_B)||game_get_key(TK_C))
		{
			if(game_get_key(TK_A)||game_get_key(TK_C))
			{
				curr_piece.rot -= 1;
				if(curr_piece.rot < 0) curr_piece.rot = curr_piece.t->frames-1;
			}
			else if(game_get_key(TK_B))
			{
				curr_piece.rot += 1;
				if(curr_piece.rot >= curr_piece.t->frames) curr_piece.rot = 0;
			}

			if(collision_check(curr_piece.t, f, curr_piece.x, _py(curr_piece.y), curr_piece.rot))
			{
				curr_piece.x -= 1;
				if(collision_check(curr_piece.t, f, curr_piece.x, _py(curr_piece.y), curr_piece.rot))
				{
					curr_piece.x += 2;
					if(collision_check(curr_piece.t, f, curr_piece.x, _py(curr_piece.y), curr_piece.rot))
					{
						curr_piece.x -= 1;
						curr_piece.rot = init_rot;
					}
				}
			}
		}
		if(curr_piece.lock >= T_LOCKDELAY)
		{
			game_state = TS_LINECLEAR;
			clear_time = T_CLEARDELAY;
			lock_piece();
		}
		break;
	case TS_LINECLEAR:
		if(clear_time == T_CLEARDELAY)
		{
			int x,y,numclear=0,empty;
			for(y=f->h-1; y>=0; --y)
			{
				empty = 0;
				for(x=f->w-1; x>=0; --x)
				{
					if(f->field[y*f->w + x] == 0)
					{
						empty = 1;
						break;
					}
				}
				if(empty == 0)
				{
					for(x=f->w-1; x>=0; --x)
						f->field[y*f->w + x] = 0xFE;
					++numclear;
				}
			}
			if(numclear == 0)
			{
				game_state = TS_ENTRY;
				break;
			}
		}
		--clear_time;
		if(clear_time == 0)	
		{
			int y,cy,cx;
			for(y=0; y<f->h; ++y)
			{
				if((unsigned char)f->field[y*f->w+0] == 0xFE)
				{
					printf("clearing line %i\n", y);
					for(cy=y; cy>0; --cy)
						for(cx=f->w-1; cx>=0; --cx)
							f->field[cy*f->w+cx] = f->field[(cy-1)*f->w+cx];
				}
			}
			game_state = TS_ENTRY;
		}
		break;
	case TS_ENTRY:
		--are_time;
		if(are_time == 0)
		{
			are_time = T_ENTRYDELAY;
			game_next_piece();
			game_state = TS_FALLING;
		}
		break;
	}

	SDL_Rect rect = { (SCREEN_WIDTH) / 2 - (10 * BLOCK_SIZE) / 2, (SCREEN_HEIGHT) - (20 * BLOCK_SIZE) - 20, BLOCK_SIZE * 10 + 1, BLOCK_SIZE * 20 + 1};
	SDL_Rect next_rect = { (rect.x) + ((BLOCK_SIZE * 10/2) - (BLOCK_SIZE * 4/2)), rect.y - (4 * BLOCK_SIZE) - 10, 4 * BLOCK_SIZE+1, 4 * BLOCK_SIZE+1};
	SDL_FillRect(screen, &rect, block_color[0xFF]);
	field_draw(f, screen, rect.x, rect.y);
	next_draw(next_piece, screen, next_rect.x, next_rect.y + (4-next_piece->mask_size)*BLOCK_SIZE);
	if(game_state == TS_FALLING)
		drop_draw(curr_piece, screen, rect.x, rect.y);
}

void game_handle_keys()
{
	Uint8 *keystate = SDL_GetKeyState(NULL);
	if(keystate['w']) ++key_time[TK_UP   ]; else key_time[TK_UP   ] = 0;
	if(keystate['s']) ++key_time[TK_DOWN ]; else key_time[TK_DOWN ] = 0;
	if(keystate['d']&&!key_time[TK_LEFT]) ++key_time[TK_RIGHT]; else key_time[TK_RIGHT] = 0;
	if(keystate['a']&&!key_time[TK_RIGHT]) ++key_time[TK_LEFT]; else key_time[TK_LEFT ] = 0;
	if(keystate['j']) ++key_time[TK_A    ]; else key_time[TK_A    ] = 0;
	if(keystate['k']) ++key_time[TK_B    ]; else key_time[TK_B    ] = 0;
	if(keystate['l']) ++key_time[TK_C    ]; else key_time[TK_C    ] = 0;
	if(keystate[SDLK_SPACE]) ++key_time[TK_HOLD ]; else key_time[TK_HOLD ] = 0;
}

int game_get_key_DAS(int key)
{
	return key_time[key]==1 || key_time[key]>T_DASDELAY+1;
}
int game_get_key(int key)
{
	return key_time[key]==1;
}


