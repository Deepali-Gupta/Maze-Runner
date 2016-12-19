#define GLEW_STATIC 
#include <GL/glew.h>
#include <GL/glu.h> 
#include <GL/glut.h>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include <ctime>
#include <iostream>
#include <fstream>

const GLint XSIZE=16;
const GLint YSIZE=9;
const GLint DIFFICULTY=2; //DIFFICULTY - higher values -> more turns in correct path

//How many times to attempt to add a "false path" coordinate 
const GLint OBFUSCATION_LOOP_RUNS=(XSIZE * YSIZE * 20);

//Logic constants for map contents
const GLint SOLUTION_PATH=2;
const GLint FALSE_PATH=1;
const GLint NO_PATH=0;

const GLint EAST=0;
const GLint SOUTH=1;
const GLint WEST=2;
const GLint NORTH=3;




GLint ((*(maze_innards()))[YSIZE])  //Returns a pointer to the variable portion of the 
{                                   // maze, in 2D format.

 //Just a simple way to surround maze w/ walls... see below...
 static int whole_maze[XSIZE+2][YSIZE+2]={NO_PATH};

 // "Innards" exclude the outer wall... it's worth noting that the array mapping
 //   function inherent to the initialization just above is completely 
 //   abandoned by the cast operation below. However, the overall purpose of this "for"
 //   block (i.e. to surround maze_innards with walls, which is assumed by the
 //   solve/obfuscate algorithm) is preserved. 
 return (int(*)[YSIZE])(&whole_maze[0][1]);
}


void make_solution2() //We don't use that extra +1 in the Y-dimension
{                                     
 //After each turn in the correct path, this variable
 // will be set to the length of the "leg" i.e. the
 // number of squares that the path continues before 
 // the next turn. 3 is a suitable initial value for
 // any maze.
 int path_leg_length=3;

 //Temporary position and direction variables
 int x=XSIZE/2,y=YSIZE/2;
 int d=EAST;

 bool facing_east_west=true;

 //Build correct path column by column
 while(x<XSIZE)
 {
  //March forward designated leg length in current direction
  while(path_leg_length-- && x<(XSIZE))
  {
   switch(d)
   {
   case EAST:
    {
    (maze_innards())[x++][y]=SOLUTION_PATH;
    }    
	break;

   case SOUTH:
  {
    (maze_innards())[x][y++]=SOLUTION_PATH;
    } 
    break;

   case WEST:
    {
    (maze_innards())[x--][y]=SOLUTION_PATH;
    } 
    break;

   case NORTH:
  {
    (maze_innards())[x][y--]=SOLUTION_PATH;
    } 
    break;
   }
  }

  int tempx,tempy;

  do
  {
   tempx=x;
   tempy=y;
   if(facing_east_west) //Time for a turn... e/w -> n/s ; n/s -> east
   {
    d=(rand()%2)?NORTH:SOUTH;
   }else{
    d=EAST;
   }

   //Close to end; just complete the path
   if(XSIZE-x<3)
   {
  
    {
    d=EAST;
    path_leg_length=XSIZE-x;
    }
   }

/*Don't require travel to West... a compromise

that simplifies things. In practice, the user
in the heat of battle will perceive a series of
left and right turns w/o noticing absolute 
direction. Also, the user will inevitably 
deviate from the correct path, possibly requiring

travel to the west. */

   //Set path_leg_length... must be at least 3 or we will make
   // "rooms," which would defy the design criteria stating that
   // there ought only to be one correct path through the maze.
   if(facing_east_west)
   {
    path_leg_length=((rand()%((XSIZE)/DIFFICULTY)+2));
   }else{
    path_leg_length=((rand()%((YSIZE)/DIFFICULTY)+2));
   }

   switch(d)
   {
   case EAST:
    tempx+=path_leg_length;
    break; 
   case SOUTH:
    tempy+=path_leg_length;
    break; 
   case WEST:
    tempx-=path_leg_length;
    break; 
   case NORTH:
    tempy-=path_leg_length;
    break;
   }
   //check here for overflow condition
  }while(tempx<0||tempy<0||tempy>=YSIZE);

  // Decided on a new direction... if prior direction
  // was vertical, new direction is horizontal and vice-versa
  facing_east_west=!facing_east_west;
 }
}

void make_solution3()
{                                     
 //After each turn in the correct path, this variable
 // will be set to the length of the "leg" i.e. the
 // number of squares that the path continues before 
 // the next turn. 3 is a suitable initial value for
 // any maze.
 int path_leg_length=3;

 //Temporary position and direction variables
 int x=XSIZE/2,y=YSIZE/2;
 int d=WEST;

 bool facing_east_west=true;

 //Build correct path column by column
 while(x>=0)
 {
  //March forward designated leg length in current direction
  while(path_leg_length-- && x>=(0))
  {
   switch(d)
   {
   case EAST:
    {
    (maze_innards())[x++][y]=SOLUTION_PATH;
    }    
	break;

   case SOUTH:
  {
    (maze_innards())[x][y++]=SOLUTION_PATH;
    } 
    break;

   case WEST:
    {
    (maze_innards())[x--][y]=SOLUTION_PATH;
    } 
    break;

   case NORTH:
  {
    (maze_innards())[x][y--]=SOLUTION_PATH;
    } 
    break;
   }
  }

  int tempx,tempy;

  do
  {
   tempx=x;
   tempy=y;
   if(facing_east_west) //Time for a turn... e/w -> n/s ; n/s -> east
   {
    d=(rand()%2)?NORTH:SOUTH;
   }else{
    d=WEST;
   }

   //Close to end; just complete the path
   if(x<3)
   {
 
    {
    d=WEST;
    path_leg_length=x;
    }
   }

/*Don't require travel to West... a compromise
that simplifies things. In practice, the user
in the heat of battle will perceive a series of
left and right turns w/o noticing absolute 
direction. Also, the user will inevitably 
deviate from the correct path, possibly requiring
travel to the west. */

   //Set path_leg_length... must be at least 3 or we will make
   // "rooms," which would defy the design criteria stating that
   // there ought only to be one correct path through the maze.
   if(facing_east_west)
   {
    path_leg_length=((rand()%((XSIZE)/DIFFICULTY)+2));
   }else{
    path_leg_length=((rand()%((YSIZE)/DIFFICULTY)+2));
   }

   switch(d)
   {
   case EAST:
    tempx+=path_leg_length;
    break; 
   case SOUTH:
    tempy+=path_leg_length;
    break; 
   case WEST:
    tempx-=path_leg_length;
    break; 
   case NORTH:
    tempy-=path_leg_length;
    break;
   }
   //check here for overflow condition
  }while(tempx>=XSIZE||tempy<0||tempy>=YSIZE);

  // Decided on a new direction... if prior direction
  // was vertical, new direction is horizontal and vice-versa
  facing_east_west=!facing_east_west;
 }
}

bool valid_for_obfuscation(int x, int y)
{
 //Check ranges
 if(x<=0) return false;
 if(y<0) return false;
 if(x>=XSIZE-1) return false;
 if(y>=YSIZE) return false;

 //Already part of the open paths
 if((maze_innards())[x][y]) return false;

 //It's a valid point of obfuscation iff exactly one of the adjacent
 // points is open. If 2+ such points are open, then we're potentially
 // creating a "shortcut," which would result in a 2nd path between
 // start and finish. If 0 such points are open, we're potentially
 // creating an orphan.
 int ret=0;

 if((maze_innards())[x+1][y]) ++ret;
 if(x-1>=0 && (maze_innards())[x-1][y]) ++ret;
 if(y+1<YSIZE && (maze_innards())[x][y+1]) ++ret;
 if(y-1>=0 && (maze_innards())[x][y-1]) ++ret;

 if (ret==1)return true;
 else return false;
}

void obfuscate_maze()
{
 int x,y;
 int c=0;

 for(int ob=0; ob < OBFUSCATION_LOOP_RUNS; ++ob)
 {
  x=rand()%XSIZE;
  y=rand()%YSIZE;

  if(valid_for_obfuscation(x,y))
  {
   c++;
   (maze_innards())[x][y]=FALSE_PATH;
  }
 }
}

void write_maze()
{
	std::ofstream file;
	file.open("file.txt");
	int c;
	for(int y=0; y<YSIZE; y++)
	{
		for(int x=0; x<XSIZE; x++)
		{
			c=(maze_innards())[x][y];
			file<<c<<"\t";
		}
		file<<"\n";
	}
	file.close();
}

int main(int argc, char **argv) 
{ 
srand(time(0));

 make_solution2();
 make_solution3();
 obfuscate_maze();
 write_maze();
  return 0;
}
