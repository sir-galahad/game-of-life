#include<ncurses.h>
#include<stdlib.h>
#include<sys/time.h>
#include"lifelib.h"

//uncomment below to use random characters to define live cells
//#define RANDOMCHAR 
void draw_cells(cellgrid *cgrid);
void microsleep(int micros);
int random_char(void);

char livecellchar='O';

int main(int argc, char**argv)
{
cellgrid *cgrid;
//*initialize program
initscr();
start_color();
cbreak();
noecho();
nonl();
intrflush(stdscr,FALSE);
keypad(stdscr, TRUE);
assume_default_colors(COLOR_GREEN,COLOR_BLACK);
clear();
gol_set_default_rules();
cgrid=create_cellgrid(LINES-2,COLS-1);
//generate_random_cells(cgrid);
generate_pulsar(cgrid);
draw_blinker(cgrid,20,20,TRUE);
while(1)
{
draw_cells(cgrid);
//getch();
gol_do_iteration(&cgrid);

if(cgrid->iteration%5==0){
	if(gol_is_repeating(cgrid)==TRUE){
	sleep(2);
	free_cellgrid(cgrid);
	cgrid=create_cellgrid(LINES-2,COLS-1);

	generate_random_cells(cgrid);	
	//generate_pulsar(cgrid);
	}
}

}
sleep(3);

endwin();
return 0;
}
char status[100];
void draw_cells(cellgrid *cgrid)
{
int x,y;
erase();
//clearok(stdscr,TRUE);
for(y=0;y<=cgrid->max_y;y++){
	for(x=0;x<=cgrid->max_x;x++){
		if(get_cell(cgrid,x,y)==TRUE){
		#ifndef RANDOMCHAR
			mvaddch(x,y,livecellchar);
		#else
			mvaddch(x,y,random_char()|A_BOLD);
		#endif
		}
	}	
}
sprintf(status,"Iteration: %lu\t\tLive cells: %u",cgrid->iteration,cgrid->live_cells);
mvaddstr(LINES-1,0,status);
refresh();
//microsleep(100);
}

void microsleep(int micros)
{
	struct timeval time;
	time.tv_sec=0;
	time.tv_usec=micros;
	select(0,NULL,NULL,NULL,&time);
}

int random_char()
{
	static int firsttime=1;
	if(firsttime==1){srand(time(NULL));
		firsttime=0;}
	return (rand()%106)+21;
}
