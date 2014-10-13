#ifndef LIFE_LIB
#define LIFE_LIB
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define TRUE 1
#define FALSE 0
typedef struct _cellgrid{
	unsigned char **grid;
	int max_x;
	int max_y;
	unsigned long iteration;
	int live_cells;
	struct _cellgrid *prev;
}cellgrid;

extern int *gol_cells_needed_to_live;
extern int *gol_cells_needed_to_spawn;

cellgrid *create_cellgrid(int max_x, int max_y);
void free_cellgrid(cellgrid *oldgrid);
void set_cell(cellgrid *cgrid,int xloc,int yloc,int value);
int get_cell(cellgrid *cgrid,int xloc,int yloc);
int count_live_cell_neighbors(cellgrid *cgrid, int xloc, int yloc);
void gol_set_rules(int *cellsneededtolive, int *cellsneededtospawn);
void gol_set_default_rules();
void gol_do_iteration(cellgrid **cgrid);
void print_cellgrid(cellgrid *cgrid);
void generate_random_cells(cellgrid *cgrid);
int gol_is_repeating(cellgrid *cgrid);
#endif //define LIFE_LIB
