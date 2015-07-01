#ifndef __FIELD_H__
#define __FIELD_H__

typedef struct field
{
	char *field;
	unsigned char w,h;
} field;

field *new_field(unsigned char w, unsigned char h);
void free_field(field *f);
char field_get(field *f, int x, int y);

#endif
