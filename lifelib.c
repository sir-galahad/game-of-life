#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"lifelib.h"



int *gol_cells_needed_to_live;
int *gol_cells_needed_to_spawn;

cellgrid *create_cellgrid(int max_x, int max_y)
{
	int x,columns,lines,gridsize;
	cellgrid *newgrid;
	newgrid=malloc(sizeof(cellgrid));
	newgrid->max_x=max_x;
	newgrid->max_y=max_y;
	newgrid->iteration=0;
	newgrid->live_cells=0;
	newgrid->prev=NULL;

	//build grid for cells
	columns=max_x+1;
	lines=(max_y/8)+1;
	gridsize=columns*lines;
	newgrid->grid=malloc(sizeof(char *)*columns);
	newgrid->grid[0]=malloc(gridsize);
	memset(newgrid->grid[0],0,gridsize);
	for(x=1;x<=max_x;x++){
		newgrid->grid[x]=newgrid->grid[x-1]+lines;
	}
	
	return newgrid;
}//end create_cellgrid
void free_cellgrid(cellgrid *oldgrid)
{
	int x;
	free(oldgrid->grid[0]);
	free(oldgrid->grid);	
	free(oldgrid);
}//end free_cellgrid

void set_cell(cellgrid *cgrid,int xloc,int yloc,int value)
{
	static char ybitflag[]={128,64,32,16,8,4,2,1};
	char valuetoset;
	int ybyte,ybit;
	if(get_cell(cgrid,xloc,yloc)==value) 
		return;
	//set up locations to wrap around if off the grid
	if(xloc>cgrid->max_x){
		xloc=xloc-(cgrid->max_x+1);
	}
	if(yloc>cgrid->max_y){
		yloc=yloc-(cgrid->max_y+1);
	}
	if(xloc<0){
		xloc+=(cgrid->max_x+1);
	}
	if(yloc<0){
		yloc+=(cgrid->max_y+1);
	}
	ybyte=yloc/8;
	ybit=yloc%8;
	//printf("byte:%d bit:%d\n",ybyte,ybit);

	
	if(value==TRUE){
		valuetoset=ybitflag[ybit];
		cgrid->live_cells++;
	}
	else{
		valuetoset=0;
		cgrid->live_cells--;
	}
	cgrid->grid[xloc][ybyte]|=valuetoset;
}//set_cell

int get_cell(cellgrid *cgrid,int xloc,int yloc)
{
	static char ybitflag[]={128,64,32,16,8,4,2,1};
	char flag;
	unsigned char value;
	int ybyte,ybit;
	
	//set up locations to wrap around if off the grid
	if(xloc>cgrid->max_x){
		xloc=xloc-(cgrid->max_x+1);
	}
	if(yloc>cgrid->max_y){
		yloc=yloc-(cgrid->max_y+1);
	}
	if(xloc<0){
		xloc+=(cgrid->max_x+1);
	}
	if(yloc<0){
		yloc+=(cgrid->max_y+1);
	}
	ybyte=yloc/8;
	ybit=yloc%8;
	flag=ybitflag[ybit];
	value=cgrid->grid[xloc][ybyte]&flag;
	if(value>0){
		return TRUE;
	}
	else{
		return FALSE;
	}
}//end get_cell

int count_live_cell_neighbors(cellgrid *cgrid, int xloc, int yloc)
{
	int x,y;
	int count=0;
	for(x=xloc-1;x<=xloc+1;x++){
		for(y=yloc-1;y<=yloc+1;y++){
			if(x==xloc&&y==yloc) continue;
			
			if(get_cell(cgrid,x,y)==TRUE){
				count++;
			}
		}//end for (y increment)
	}//end for (x increment)
	return count;
}//end count_live_cell_neighbors

void gol_set_rules(int *cellsneededtolive, int *cellsneededtospawn)
{
	gol_cells_needed_to_live=cellsneededtolive;
	gol_cells_needed_to_spawn=cellsneededtospawn;
}
void gol_set_default_rules()
{
	int live[]={2,3,-1};
	int spawn[]={3,-1};
	int *plive, *pspawn;
	plive=malloc(sizeof(int)*3);
	pspawn=malloc(sizeof(int)*2);
	memcpy(plive,live,sizeof(int)*3);
	memcpy(pspawn,spawn,sizeof(int)*2);
	gol_set_rules(plive,pspawn);
}
void gol_do_iteration(cellgrid **cgrid)
{
	int x,y,i;
	int max_x,max_y;
	int neighbors;
	int value;
	cellgrid *oldgrid;
	cellgrid *newgrid;
	max_x=(*cgrid)->max_x;
	max_y=(*cgrid)->max_y;
	oldgrid=(*cgrid);
	newgrid=create_cellgrid(max_x,max_y);

	for(x=0;x<=max_x;x++)
	{
		for(y=0;y<=max_y;y++)
		{
			neighbors=count_live_cell_neighbors(oldgrid,x,y);
			value=get_cell(oldgrid,x,y);
			if(value==TRUE){
			for(i=0;gol_cells_needed_to_live[i]!=-1;i++)
			{
				if(neighbors==gol_cells_needed_to_live[i]){
					set_cell(newgrid,x,y,TRUE);
					break;
				}	
			}}//end if
			else{
			for(i=0;gol_cells_needed_to_spawn[i]>0;i++){
				if(neighbors==gol_cells_needed_to_spawn[i]){
					//printf("spawning %d,%d : %d\n",x,y,neighbors);
					set_cell(newgrid,x,y,TRUE);
					break;
				}
			}//end for
			}//end else
		}//end for (y increment)
	}//end for (x increment)
	newgrid->iteration=++(oldgrid->iteration);
	(*cgrid)=newgrid;
	newgrid->prev=oldgrid;
	
	//delete 5th previous iteration
	x=0;
	while(newgrid->prev!=NULL)
	{
		x++;
		if(x==5){
			free_cellgrid(newgrid->prev);
			newgrid->prev=NULL;
			break;
		}
		newgrid=newgrid->prev;
	}//end while
	
}

void print_cellgrid(cellgrid *cgrid)
{
	int x,y;
	static char dispchars[]={'*','O'};
	for(y=0;y<=cgrid->max_y;y++){
		for(x=0;x<=cgrid->max_x;x++){
			putchar(dispchars[get_cell(cgrid,x,y)]);
		}
		putchar('|');
		putchar('\n');
	}
}

void generate_random_cells(cellgrid *cgrid)
{
int x,y;
srand(time(NULL));
for(x=0;x<=cgrid->max_x;x++){
	for(y=0;y<=cgrid->max_y;y++){
		if(rand()<RAND_MAX/3){
			set_cell(cgrid,x,y,TRUE);
		}
	}
}
}
int gol_is_repeating(cellgrid *cgrid)
{
cellgrid *tmpgrid;
int lines,columns,gridsize;

columns=cgrid->max_x+1;
lines=(cgrid->max_y/8)+1;
gridsize=columns*lines;
tmpgrid=cgrid->prev;

for(;tmpgrid!=NULL;tmpgrid=tmpgrid->prev){
	if(memcmp(cgrid->grid[0],tmpgrid->grid[0],gridsize)==0){
		return TRUE;
	}
}
return FALSE;
}//end gol_is_repeating
