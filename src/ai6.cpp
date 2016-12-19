/***************************************************************************************
O
***************************************************************************************/

// Libraries; we are using GLU, GLUT, and GLEW, along with C stdlib.

#define GLEW_STATIC 
#include <GL/glew.h>
//#include <windows.h> //Seems necessary for GLUT
#include <GL/glu.h> 
#include <GL/glut.h>
#include <stdlib.h>
#include <fstream>
#include <cmath>
#include <ctime>
//#include <process.h>
#include <iostream>

//#include "balls.h"


//Constants

const GLint CONTROLLER_PLAY=250;
const GLint WINDOW_STARTX=20;
const GLint WINDOW_STARTY=20;
const GLint ESCAPE=27; /* ASCII code for the escape key. */
const GLint TEXTURE_SIZE=512;
const GLint MAX_APPERROR=64;
const GLint BMP_HEADER_SIZE=54; //Assumed 24-bit depth
const GLint WINDOW_MARGIN=100;
const GLfloat MAZE_EXTREME_LEFT=-5.0f;
const GLfloat MAZE_EXTREME_TOP=-9.0f;
const GLfloat HALF_CUBE=1.25f;
const GLfloat FULL_CUBE=HALF_CUBE+HALF_CUBE;
const GLfloat START_X_AT=-10.0f;
const GLfloat START_Y_AT=0.0f;
const GLfloat START_ROT=270.0f;
const GLfloat START_CAMERA_Y=0.0f;
const GLfloat CAMERA_SINK=0.025f;
const GLfloat VIEW_FIELD=45.0f;
const GLfloat NEAR_Z=0.1f;
const GLfloat FAR_Z=1000.0f;
const GLfloat SKY_DISTANCE=250.0f;
const GLfloat LEFTMOST_CUBE_CENTER=MAZE_EXTREME_LEFT+HALF_CUBE;
const GLfloat COLLIDE_MARGIN=0.15625;  //To keep from looking inside the cubes
const GLfloat ROTATE_MOUSE_SENSE=0.00004f;
const GLfloat ROTATE_KEY_SENSE=0.08f;
const GLfloat WALK_MOUSE_SENSE=0.000125f;
const GLfloat WALK_KEY_SENSE=0.125f;
const GLfloat WALK_MOUSE_REVERSE_SENSE=0.00004f; //Slower when backpedaling
const GLfloat WALK_KEY_REVERSE_SENSE=0.04f;
const GLfloat BOUNCEBACK=5.0f; //1.0f means none (just reverse collision)
const GLfloat SKY_SCALE=6.0f;
const GLfloat HEALTH_SCALE=10000.0f;
const GLfloat HEALTH_DISTANCE=2.0f;

#define TEXTURE_FILE "wall.bmp"
#define SKY_FILE "sky.bmp"
#define COIN_FILE "photo.bmp"
#define HEALTH_FILE "board.bmp"







//Maze compile-time parameters

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



// File-level variables... these are all position-state / input state variables. OpenGL 
//  callbacks with defined signatures must edit these variables, so there's no easy 
//  alternative to giving them file-level scope.
static GLfloat x_at=START_X_AT; 
static GLfloat y_at=START_Y_AT;
static GLfloat rot=START_ROT;
static GLint xin=0,yin=0;



 static GLuint mesh; /*Texture for the cube*/
 static GLuint haze; /*Texture for the sky*/
 static GLuint coin;
 static GLuint healthfile;
 int visited[XSIZE][YSIZE];

float x_at_c = 6.151852;
float y_at_c = 5.851381;
float x_at_min = MAZE_EXTREME_LEFT+4*FULL_CUBE-COLLIDE_MARGIN;
float x_at_max = MAZE_EXTREME_LEFT+FULL_CUBE+4*FULL_CUBE+COLLIDE_MARGIN;
float y_at_min = MAZE_EXTREME_TOP+(4+1)*FULL_CUBE-COLLIDE_MARGIN;
float y_at_max = MAZE_EXTREME_TOP+(4+2)*FULL_CUBE+COLLIDE_MARGIN;

int shoot=0;

const GLint SPACE=32; /* ASCII code for the escape key. */



const int sides = 50; 
	int state=0;
	float pz;
	float vx=0.0, vy=0.0, vz=0.0;
	
	float rad=0.25;	
	float X[sides+1][sides+1],Y[sides+1][sides+1],Z[sides+1][sides+1];

float px 	;
	float py;
float rotation;
int count12=0;
int health=10;
int coins=0;

 
//Initialize the ball variables randomly

	
	






// Functions

GLint windowwidth()
{
 static int ret=0;
 if(!ret)ret=glutGet(GLUT_SCREEN_WIDTH)-WINDOW_MARGIN;
 return ret;
}

GLint windowheight()
{
 static int ret=0;
 if(!ret)ret=glutGet(GLUT_SCREEN_HEIGHT)-WINDOW_MARGIN;
 return ret;
}

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

//App-level "init" function
void initgl(GLint width, GLint height) 
{
 glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
 glClearDepth(1.0);
 glEnable(GL_DEPTH_TEST);
 glEnable(GL_CULL_FACE);
 glFrontFace(GL_CCW);
 glShadeModel(GL_SMOOTH); 
 glMatrixMode(GL_PROJECTION);
 gluPerspective(VIEW_FIELD,(GLfloat)width/(GLfloat)height,NEAR_Z,FAR_Z); 
 glMatrixMode(GL_MODELVIEW);
}

void Fontopen(void *font,const char *text,int x,int y,int z)
{
	//glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glRasterPos3i(x, y,z);
	while( *text != '\0' )
	{
		glutBitmapCharacter( font, *text );
		++text;
	}
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_LIGHTING);
}










/* The function called when our window is resized (not supported with our textur. sys.)*/
void resizer(GLint width, GLint height)
{ 
 if(width!=windowwidth() || height!=windowheight()) exit(0); 
}

void app_assert_success(const char* szz)
{
 if(GLint xerr= glGetError())
 { 
  char szerr[MAX_APPERROR]; 
  sprintf(szerr,"%s , %d",szz,xerr); 
  fprintf(stderr,"%s",szerr); 
  exit(1);
 }
}

//Loads a texture from a text file and returns its integer OpenGL handle
GLuint maketex(const char* tfile,GLint xSize,GLint ySize) //returns tex. no.
{
 GLuint rmesh;
 FILE * file;
 unsigned char * texdata = (unsigned char*) malloc( xSize * ySize * 3 ); //3 is {R,G,B}

 file = fopen(tfile, "rb" );
 fseek(file,BMP_HEADER_SIZE,SEEK_CUR);
 fread( texdata, xSize * ySize * 3, 1, file ); 
 fclose( file );
 glEnable( GL_TEXTURE_2D );

 char* colorbits = new char[ xSize * ySize * 3]; 

 for(GLint a=0; a<xSize * ySize * 3; ++a) colorbits[a]=0xFF; 

 glGenTextures(1,&rmesh);
 glBindTexture(GL_TEXTURE_2D,rmesh);

 glTexImage2D(GL_TEXTURE_2D,0 ,3 , xSize,
 ySize, 0 , GL_RGB, GL_UNSIGNED_BYTE, colorbits);

 app_assert_success("post0_image");

 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER, GL_LINEAR);
 glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER, GL_LINEAR);

 app_assert_success("pre_getview");

 //Save viewport and set up new one
 GLint viewport[4]; //4 is {X,Y,Width,Height}
 glGetIntegerv(GL_VIEWPORT,(GLint*)viewport);

 app_assert_success("pre_view");
 glViewport(0,0,xSize,ySize);
 app_assert_success("post0_view");

 //Clear target and depth buffers
 glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); 

 glPushMatrix(); //Duplicates MODELVIEW stack top
 glLoadIdentity(); //Replace new top with {1}

 app_assert_success("ogl_mvx");

 glDrawPixels(xSize,ySize,GL_BGR, GL_UNSIGNED_BYTE,texdata);

 app_assert_success("pre_copytext");
 glPopMatrix();
 app_assert_success("copytext2");
 glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
 0,0, xSize, ySize, 0);
 app_assert_success("post_copy");
 
 //Restore viewport
 glViewport(viewport[0],viewport[1],viewport[2],viewport[3]); //{X,Y,Width,Height}
 app_assert_success("ogl_mm1");
 delete[] colorbits;
 free(texdata);
 return rmesh;

}

void cube(GLfloat x, GLfloat y, GLfloat z) //Draws a cube centered at (x,y,z)
{
	glBindTexture(GL_TEXTURE_2D,mesh);
	glBegin(GL_QUADS); // Draw the cube (6 faces)
 // Top of cube
 glTexCoord2d(1.0,1.0);
 glVertex3f(x+HALF_CUBE, HALF_CUBE,z-HALF_CUBE); // Top Right Of The Quad (Top)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE, HALF_CUBE,z-HALF_CUBE); // Top Left Of The Quad (Top)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE, HALF_CUBE, z+HALF_CUBE); // Bottom Left Of The Quad (Top)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x+HALF_CUBE, HALF_CUBE, z+HALF_CUBE); // Bottom Right Of The Quad (Top)

 // Bottom
 glTexCoord2d(1.0,1.0);
 glVertex3f(x+HALF_CUBE,-HALF_CUBE,z+HALF_CUBE); // Top Right Of The Quad (Bottom)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE,-HALF_CUBE,z+HALF_CUBE); // Top Left Of The Quad (Bottom)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE,-HALF_CUBE,z-HALF_CUBE); // Bottom Left Of The Quad (Bottom)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x+HALF_CUBE,-HALF_CUBE,z-HALF_CUBE); // Bottom Right Of The Quad (Bottom)

 // Front
 glTexCoord2d(1.0,1.0);
 glVertex3f(x+HALF_CUBE, HALF_CUBE, z+HALF_CUBE); // Top Right Of The Quad (Front)
 glTexCoord2d(0.0,1.0); 
 glVertex3f(x-HALF_CUBE, HALF_CUBE, z+HALF_CUBE); // Top Left Of The Quad (Front)
 glTexCoord2d(0.0,0.0); 
 glVertex3f(x-HALF_CUBE,-HALF_CUBE, z+HALF_CUBE); // Bottom Left Of The Quad (Front)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x+HALF_CUBE,-HALF_CUBE, z+HALF_CUBE); // Bottom Right Of The Quad (Front)

 // Back
 glTexCoord2d(1.0,1.0); 
 glVertex3f(x-HALF_CUBE,HALF_CUBE,z-HALF_CUBE); // Top Right Of The Quad (Back)
 glTexCoord2d(0.0,1.0); 
 glVertex3f(x+HALF_CUBE,HALF_CUBE,z-HALF_CUBE); // Top Left Of The Quad (Back)
 glTexCoord2d(0.0,0.0); 
 glVertex3f(x+HALF_CUBE,-HALF_CUBE,z-HALF_CUBE); // Bottom Left Of The Quad (Back)
 glTexCoord2d(1.0,0.0); 
 glVertex3f(x-HALF_CUBE,-HALF_CUBE,z-HALF_CUBE); // Bottom Right Of The Quad (Back)

 // left of cube
 glTexCoord2d(1.0,1.0);
 glVertex3f(x-HALF_CUBE, HALF_CUBE,z+HALF_CUBE); // Top Right Of The Quad (Left)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE, HALF_CUBE,z-HALF_CUBE); // Top Left Of The Quad (Left)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE,-HALF_CUBE,z-HALF_CUBE); // Bottom Left Of The Quad (Left)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x-HALF_CUBE,-HALF_CUBE,z+HALF_CUBE); // Bottom Right Of The Quad (Left)

 // Right of cube
 glTexCoord2d(1.0,1.0);
 glVertex3f(x+HALF_CUBE, HALF_CUBE,z-HALF_CUBE); // Top Right Of The Quad (Right)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x+HALF_CUBE, HALF_CUBE,z+HALF_CUBE); // Top Left Of The Quad (Right)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x+HALF_CUBE,-HALF_CUBE,z+HALF_CUBE); // Bottom Left Of The Quad (Right)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x+HALF_CUBE,-HALF_CUBE,z-HALF_CUBE); // Bottom Right Of The Quad (Right)
 
 glEnd();
}


void draw_balls2(GLfloat px, GLfloat py, GLfloat pz, GLfloat rotation)
{	
	rad=0.15;
	glBindTexture(GL_TEXTURE_2D,mesh);
	for (int i = 0; i <= sides; i++)
   {
      float angle1 = 2 * i * M_PI / sides;
      for (int j = 0; j <= sides; j++)
      {
         float angle2 = j * M_PI / sides;
         X[i][j] = cos(angle1) * sin(angle2);
         Y[i][j] = sin(angle1) * sin(angle2);
	 Z[i][j] = cos(angle2); 
	}
}

// Draw the surface
   for (int i = 0; i < sides; i++)
   {
      for (int j = 0; j < sides; j++)
      {
		  glBegin(GL_POLYGON);
		  //glColor3ub(r2, g2, b2);
			glNormal3f(X[i][j], Y[i][j], Z[i][j]);
			glVertex3f(px+rad*X[i][j],py+rad*Y[i][j], pz+rad*Z[i][j]);

			glNormal3f(X[i][j + 1], Y[i][j + 1], Z[i][j + 1]);
			glVertex3f(px+rad*X[i][j + 1],py+rad*Y[i][j + 1], pz+rad*Z[i][j + 1]);

			glNormal3f(X[i + 1][j + 1], Y[i + 1][j + 1], Z[i + 1][j + 1]);
			glVertex3f(px+rad*X[i + 1][j + 1],py+rad*Y[i + 1][j + 1], pz+rad*Z[i + 1][j + 1]);

			glNormal3f(X[i + 1][j], Y[i + 1][j], Z[i + 1][j]);
			glVertex3f(px+rad*X[i + 1][j],py+rad*Y[i + 1][j], pz+rad*Z[i + 1][j]);
         glEnd();
      }
   }


	
}




void cube2(GLfloat x, GLfloat y, GLfloat z) //Draws a cube centered at (x,y,z)
{
	glBindTexture(GL_TEXTURE_2D,coin);
	glBegin(GL_QUADS); // Draw the cube (6 faces)
 // Top of cube
 glTexCoord2d(1.0,1.0);
 glVertex3f( x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) + HALF_CUBE/4, HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) - HALF_CUBE/4); // Top Right Of The Quad (Top)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) -HALF_CUBE/4, HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) - HALF_CUBE/4); // Top Left Of The Quad (Top)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) -HALF_CUBE/4, HALF_CUBE, z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) +HALF_CUBE/4); // Bottom Left Of The Quad (Top)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) +HALF_CUBE/4, HALF_CUBE, z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) +HALF_CUBE/4); // Bottom Right Of The Quad (Top)

 // Bottom
 glTexCoord2d(1.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) +HALF_CUBE/4,-HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) +HALF_CUBE/4); // Top Right Of The Quad (Bottom)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) -HALF_CUBE/4,-HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) +HALF_CUBE/4); // Top Left Of The Quad (Bottom)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) -HALF_CUBE/4,-HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) - HALF_CUBE/4); // Bottom Left Of The Quad (Bottom)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) +HALF_CUBE/4,-HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) - HALF_CUBE/4); // Bottom Right Of The Quad (Bottom)

 // Front
 glTexCoord2d(1.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) +HALF_CUBE/4, HALF_CUBE, z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) +HALF_CUBE/4); // Top Right Of The Quad (Front)
 glTexCoord2d(0.0,1.0); 
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) -HALF_CUBE/4, HALF_CUBE, z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) +HALF_CUBE/4); // Top Left Of The Quad (Front)
 glTexCoord2d(0.0,0.0); 
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) -HALF_CUBE/4,-HALF_CUBE, z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) +HALF_CUBE/4); // Bottom Left Of The Quad (Front)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) +HALF_CUBE/4,-HALF_CUBE, z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) +HALF_CUBE/4); // Bottom Right Of The Quad (Front)

 // Back
 glTexCoord2d(1.0,1.0); 
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) -HALF_CUBE/4,HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) - HALF_CUBE/4); // Top Right Of The Quad (Back)
 glTexCoord2d(0.0,1.0); 
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) +HALF_CUBE/4,HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) - HALF_CUBE/4); // Top Left Of The Quad (Back)
 glTexCoord2d(0.0,0.0); 
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) +HALF_CUBE/4,-HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) - HALF_CUBE/4); // Bottom Left Of The Quad (Back)
 glTexCoord2d(1.0,0.0); 
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) -HALF_CUBE/4,-HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) - HALF_CUBE/4); // Bottom Right Of The Quad (Back)

 // left of cube
 glTexCoord2d(1.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) -HALF_CUBE/4, HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) +HALF_CUBE/4); // Top Right Of The Quad (Left)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) -HALF_CUBE/4, HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) - HALF_CUBE/4); // Top Left Of The Quad (Left)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) -HALF_CUBE/4,-HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) - HALF_CUBE/4); // Bottom Left Of The Quad (Left)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) -HALF_CUBE/4,-HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) +HALF_CUBE/4); // Bottom Right Of The Quad (Left)

 // Right of cube
 glTexCoord2d(1.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) +HALF_CUBE/4, HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) - HALF_CUBE/4); // Top Right Of The Quad (Right)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) +HALF_CUBE/4, HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) +HALF_CUBE/4); // Top Left Of The Quad (Right)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) +HALF_CUBE/4,-HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) +HALF_CUBE/4); // Bottom Left Of The Quad (Right)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at_c-x_at_min)/(x_at_max-x_at_min))*(2*HALF_CUBE) +HALF_CUBE/4,-HALF_CUBE,z-HALF_CUBE + ((y_at_c-y_at_min)/(y_at_max-y_at_min))*(2*HALF_CUBE) - HALF_CUBE/4); // Bottom Right Of The Quad (Right)
 
 glEnd();
}


void sky(GLuint haze)
{ //Modeled after cube front
 glBindTexture(GL_TEXTURE_2D,haze);
 glBegin(GL_QUADS); 
 glTexCoord2d(1.0,1.0);
 glVertex3f( (windowwidth()/SKY_SCALE), (windowheight()/SKY_SCALE),-SKY_DISTANCE); 
 glTexCoord2d(0.0,1.0);
 glVertex3f( -(windowwidth()/SKY_SCALE), (windowheight()/SKY_SCALE),-SKY_DISTANCE); 
 glTexCoord2d(0.0,0.0);
 glVertex3f( -(windowwidth()/SKY_SCALE), -(windowheight()/SKY_SCALE),-SKY_DISTANCE); 
 glTexCoord2d(1.0,0.0);
 glVertex3f( (windowwidth()/SKY_SCALE), -(windowheight()/SKY_SCALE),-SKY_DISTANCE); 
 glEnd();
}



void draw_health(GLuint healthfile)
{ //Modeled after cube front
 glBindTexture(GL_TEXTURE_2D,healthfile);
 glBegin(GL_QUADS); 
 glTexCoord2d(1.0,1.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.5, (windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,1.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, (windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,0.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, -(windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(1.0,0.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.5, -(windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glEnd();
}




void cube1(GLfloat x, GLfloat y, GLfloat z) //Draws a cube centered at (x,y,z)
{
	glBindTexture(GL_TEXTURE_2D,coin);
	glBegin(GL_QUADS); 
	
// Top of cube
 glTexCoord2d(1.0,1.0);
 glVertex3f(x+HALF_CUBE/2, -HALF_CUBE/2,z-HALF_CUBE/2); // Top Right Of The Quad (Top)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE/2, -HALF_CUBE/2,z-HALF_CUBE/2); // Top Left Of The Quad (Top)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE/2, -HALF_CUBE/2, z+HALF_CUBE/2); // Bottom Left Of The Quad (Top)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x+HALF_CUBE/2, -HALF_CUBE/2, z+HALF_CUBE/2); // Bottom Right Of The Quad (Top)

 glEnd();
 

	}
void make_solution() //We don't use that extra +1 in the Y-dimension
{                                     
 //After each turn in the correct path, this variable
 // will be set to the length of the "leg" i.e. the
 // number of squares that the path continues before 
 // the next turn. 3 is a suitable initial value for
 // any maze.
 int path_leg_length=3;

 //Temporary position and direction variables
 int x=0,y=0;
 int d=EAST;

 bool facing_east_west=true;

 //Start at a random row at column 0
 y=rand()%YSIZE;

 //Build correct path column by column
 while(x<XSIZE)
 {
  //March forward designated leg length in current direction
  while(path_leg_length-- && x<(XSIZE))
  {
   switch(d)
   {
   case EAST:
    (maze_innards())[x++][y]=SOLUTION_PATH;
    break;

   case SOUTH:
    (maze_innards())[x][y++]=SOLUTION_PATH;
    break;

   case WEST:
    (maze_innards())[x--][y]=SOLUTION_PATH;
    break;

   case NORTH:
    (maze_innards())[x][y--]=SOLUTION_PATH;
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
    d=EAST;
    path_leg_length=XSIZE-x;
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
    path_leg_length=((rand()%(XSIZE/DIFFICULTY)+2));
   }else{
    path_leg_length=((rand()%(YSIZE/DIFFICULTY)+2));
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
   //check here for overflow condition - JBW
  }while(tempx<0||tempy<0||tempy>=YSIZE);

  // Decided on a new direction... if prior direction
  // was vertical, new direction is horizontal and vice-versa
  facing_east_west=!facing_east_west;
 }
}






int v[XSIZE][YSIZE]={0};






int A,B;
void ai()
{
	for(int i=0; i<YSIZE;i++)
	{
		if((maze_innards())[1][i]==SOLUTION_PATH)
		{x_at=-3.0f;
			A=0;B=i;
std::cout<<"\n"<<i<<"\n";
		y_at=MAZE_EXTREME_TOP+(i+1)*FULL_CUBE+HALF_CUBE;
		
std::cout<<y_at;
		}
	}
	while(A<=XSIZE-2 && B<=YSIZE-2) {
	if((maze_innards())[A+1][B]==SOLUTION_PATH&&!v[A+1][B])
  {	//move(WALK_KEY_SENSE);
		for(int k=0;k<60;k++) {
	x_at+=FULL_CUBE/60; std::cout<<"x_at="<<x_at<<"\ty_at="<<y_at<<"\t"<<A<<"\t"<<B<<"\n";
	 std::ofstream file;
 file.open("ai6.txt",std::ios::app);
 file<<x_at<<" "<<y_at<<"\n";
 file.close();
	 }
	v[A][B]=1;
	A++;
  }
  else if((maze_innards())[A][B+1]==SOLUTION_PATH&&!v[A][B+1])
  {
	//move(ROTATE_KEY_SENSE);
	//rot+=90.0f;
	for(int k=0;k<60;k++) {
	y_at+=FULL_CUBE/60;std::cout<<"x_at="<<x_at<<"\ty_at="<<y_at<<"\t"<<A<<"\t"<<B<<"\n"; 
	std::ofstream file;
 file.open("ai6.txt",std::ios::app);
 file<<x_at<<" "<<y_at<<"\n";
 file.close();
	}
	v[A][B]=1;
	B++;
  }
  else if((maze_innards())[A][B-1]==SOLUTION_PATH&&!v[A][B-1])
  {
	//move((-1)*ROTATE_KEY_SENSE);
//	rot-=90.0f;
	for(int k=0;k<60;k++) {
	y_at-=FULL_CUBE/60;std::cout<<"x_at="<<x_at<<"\ty_at="<<y_at<<"\t"<<A<<"\t"<<B<<"\n"; 
	std::ofstream file;
 file.open("ai6.txt",std::ios::app);
 file<<x_at<<" "<<y_at<<"\n";
 file.close();
	 }
	v[A][B]=1;
	B--;
  } }
  //std::cout<<"ljsfjf";
  exit(0);
}


void read_maze()
{
	std::ifstream file3;
	file3.open("file.txt");
	//int maze[XSIZE][YSIZE]={0};
	int c;
	for(int y=0; y<YSIZE; y++)
	{
		for(int x=0; x<XSIZE; x++)
		{
			file3>>c;
			(maze_innards())[x][y]=c;
		}
	}
	file3.close();
}


int main(int argc, char **argv) 
{ 
 

 srand(time(0));

 
  read_maze();
 
  ai();
 
  exit(0);
 
 return 0;
}
