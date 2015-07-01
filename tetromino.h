/* tetromino struct
 * Pointer to mask is an array of a 2 dimensional bitmask of the tetromino
 * shape. each 2d array is one frame of rotation, starting at the initial
 * rotation, rotating with the left trigger (A) from there on.
 *
 * Mask must use square frames, and the size of the square is stored in
 * mask_size.
 *
 * In the next frame, tetromino will be aligned lower-center.
 */

#ifndef __TETROMINO_H__
#define __TETROMINO_H__

#include "field.h"

typedef struct tetromino
{
	char *mask;
	int frames;
	int mask_size;
	int y_init_clip;
	int x_init_off;
} tetromino;

typedef int (*rotation_collision_check)(int rotation_from, int rotation_to, tetromino *t, field *f, int *x, int *y);
typedef struct rotation_system
{
	tetromino I;
	tetromino T;
	tetromino L;
	tetromino J;
	tetromino S;
	tetromino Z;
	tetromino O;

	rotation_collision_check check;
} rotation_system;

static inline int collision_check(tetromino *t, field *f, int x, int y, int rotation)
{
	int cx, cy;
	for(cy=0; cy < t->mask_size; ++cy)
	{
		for(cx=0; cx < t->mask_size; ++cx)
		{
			int mask_val = t->mask[((rotation*t->mask_size)+cy)*t->mask_size+cx];

			// If mask is empty here, ignore
			if(mask_val == 0)
				continue;

			// Past bounds? collision
			if(x+cx >= f->w || y+cy >= f->h || x+cx < 0)
				return 1;

			if(y+cy < 0) continue;

			// Field already have block at position? collision
			if(f->field[(y+cy)*f->w + (x+cx)] != 0)
				return 1;
		}
	}
	return 0;
}

#endif
