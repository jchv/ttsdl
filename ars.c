#include "ars.h"

static char ars_i[2][4][4]=
{
	{{0,0,0,0},
	 {1,1,1,1},
	 {0,0,0,0},
	 {0,0,0,0}},
	{{0,0,1,0},
	 {0,0,1,0},
	 {0,0,1,0},
	 {0,0,1,0}},
};
static char ars_t[4][3][3]=
{
	{{0,0,0},
	 {2,2,2},
	 {0,2,0}},
	{{0,2,0},
	 {2,2,0},
	 {0,2,0}},
	{{0,0,0},
	 {0,2,0},
	 {2,2,2}},
	{{0,2,0},
	 {0,2,2},
	 {0,2,0}},
};
static char ars_l[4][3][3]=
{
	{{0,0,0},
	 {3,3,3},
	 {3,0,0}},
	{{3,3,0},
	 {0,3,0},
	 {0,3,0}},
	{{0,0,0},
	 {0,0,3},
	 {3,3,3}},
	{{0,3,0},
	 {0,3,0},
	 {0,3,3}},
};
static char ars_j[4][3][3]=
{
	{{0,0,0},
	 {4,4,4},
	 {0,0,4}},
	{{0,4,0},
	 {0,4,0},
	 {4,4,0}},
	{{0,0,0},
	 {4,0,0},
	 {4,4,4}},
	{{0,4,4},
	 {0,4,0},
	 {0,4,0}},
};
static char ars_s[2][3][3]=
{
	{{0,0,0},
	 {0,5,5},
	 {5,5,0}},
	{{5,0,0},
	 {5,5,0},
	 {0,5,0}},
};
static char ars_z[2][3][3]=
{
	{{0,0,0},
	 {6,6,0},
	 {0,6,6}},
	{{0,0,6},
	 {0,6,6},
	 {0,6,0}},
};
static char ars_o[1][4][4]=
{
	{{0,0,0,0},
	 {0,7,7,0},
	 {0,7,7,0},
	 {0,0,0,0}},
};
int ars_check(int rotation_from, int rotation_to, tetromino *t, field *f, int *x, int *y);
static rotation_system ars =
{
	{(char *)ars_i,2,4,2,0},
	{(char *)ars_t,4,3,0,0},
	{(char *)ars_l,4,3,0,0},
	{(char *)ars_j,4,3,0,0},
	{(char *)ars_s,2,3,0,0},
	{(char *)ars_z,2,3,0,0},
	{(char *)ars_o,1,4,1,0},
	ars_check
};

int ars_check(int rotation_from, int rotation_to, tetromino *t, field *f, int *x, int *y)
{
	return 1;
}

rotation_system *get_ars_rotation_system()
{
	return &ars;
}
