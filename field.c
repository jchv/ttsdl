#include "field.h"
#include <stdlib.h>
#include <string.h>

field *new_field(unsigned char w, unsigned char h)
{
	field *f = malloc(sizeof(field));
	if(f == NULL)
		return NULL;

	f->w = w;
	f->h = h;

	f->field = malloc(w*h);
	if(f->field == NULL)
	{
		free(f);
		return NULL;
	}

	memset(f->field, 0, w*h);

	return f;
}

void free_field(field *f)
{
	if(f != NULL)
	{
		if(f->field != NULL)
			free(f->field);
		free(f);
	}
}

char field_get(field *f, int x, int y)
{
	if(x < 0) return 0xFF;
	if(y < 0) return 0;
	if(x >= f->w) return 0xFF;
	if(y >= f->h) return 0xFF;
	return f->field[y*f->w+x];
}

