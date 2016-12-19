

// Libraries; we are using GLU, GLUT, and GLEW, along with C stdlib.

#define GLEW_STATIC 
#include <GL/glew.h>
//#include <windows.h> //Seems necessary for GLUT
#include <GL/glu.h> 
#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fstream>
#include <cmath>
#include <ctime>
#include <iostream>
#include "openmaze.h"
#include "udp_client.h"
#include "udp_server.h"
//#include  "wavefrontLoader.h"
//WFObject coin1;

//#include "balls.h"
std::string ipaddress[6] = {"1.0.1.1","1.1.1.1","1.2.1.1","1.3.1.1","1.4.1.1","1.5.1.1"};
std::string playername[6] = {"AI","AI","AI","AI","AI","AI"};

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
 GLfloat START_X_AT=-10.0f;
 GLfloat START_Y_AT=0.0f;
const GLfloat START_ROT=270.0f;
const GLfloat START_CAMERA_Y=5.0f;
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
#define HEALTH_FILE "1.bmp"
#define LIFE_FILE "2.bmp"
#define TREASURE_FILE "treasure.bmp"
#define HINT_FILE "hint.bmp"

;




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
//static GLfloat x_at=START_X_AT; 
//static GLfloat y_at=START_Y_AT;
static GLfloat rot=START_ROT;
static GLint xin=0,yin=0;



 static GLuint mesh; /*Texture for the cube*/
 static GLuint haze; /*Texture for the sky*/
 static GLuint coin;
 static GLuint healthfile;
  static GLuint lifefile;
static GLuint hintfile; static GLuint treasure;
 int visited[XSIZE][YSIZE];
 static GLfloat camera_y=START_CAMERA_Y;

//float x_at_c = 6.151852;
//float y_at_c = 5.851381;
float x_at[6]={-10.0f, -10.0f, 15.0f, 15.0f, 40.0f, 40.0f};
float y_at[6]={0.0f, -12.0f, -12.0f, 24.0f, 0.0f, 12.0f};
int score[6]={0,0,0,0,0,0};
float x_at_min[6];
float x_at_max[6];
float y_at_min[6];
float y_at_max[6];
float rot_player[6];
float is_shoot[6]={0,0,0,0,0,0};
float bullet_px[6];
float bullet_py[6];
int my_id=0;
int win=0;

const GLint SPACE=32; /* ASCII code for the escape key. */



const int sides = 20; 
	int state=0;
	float pz;
	float vx=0.0, vy=0.0, vz=0.0;
	
	float rad=0.25;	
	float X[sides+1][sides+1],Y[sides+1][sides+1],Z[sides+1][sides+1];


float rotation;
int count12=0;

int coins=0;
int health1=5;
int lives=3;

int fhint[XSIZE][YSIZE]={0};
float rotqube=0;
int flag[16][9]={0};

int TOTAL_PLAYERS=1;

int flag2[16][9]={0};
int flag3[XSIZE][YSIZE]={0};
// float xs[6] = {-10.0f, -10.0f, 15.0f, 15.0f, 40.0f, 40.0f};
//float ys[6]= {0.0f, -12.0f, -12.0f, 24.0f, 0.0f, 12.0f};
 
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
	//glDisable(GL_TEXTURE_2D);
	glRasterPos2i(10,10);
	while( *text != '\0' )
	{
		glutBitmapCharacter( font, *text );
		++text;
	}
	//glEnable(GL_TEXTURE_2D);
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






void draw_balls(GLfloat px, GLfloat py, GLfloat pz)
{	
	py = py-HALF_CUBE/2;
	glBindTexture(GL_TEXTURE_2D,coin);
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





void draw_balls2(GLfloat px, GLfloat py, GLfloat pz, GLfloat rotation)
{	
	float rad1=0.05;
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
			glVertex3f(px+rad1*X[i][j],py+rad1*Y[i][j], pz+rad1*Z[i][j]);

			glNormal3f(X[i][j + 1], Y[i][j + 1], Z[i][j + 1]);
			glVertex3f(px+rad1*X[i][j + 1],py+rad1*Y[i][j + 1], pz+rad1*Z[i][j + 1]);

			glNormal3f(X[i + 1][j + 1], Y[i + 1][j + 1], Z[i + 1][j + 1]);
			glVertex3f(px+rad1*X[i + 1][j + 1],py+rad1*Y[i + 1][j + 1], pz+rad1*Z[i + 1][j + 1]);

			glNormal3f(X[i + 1][j], Y[i + 1][j], Z[i + 1][j]);
			glVertex3f(px+rad1*X[i + 1][j],py+rad1*Y[i + 1][j], pz+rad1*Z[i + 1][j]);
         glEnd();
      }
   }


	
}



void cube_t(GLfloat x, GLfloat y, GLfloat z) //Draws a cube centered at (x,y,z)
{
	
	glBindTexture(GL_TEXTURE_2D,treasure);
	glBegin(GL_QUADS); // Draw the cube (6 faces)
	glRotatef(rotqube,0.0f,1.0f,0.0f);	// Rotate The cube around the Y axis
    glRotatef(rotqube,1.0f,1.0f,1.0f);
 // Top of cube
 glTexCoord2d(1.0,1.0);
 glVertex3f(x+HALF_CUBE/2, HALF_CUBE/2,z-HALF_CUBE/2); // Top Right Of The Quad (Top)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE/2, HALF_CUBE/2,z-HALF_CUBE/2); // Top Left Of The Quad (Top)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE/2, HALF_CUBE/2, z+HALF_CUBE/2); // Bottom Left Of The Quad (Top)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x+HALF_CUBE/2, HALF_CUBE/2, z+HALF_CUBE/2); // Bottom Right Of The Quad (Top)

 // Bottom
 glTexCoord2d(1.0,1.0);
 glVertex3f(x+HALF_CUBE/2,-HALF_CUBE/2,z+HALF_CUBE/2); // Top Right Of The Quad (Bottom)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE/2,-HALF_CUBE/2,z+HALF_CUBE/2); // Top Left Of The Quad (Bottom)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE/2,-HALF_CUBE/2,z-HALF_CUBE/2); // Bottom Left Of The Quad (Bottom)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x+HALF_CUBE/2,-HALF_CUBE/2,z-HALF_CUBE/2); // Bottom Right Of The Quad (Bottom)

 // Front
 glTexCoord2d(1.0,1.0);
 glVertex3f(x+HALF_CUBE/2, HALF_CUBE/2, z+HALF_CUBE/2); // Top Right Of The Quad (Front)
 glTexCoord2d(0.0,1.0); 
 glVertex3f(x-HALF_CUBE/2, HALF_CUBE/2, z+HALF_CUBE/2); // Top Left Of The Quad (Front)
 glTexCoord2d(0.0,0.0); 
 glVertex3f(x-HALF_CUBE/2,-HALF_CUBE/2, z+HALF_CUBE/2); // Bottom Left Of The Quad (Front)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x+HALF_CUBE/2,-HALF_CUBE/2, z+HALF_CUBE/2); // Bottom Right Of The Quad (Front)

 // Back
 glTexCoord2d(1.0,1.0); 
 glVertex3f(x-HALF_CUBE/2,HALF_CUBE/2,z-HALF_CUBE/2); // Top Right Of The Quad (Back)
 glTexCoord2d(0.0,1.0); 
 glVertex3f(x+HALF_CUBE/2,HALF_CUBE/2,z-HALF_CUBE/2); // Top Left Of The Quad (Back)
 glTexCoord2d(0.0,0.0); 
 glVertex3f(x+HALF_CUBE/2,-HALF_CUBE/2,z-HALF_CUBE/2); // Bottom Left Of The Quad (Back)
 glTexCoord2d(1.0,0.0); 
 glVertex3f(x-HALF_CUBE/2,-HALF_CUBE/2,z-HALF_CUBE/2); // Bottom Right Of The Quad (Back)

 // left of cube
 glTexCoord2d(1.0,1.0);
 glVertex3f(x-HALF_CUBE/2, HALF_CUBE/2,z+HALF_CUBE/2); // Top Right Of The Quad (Left)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE/2, HALF_CUBE/2,z-HALF_CUBE/2); // Top Left Of The Quad (Left)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE/2,-HALF_CUBE/2,z-HALF_CUBE/2); // Bottom Left Of The Quad (Left)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x-HALF_CUBE/2,-HALF_CUBE/2,z+HALF_CUBE/2); // Bottom Right Of The Quad (Left)

 // Right of cube
 glTexCoord2d(1.0,1.0);
 glVertex3f(x+HALF_CUBE/2, HALF_CUBE/2,z-HALF_CUBE/2); // Top Right Of The Quad (Right)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x+HALF_CUBE/2, HALF_CUBE/2,z+HALF_CUBE/2); // Top Left Of The Quad (Right)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x+HALF_CUBE/2,-HALF_CUBE/2,z+HALF_CUBE/2); // Bottom Left Of The Quad (Right)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x+HALF_CUBE/2,-HALF_CUBE/2,z-HALF_CUBE/2); // Bottom Right Of The Quad (Right)
 rotqube+=0.9;
 glEnd();
}



void cube2(int i,GLfloat x, GLfloat y, GLfloat z) //Draws a cube centered at (x,y,z)
{
	//draw_balls(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE),HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*2*HALF_CUBE);
	glBindTexture(GL_TEXTURE_2D,coin);
	
	glBegin(GL_QUADS); // Draw the cube (6 faces)
 // Top of cube
 glTexCoord2d(1.0,1.0);
 glVertex3f( x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) + HALF_CUBE/4, HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) - HALF_CUBE/4); // Top Right Of The Quad (Top)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) -HALF_CUBE/4, HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) - HALF_CUBE/4); // Top Left Of The Quad (Top)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) -HALF_CUBE/4, HALF_CUBE/4, z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4); // Bottom Left Of The Quad (Top)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4, HALF_CUBE/4, z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4); // Bottom Right Of The Quad (Top)

 // Bottom
 glTexCoord2d(1.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4,-HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4); // Top Right Of The Quad (Bottom)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) -HALF_CUBE/4,-HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4); // Top Left Of The Quad (Bottom)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) -HALF_CUBE/4,-HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) - HALF_CUBE/4); // Bottom Left Of The Quad (Bottom)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4,-HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) - HALF_CUBE/4); // Bottom Right Of The Quad (Bottom)

 // Front
 glTexCoord2d(1.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4, HALF_CUBE/4, z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4); // Top Right Of The Quad (Front)
 glTexCoord2d(0.0,1.0); 
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) -HALF_CUBE/4, HALF_CUBE/4, z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4); // Top Left Of The Quad (Front)
 glTexCoord2d(0.0,0.0); 
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) -HALF_CUBE/4,-HALF_CUBE/4, z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4); // Bottom Left Of The Quad (Front)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4,-HALF_CUBE/4, z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4); // Bottom Right Of The Quad (Front)

 // Back
 glTexCoord2d(1.0,1.0); 
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) -HALF_CUBE/4,HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) - HALF_CUBE/4); // Top Right Of The Quad (Back)
 glTexCoord2d(0.0,1.0); 
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4,HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) - HALF_CUBE/4); // Top Left Of The Quad (Back)
 glTexCoord2d(0.0,0.0); 
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4,-HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) - HALF_CUBE/4); // Bottom Left Of The Quad (Back)
 glTexCoord2d(1.0,0.0); 
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) -HALF_CUBE/4,-HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) - HALF_CUBE/4); // Bottom Right Of The Quad (Back)

 // left of cube
 glTexCoord2d(1.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) -HALF_CUBE/4, HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4); // Top Right Of The Quad (Left)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) -HALF_CUBE/4, HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) - HALF_CUBE/4); // Top Left Of The Quad (Left)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) -HALF_CUBE/4,-HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) - HALF_CUBE/4); // Bottom Left Of The Quad (Left)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) -HALF_CUBE/4,-HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4); // Bottom Right Of The Quad (Left)

 // Right of cube
 glTexCoord2d(1.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4, HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) - HALF_CUBE/4); // Top Right Of The Quad (Right)
 glTexCoord2d(0.0,1.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4, HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4); // Top Left Of The Quad (Right)
 glTexCoord2d(0.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4,-HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4); // Bottom Left Of The Quad (Right)
 glTexCoord2d(1.0,0.0);
 glVertex3f(x-HALF_CUBE + ((x_at[i]-x_at_min[i])/(x_at_max[i]-x_at_min[i]))*(2*HALF_CUBE) +HALF_CUBE/4,-HALF_CUBE/4,z-HALF_CUBE + ((y_at[i]-y_at_min[i])/(y_at_max[i]-y_at_min[i]))*(2*HALF_CUBE) - HALF_CUBE/4); // Bottom Right Of The Quad (Right)
 
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
  if(health1==5)
 {
 glTexCoord2d(1.0,1.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.0, (windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,1.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, (windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,0.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, -(windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(1.0,0.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.0, -(windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 }
 if(health1==4)
 {
  glTexCoord2d(1.0,1.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.1, (windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,1.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, (windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,0.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, -(windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(1.0,0.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.1, -(windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 }
 if(health1==3)
 {
  glTexCoord2d(1.0,1.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.2, (windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,1.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, (windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,0.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, -(windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(1.0,0.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.2, -(windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 }
 if(health1==2)
 {
  glTexCoord2d(1.0,1.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.3, (windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,1.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, (windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,0.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, -(windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(1.0,0.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.3, -(windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 }
 if(health1==1)
 {
  glTexCoord2d(1.0,1.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.4, (windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,1.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, (windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,0.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, -(windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 glTexCoord2d(1.0,0.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.4, -(windowheight()/HEALTH_SCALE)+0.7,-HEALTH_DISTANCE); 
 }
 if(health1<=0)
 {
  //x_at[my_id]=START_X_AT;
  //y_at[my_id]=START_Y_AT;
//health1=5;
   //~ aifile.close();
   //~ aifile.open("ai6.txt");
   //~ aifile.clear();
   //~ aifile.seekg(0,ios::beg);
   health1=5;
  lives--;
  std::cout<<"lives="<<lives<<"\t";
 }
 glEnd();
}

void draw_lives(GLuint lifefile)
{ //Modeled after cube front
 
 glBindTexture(GL_TEXTURE_2D,lifefile);
 glBegin(GL_QUADS); 
  if(lives==3)
 {
 glTexCoord2d(1.0,1.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.0, (windowheight()/HEALTH_SCALE)+0.4,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,1.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, (windowheight()/HEALTH_SCALE)+0.4,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,0.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, -(windowheight()/HEALTH_SCALE)+0.4,-HEALTH_DISTANCE); 
 glTexCoord2d(1.0,0.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.0, -(windowheight()/HEALTH_SCALE)+0.4,-HEALTH_DISTANCE); 
 }
 if(lives==2)
 {
  glTexCoord2d(1.0,1.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.2, (windowheight()/HEALTH_SCALE)+0.4,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,1.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, (windowheight()/HEALTH_SCALE)+0.4,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,0.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, -(windowheight()/HEALTH_SCALE)+0.4,-HEALTH_DISTANCE); 
 glTexCoord2d(1.0,0.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.2, -(windowheight()/HEALTH_SCALE)+0.4,-HEALTH_DISTANCE); 
 }
 if(lives==1)
 {
  glTexCoord2d(1.0,1.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.5, (windowheight()/HEALTH_SCALE)+0.4,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,1.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, (windowheight()/HEALTH_SCALE)+0.4,-HEALTH_DISTANCE); 
 glTexCoord2d(0.0,0.0);
 glVertex3f( -(windowwidth()/HEALTH_SCALE)-1.5, -(windowheight()/HEALTH_SCALE)+0.4,-HEALTH_DISTANCE); 
 glTexCoord2d(1.0,0.0);
 glVertex3f( (windowwidth()/HEALTH_SCALE)-1.5, -(windowheight()/HEALTH_SCALE)+0.4,-HEALTH_DISTANCE); 
 }
 if(lives==0)
 {
  exit(0);
 }
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

void cube_hint(GLfloat x, GLfloat y, GLfloat z) //Draws a cube centered at (x,y,z)
{
	glBindTexture(GL_TEXTURE_2D,hintfile);
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


	
	
void draw_player (int i)
{	int x,y;
	for(y=0; y<YSIZE ; ++y ) 
 {
  for(x=0; x<XSIZE ; ++x )
  {
	  if((maze_innards())[x][y]!=NO_PATH)
		{
    if( x_at[i]>=MAZE_EXTREME_LEFT+x*FULL_CUBE-COLLIDE_MARGIN && 
      x_at[i]<=MAZE_EXTREME_LEFT+FULL_CUBE+x*FULL_CUBE+COLLIDE_MARGIN && 
      y_at[i]>=MAZE_EXTREME_TOP+(y+1)*FULL_CUBE-COLLIDE_MARGIN && 
      y_at[i]<=MAZE_EXTREME_TOP+(y+2)*FULL_CUBE+COLLIDE_MARGIN )   
		{
			x_at_min[i] = MAZE_EXTREME_LEFT+x*FULL_CUBE-COLLIDE_MARGIN;
			x_at_max[i] = MAZE_EXTREME_LEFT+FULL_CUBE+x*FULL_CUBE+COLLIDE_MARGIN;
			y_at_min[i] = MAZE_EXTREME_TOP+(y+1)*FULL_CUBE-COLLIDE_MARGIN;
			y_at_max[i] = MAZE_EXTREME_TOP+(y+2)*FULL_CUBE+COLLIDE_MARGIN;
			cube2(i,LEFTMOST_CUBE_CENTER+((GLfloat)x*FULL_CUBE),
    		0.0,
    		MAZE_EXTREME_TOP+HALF_CUBE+FULL_CUBE+((GLfloat)y*FULL_CUBE));
    	}
	}
}
}
			
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

 //Start at a random row at column 0
 //y=rand()%YSIZE;

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
     //if((maze_innards())[x++][y]==NO_PATH)
    (maze_innards())[x++][y]=SOLUTION_PATH;
    }    
	break;

   case SOUTH:
  {
     //if((maze_innards())[x][y++]==NO_PATH)
    (maze_innards())[x][y++]=SOLUTION_PATH;
    } 
    break;

   case WEST:
    {
    // if((maze_innards())[x--][y]==NO_PATH)
    (maze_innards())[x--][y]=SOLUTION_PATH;
    } 
    break;

   case NORTH:
  {
    // if((maze_innards())[x][y--]==NO_PATH)
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
   /* if(y>YSIZE/2)
    {
	while(y>YSIZE/2)
		{
     if((maze_innards())[x][y-1]==NO_PATH)
    (maze_innards())[x][y--]=SOLUTION_PATH;
    } 
    }
    else if(y<YSIZE/2)
    {
	while(y<YSIZE/2)
		{
     if((maze_innards())[x][y+1]==NO_PATH)
    (maze_innards())[x][y++]=SOLUTION_PATH;
    } 
    }
    else*/
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
   //check here for overflow condition - JBW
  }while(tempx<0||tempy<0||tempy>=YSIZE);

  // Decided on a new direction... if prior direction
  // was vertical, new direction is horizontal and vice-versa
  facing_east_west=!facing_east_west;
 }
}

void make_solution3() //We don't use that extra +1 in the Y-dimension
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

 //Start at a random row at column 0
 //y=rand()%YSIZE;

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
     //if((maze_innards())[x++][y]==NO_PATH)
    (maze_innards())[x++][y]=SOLUTION_PATH;
    }    
	break;

   case SOUTH:
  {
     //if((maze_innards())[x][y++]==NO_PATH)
    (maze_innards())[x][y++]=SOLUTION_PATH;
    } 
    break;

   case WEST:
    {
    // if((maze_innards())[x--][y]==NO_PATH)
    (maze_innards())[x--][y]=SOLUTION_PATH;
    } 
    break;

   case NORTH:
  {
    // if((maze_innards())[x][y--]==NO_PATH)
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
   /* if(y>YSIZE/2)
    {
	while(y>YSIZE/2)
		{
     if((maze_innards())[x][y-1]==NO_PATH)
    (maze_innards())[x][y--]=SOLUTION_PATH;
    } 
    }
    else if(y<YSIZE/2)
    {
	while(y<YSIZE/2)
		{
     if((maze_innards())[x][y+1]==NO_PATH)
    (maze_innards())[x][y++]=SOLUTION_PATH;
    } 
    }
    else*/
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
   //check here for overflow condition - JBW
  }while(tempx>=XSIZE||tempy<0||tempy>=YSIZE);

  // Decided on a new direction... if prior direction
  // was vertical, new direction is horizontal and vice-versa
  facing_east_west=!facing_east_west;
 }
}







//For a given X,Y coord. within a given maze, determines if
// opening up X,Y as a path will obfuscate the maze without
// introducing an additional path b/w start and finish. Also,
// the logic below will prevent "orphaned" openings, i.e.
// isolated paths that don't connect to anything else.
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

void coin_func()
{
	score[my_id]++;
	coins++;
	printf("Coin found\n");
		  send_to_all((float)5,(float)score[my_id], (float)0);

}

void treasure_func()
{
	score[my_id]+=25;
	coins+=25;
	printf("Treasure found\n");
	win=1;
		  send_to_all((float)5,(float)score[my_id], (float)0);

}

void collide1()
{
	int x,y;
	for(y=0; y<YSIZE ; ++y )
 {
  for(x=0; x<XSIZE ; ++x )
  {
   if((maze_innards())[x][y]!=NO_PATH)
   {
    if( x_at[my_id]>=MAZE_EXTREME_LEFT+x*FULL_CUBE-COLLIDE_MARGIN && 
      x_at[my_id]<=MAZE_EXTREME_LEFT+FULL_CUBE+x*FULL_CUBE+COLLIDE_MARGIN && 
      y_at[my_id]>=MAZE_EXTREME_TOP+(y+1)*FULL_CUBE-COLLIDE_MARGIN && 
      y_at[my_id]<=MAZE_EXTREME_TOP+(y+2)*FULL_CUBE+COLLIDE_MARGIN )   
    {
	if (!visited[x][y])
		{
			coin_func();
		printf("coins %d\n",coins);}
	visited[x][y]=1;
	
     
    }
   }
  }
 }

}


bool check_overlap(int x,int y)
{
	if( x_at[my_id]>=MAZE_EXTREME_LEFT+x*FULL_CUBE-COLLIDE_MARGIN && 
      x_at[my_id]<=MAZE_EXTREME_LEFT+FULL_CUBE+x*FULL_CUBE+COLLIDE_MARGIN && 
      y_at[my_id]>=MAZE_EXTREME_TOP+(y+1)*FULL_CUBE-COLLIDE_MARGIN && 
      y_at[my_id]<=MAZE_EXTREME_TOP+(y+2)*FULL_CUBE+COLLIDE_MARGIN ){  

		return true;}
	else return false;
}


bool collide_bullet(int i) //Is player in a state of collision?
{
 int x,y;

 //Walls...
 if(bullet_px[i]>=MAZE_EXTREME_LEFT-COLLIDE_MARGIN && 
   bullet_px[i]<=MAZE_EXTREME_LEFT+XSIZE*FULL_CUBE+COLLIDE_MARGIN)
 {
  if( bullet_py[i]<=(MAZE_EXTREME_TOP+FULL_CUBE)+COLLIDE_MARGIN && 
    bullet_py[i]>=MAZE_EXTREME_TOP-COLLIDE_MARGIN)
  {
   return 1; 
  }

  if(bullet_py[i]<=(MAZE_EXTREME_TOP+FULL_CUBE)+FULL_CUBE+(YSIZE*FULL_CUBE)+COLLIDE_MARGIN && 
     bullet_py[i]>= MAZE_EXTREME_TOP+FULL_CUBE+(YSIZE*FULL_CUBE)-COLLIDE_MARGIN)
  {
   return 1;
  }
 }

 //Maze proper
 for(y=0; y<YSIZE ; ++y )
 {
  for(x=0; x<XSIZE ; ++x )
  {
   if((maze_innards())[x][y]==NO_PATH)
   {
    if( bullet_px[i]>=MAZE_EXTREME_LEFT+x*FULL_CUBE-COLLIDE_MARGIN && 
      bullet_px[i]<=MAZE_EXTREME_LEFT+FULL_CUBE+x*FULL_CUBE+COLLIDE_MARGIN && 
      bullet_py[i]>=MAZE_EXTREME_TOP+(y+1)*FULL_CUBE-COLLIDE_MARGIN && 
      bullet_py[i]<=MAZE_EXTREME_TOP+(y+2)*FULL_CUBE+COLLIDE_MARGIN )   
    {
	//printf("px %f x %d",px,x);
	//printf("y_at %f y %d\n",y_at[my_id],y);
     return 1;
    }

   }
  }
 }

 return 0;
}




void print_maze() //Renders the necessary OpenGL cubes
{
	
 int x,y; 
 for(x=0; x<XSIZE ; ++x ) //Print a border
 {
  cube(MAZE_EXTREME_LEFT+HALF_CUBE+((GLfloat)x*FULL_CUBE),
  0.0,
  MAZE_EXTREME_TOP+HALF_CUBE);

  cube(MAZE_EXTREME_LEFT+HALF_CUBE+((GLfloat)x*FULL_CUBE),
  0.0,
  MAZE_EXTREME_TOP+HALF_CUBE+FULL_CUBE+(YSIZE*(FULL_CUBE)) );
 } 
 for(y=0; y<YSIZE ; ++y ) //Maze proper
 {
  for(x=0; x<XSIZE ; ++x )
  {
	
	  if((maze_innards())[x][y]==NO_PATH)
   {
    cube(LEFTMOST_CUBE_CENTER+((GLfloat)x*FULL_CUBE),
    0.0,
    MAZE_EXTREME_TOP+HALF_CUBE+FULL_CUBE+((GLfloat)y*FULL_CUBE));
     
   }
   else if((maze_innards())[x][y]!=NO_PATH && flag[x][y]==0 && (x!=XSIZE/2 || y!=YSIZE/2)&& fhint[x][y]==0)
   {
	//printf("%d\n",visited[x][y]);
   /* cube1(LEFTMOST_CUBE_CENTER+((GLfloat)x*FULL_CUBE),
    0.0,
    MAZE_EXTREME_TOP+HALF_CUBE+FULL_CUBE+((GLfloat)y*FULL_CUBE));*/
    draw_balls(LEFTMOST_CUBE_CENTER+((GLfloat)x*FULL_CUBE),
    0.0,
    MAZE_EXTREME_TOP+HALF_CUBE+FULL_CUBE+((GLfloat)y*FULL_CUBE));
     
   }
   if((maze_innards())[x][y]!=NO_PATH && visited[x][y]==0 && x==XSIZE/2 && y==YSIZE/2 && fhint[x][y]==0)
   {
	//printf("%d\n",visited[x][y]);
    cube_t(LEFTMOST_CUBE_CENTER+((GLfloat)x*FULL_CUBE),
    0.0,
    MAZE_EXTREME_TOP+HALF_CUBE+FULL_CUBE+((GLfloat)y*FULL_CUBE));
     
   }
   if((maze_innards())[x][y]!=NO_PATH && visited[x][y]==0 && fhint[x][y]==1)
   {
	//printf("%d\n",visited[x][y]);
    cube_hint(LEFTMOST_CUBE_CENTER+((GLfloat)x*FULL_CUBE),
    0.0,
    MAZE_EXTREME_TOP+HALF_CUBE+FULL_CUBE+((GLfloat)y*FULL_CUBE));
     
   }
	if (is_shoot[my_id] and (maze_innards())[x][y]!=NO_PATH and check_overlap(x,y)==true)
{	
		//shoot=0;
	//
	if(collide_bullet(0)==false)
	{printf("%f %f %f\n",bullet_px[my_id],camera_y,bullet_py[my_id]);
	float time=0.005;
	bullet_px[my_id]+=50*cos(rotation)*time;
	bullet_py[my_id]+=50*sin(rotation)*time;
	//usleep(20000);
	count12++;

	//if (shoot==0)
    draw_balls2(bullet_px[my_id], 0.0, bullet_py[my_id],rotation);
	//cube2(0,px,0.0,pz);	
	}
	else is_shoot[my_id]=0;
   	}
 
  }
 }
}


bool collide() //Is player in a state of collision?
{
 int x,y;

 //Walls...
 if(x_at[my_id]>=MAZE_EXTREME_LEFT-COLLIDE_MARGIN && 
   x_at[my_id]<=MAZE_EXTREME_LEFT+XSIZE*FULL_CUBE+COLLIDE_MARGIN)
 {
  if( y_at[my_id]<=(MAZE_EXTREME_TOP+FULL_CUBE)+COLLIDE_MARGIN && 
    y_at[my_id]>=MAZE_EXTREME_TOP-COLLIDE_MARGIN)
  {
	health1--;
	//printf("%d",health1);
   return 1; 
  }

  if(y_at[my_id]<=(MAZE_EXTREME_TOP+FULL_CUBE)+FULL_CUBE+(YSIZE*FULL_CUBE)+COLLIDE_MARGIN && 
     y_at[my_id]>= MAZE_EXTREME_TOP+FULL_CUBE+(YSIZE*FULL_CUBE)-COLLIDE_MARGIN)
  {
	health1--;
	//printf("%d",health1);
   return 1;
  }
 }

 //Maze proper
 for(y=0; y<YSIZE ; ++y )
 {
  for(x=0; x<XSIZE ; ++x )
  {
   if((maze_innards())[x][y]==NO_PATH)
   {
    if( x_at[my_id]>=MAZE_EXTREME_LEFT+x*FULL_CUBE-COLLIDE_MARGIN && 
      x_at[my_id]<=MAZE_EXTREME_LEFT+FULL_CUBE+x*FULL_CUBE+COLLIDE_MARGIN && 
      y_at[my_id]>=MAZE_EXTREME_TOP+(y+1)*FULL_CUBE-COLLIDE_MARGIN && 
      y_at[my_id]<=MAZE_EXTREME_TOP+(y+2)*FULL_CUBE+COLLIDE_MARGIN )   
    {
	health1--;
	//printf("%d",health1);
	//printf("x_at %f x %d",x_at[my_id],x);
	//printf("y_at %f y %d\n",y_at[my_id],y);
     return 1;
    }

   }
  }
 }

 return 0;
}


bool check_coin()
{
	int x,y;
	for(y=0; y<YSIZE ; ++y )
    {
		for(x=0; x<XSIZE ; ++x )
		{
			if((maze_innards())[x][y]!=NO_PATH)
			{
				if( x_at[my_id]>=MAZE_EXTREME_LEFT+x*FULL_CUBE-COLLIDE_MARGIN && 
					x_at[my_id]<=MAZE_EXTREME_LEFT+FULL_CUBE+x*FULL_CUBE+COLLIDE_MARGIN && 
					y_at[my_id]>=MAZE_EXTREME_TOP+(y+1)*FULL_CUBE-COLLIDE_MARGIN && 
					y_at[my_id]<=MAZE_EXTREME_TOP+(y+2)*FULL_CUBE+COLLIDE_MARGIN )   
				{
					visited[x][y]=1;
				}
			}
			if(flag[x][y]==0&&visited[x][y]==1)
			{
				send_to_all((float)3,(float)x, (float)y);
				flag[x][y]=1;
				return true; 
			}    

		}
	}
	return false;
}

bool check_treasure()
{
	int x,y;
	for(y=0; y<YSIZE ; ++y )
	{
		for(x=0; x<XSIZE ; ++x )
		{
			if(x==XSIZE/2 && y==YSIZE/2 && visited[x][y]==1)
			{
				if( x_at[my_id]>=MAZE_EXTREME_LEFT+x*FULL_CUBE-COLLIDE_MARGIN && 
					x_at[my_id]<=MAZE_EXTREME_LEFT+FULL_CUBE+x*FULL_CUBE+COLLIDE_MARGIN && 
					y_at[my_id]>=MAZE_EXTREME_TOP+(y+1)*FULL_CUBE-COLLIDE_MARGIN && 
					y_at[my_id]<=MAZE_EXTREME_TOP+(y+2)*FULL_CUBE+COLLIDE_MARGIN )   
				{
					if(flag2[x][y]==0)
					{
						flag2[x][y]=1;
						send_to_all((float)4,(float)x, (float)y);
						return true;     
					}
				}
			}
		}
	}
	return false;
}



bool check_hint()
{
	int x,y;
	for(y=0; y<YSIZE ; ++y )
	{
		for(x=0; x<XSIZE ; ++x )
		{
			if(fhint[x][y]==1 && visited[x][y]==1)
			{
				if( x_at[my_id]>=MAZE_EXTREME_LEFT+x*FULL_CUBE-COLLIDE_MARGIN && 
					x_at[my_id]<=MAZE_EXTREME_LEFT+FULL_CUBE+x*FULL_CUBE+COLLIDE_MARGIN && 
					y_at[my_id]>=MAZE_EXTREME_TOP+(y+1)*FULL_CUBE-COLLIDE_MARGIN && 
					y_at[my_id]<=MAZE_EXTREME_TOP+(y+2)*FULL_CUBE+COLLIDE_MARGIN )   
				{
					if(flag3[x][y]==0)
					{
						flag3[x][y]=1;
						return true;     
					}
				}
			}
		}
	}
	return false;
}



void hint_func()
{
	printf("Hint found");
	system("g++ -o hint hint.cpp -lglut -lGL -lGLU -lGLEW");
	system("./hint");
}







void move(GLfloat amt) //Move, incorporating collision and bounceback
{
  x_at[my_id]+=cos(rot_player[my_id])*amt;
  y_at[my_id]+=sin(rot_player[my_id])*amt; 
  
	
  if(collide()) //Don't let player walk through walls
  {
   health1--;
   std::cout<<"health1="<<health1<<"\t";
   x_at[my_id]-=BOUNCEBACK*cos(rot_player[my_id])*amt;
   y_at[my_id]-=BOUNCEBACK*sin(rot_player[my_id])*amt;
  } 
  if(collide()) //Bounced into another wall... just reverse original move
  {
   health1--;
   std::cout<<"health1="<<health1<<"\t";
   x_at[my_id]+=BOUNCEBACK*cos(rot_player[my_id])*amt;
   y_at[my_id]+=BOUNCEBACK*sin(rot_player[my_id])*amt;
   x_at[my_id]-=cos(rot_player[my_id])*amt;
   y_at[my_id]-=sin(rot_player[my_id])*amt; 
  } 
  collide1();
  if(check_coin())
  {
	  coin_func();
  }
  if(check_treasure())
  {
	 treasure_func();
  }
  if(check_hint())
  {
	 hint_func();
  }
  

 }

bool is_hit(int i)
{
	int x,y;
			for(y=0; y<YSIZE ; ++y )
			{
			  for(x=0; x<XSIZE ; ++x )
			  {
			   if((maze_innards())[x][y]!=NO_PATH)
			   {
				if( x_at[my_id]>=MAZE_EXTREME_LEFT+x*FULL_CUBE-COLLIDE_MARGIN && 
				  x_at[my_id]<=MAZE_EXTREME_LEFT+FULL_CUBE+x*FULL_CUBE+COLLIDE_MARGIN && 
				  y_at[my_id]>=MAZE_EXTREME_TOP+(y+1)*FULL_CUBE-COLLIDE_MARGIN && 
				  y_at[my_id]<=MAZE_EXTREME_TOP+(y+2)*FULL_CUBE+COLLIDE_MARGIN )   
				{
					if( bullet_px[i]>=MAZE_EXTREME_LEFT+x*FULL_CUBE-COLLIDE_MARGIN && 
				  bullet_px[i]<=MAZE_EXTREME_LEFT+FULL_CUBE+x*FULL_CUBE+COLLIDE_MARGIN && 
				  bullet_py[i]>=MAZE_EXTREME_TOP+(y+1)*FULL_CUBE-COLLIDE_MARGIN && 
				  bullet_py[i]<=MAZE_EXTREME_TOP+(y+2)*FULL_CUBE+COLLIDE_MARGIN ) 
					return true;
				}
			}
		}
	}
	return false;
	
		
}
 std::ifstream aifile;
 std::ifstream aifile2;
 std::ifstream aifile3;



int ctr=0;

void drawscene()
{ 
 
 
 static bool init=0;

 if(!init)
 { 
  init=1;
  mesh=maketex(TEXTURE_FILE,TEXTURE_SIZE,TEXTURE_SIZE);
  haze=maketex(SKY_FILE,TEXTURE_SIZE,TEXTURE_SIZE);
  coin=maketex(COIN_FILE,TEXTURE_SIZE,TEXTURE_SIZE);
  healthfile=maketex(HEALTH_FILE,TEXTURE_SIZE,TEXTURE_SIZE);
   lifefile=maketex(LIFE_FILE,TEXTURE_SIZE,TEXTURE_SIZE);
  hintfile=maketex(HINT_FILE,TEXTURE_SIZE,TEXTURE_SIZE);
  treasure=maketex(TREASURE_FILE,TEXTURE_SIZE,TEXTURE_SIZE);
   }

 if(camera_y<=0.0f && xin && yin)
 { 
  if(yin<CONTROLLER_PLAY) 
   move((yin-windowheight()/2.0f)*-WALK_MOUSE_SENSE); 
  if(yin>(windowheight()-CONTROLLER_PLAY))
   move(((windowheight()/2.0f)-yin)*WALK_MOUSE_REVERSE_SENSE);
  if(xin<CONTROLLER_PLAY || xin>(windowwidth()-CONTROLLER_PLAY))
   rot_player[my_id]+=(xin-(windowwidth()/2.0f))*ROTATE_MOUSE_SENSE;
 }
 
 glLoadIdentity(); // Make sure we're no longer rotated.
 glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // Clear screen and depth buffer
 draw_health(healthfile);
 draw_lives(lifefile);
 sky(haze); //Draw sky
 
 
 gluLookAt(x_at[my_id],camera_y,y_at[my_id],x_at[my_id]+cos(rot_player[my_id]),camera_y,y_at[my_id]+sin(rot_player[my_id]),0.0,1.0,0.0);

 if(camera_y>0.0) camera_y-=CAMERA_SINK;
  
 //std::cout<<"x_at mine "<<x_at[my_id]<<" "<<"x_at other "<<x_at[1]<<" "<<std::endl;

send_to_all(float(1),x_at[my_id],y_at[my_id]); 
 print_maze();
 ctr++;
 if(ctr%10==0)
 {
	if (lives==3)
	 aifile>>x_at[my_id]>>y_at[my_id];
	 else if(lives==2)
	 	 aifile2>>x_at[my_id]>>y_at[my_id];

	 else if(lives==1)
	 	 aifile3>>x_at[my_id]>>y_at[my_id];

	
	
	 //health1=5;
	 //std::cout<<"\n ai_x="<<x_at[my_id]<<" ai_y="<<y_at[my_id];
	 //send_to_all(float(1),x_at[my_id],y_at[my_id]);
}
 for (int i=0;i<6;i++)
 {
	 if (i!=my_id)
	 {
		 draw_player(i);
	 }
 //std::cout<<health1<<"dfasf \n"; 
 for (int i=0;i<6;i++)
 {
		if (i!=my_id)
		{
			if (is_shoot[i]==1)
			{
					float time=0.005;
					bullet_px[i]+=50*cos(rot_player[i])*time;
					bullet_py[i]+=50*sin(rot_player[i])*time;
					draw_balls2(bullet_px[i], 0.0, bullet_py[i],rot_player[i]);
					std::cout<<bullet_px[i]<<" "<<bullet_py[i]<<std::endl;
					std::cout<<is_hit(i)<<" ishit"<<std::endl;
					if (is_hit(i))
						{
							
							health1--;
							std::cout<<health1<<"health1 "<<std::endl;
							bullet_px[i]=x_at[i];
							bullet_py[i]=y_at[i];
							is_shoot[i]=0;
						}
					if (collide_bullet(i))
					{
							bullet_px[i]=x_at[i];
							bullet_py[i]=y_at[i];
							is_shoot[i]=0;
					}
			}
			else
				{
					bullet_px[i]=x_at[i];
					bullet_py[i]=y_at[i];
				}
		}
 }
   if(check_coin())
  {
	  coin_func();
  }
  if(check_treasure())
  {
	 treasure_func();
  }
  /*if(check_hint())
  {
	 hint_func();
  }*/
if(flag2[8][4]==1)
  {
	  //score[my_id]=coins;  
	  send_to_all((float)5,(float)score[my_id], (float)0);
	  //~ std::cout<<"\nGame Over!";
	  //~ std::cout<<"\nYour score="<<score[my_id];
	  //~ for(int i=0;i<6;i++)
	  //~ {
		  //~ std::cout<<"\nPlayer "<<i+1<<" Score:"<<score[i];
	  //~ }
	  //~ if(win)
				//~ std::cout<<"\nYou won!!!";
	  //~ else
				//~ std::cout<<"\nYou lost...";
	  exit(0);
  } 
 
 glutSwapBuffers();
}
}






void arrows(GLint key, GLint x, GLint y) 
{
 if(key == GLUT_KEY_UP) 
  move(WALK_KEY_SENSE);
 if(key == GLUT_KEY_DOWN) 
  move(-WALK_KEY_REVERSE_SENSE);  
 if(key == GLUT_KEY_RIGHT)
  rot_player[my_id]+=ROTATE_KEY_SENSE;
 if(key == GLUT_KEY_LEFT)
  rot_player[my_id]-=ROTATE_KEY_SENSE;
}

void keypress(unsigned char key, GLint x, GLint y) 
{
 if(key==ESCAPE)exit(0); 
 if(key==SPACE)
{
int x1,y1;
for(y1=0; y1<YSIZE ; ++y1 )
 {
  for(x1=0; x1<XSIZE ; ++x1 )
  {
   if((maze_innards())[x1][y1]!=NO_PATH)
   {
    if( x_at[my_id]>=MAZE_EXTREME_LEFT+x1*FULL_CUBE-COLLIDE_MARGIN && 
      x_at[my_id]<=MAZE_EXTREME_LEFT+FULL_CUBE+x1*FULL_CUBE+COLLIDE_MARGIN && 
      y_at[my_id]>=MAZE_EXTREME_TOP+(y1+1)*FULL_CUBE-COLLIDE_MARGIN && 
      y_at[my_id]<=MAZE_EXTREME_TOP+(y1+2)*FULL_CUBE+COLLIDE_MARGIN )   
    {
	//printf("x_at %f x %d",x_at[my_id],x);
	//printf("y_at %f y %d\n",y_at[my_id],y);
	//px_at = x_at;
	//pz_at = z_at;	
	//~ bullet_px[my_id] = LEFTMOST_CUBE_CENTER+((GLfloat)x1*FULL_CUBE);	
	//~ bullet_py[my_id] = MAZE_EXTREME_TOP+HALF_CUBE+FULL_CUBE+((GLfloat)y1*FULL_CUBE);
	bullet_px[my_id]=x_at[my_id];
	bullet_py[my_id]=y_at[my_id];
	 rotation=rot_player[my_id];
	//count12=0;
	is_shoot[my_id]=1;
	//send_to_all(float(2),rot_player[my_id],float(0));
}
}}}}
if(key==9)
{
	//is_shoot[1]=1;
}
}

void mouse(int x, int y) 
{
 static int mouses=0; //Used to wait for a real mouse move vs. initial positioning call
 
 if(mouses<=1)
 { 
  ++mouses;
  xin=0; yin=0;
  return;
 }
 xin=x; yin=y;
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

void read_maze()
{
	std::ifstream file;
	file.open("file.txt");
	//int maze[XSIZE][YSIZE]={0};
	int c;
	for(int y=0; y<YSIZE; y++)
	{
		for(int x=0; x<XSIZE; x++)
		{
			file>>c;
			(maze_innards())[x][y]=c;
		}
	}
	file.close();
}

void decide_hint()
{
	int ctr=0;
	for(int y=0; y<YSIZE; y++)	
	{
		for(int x=0; x<XSIZE; x++)
		{
			if((maze_innards())[x][y]!=0 && ctr<=4)
			{
				int f=rand()%20;
				if(f==1)
				{
					fhint[x][y]=1;
					ctr++;
				}
			}
		}
	}			
}


void read_players(){
		std::ifstream file;
	file.open("players.txt");
	for (int i=0;i<6;i++)
	{
	file>>ipaddress[i];
	//~ ipaddress[i]=c;
	file>>playername[i];
	//~ playername[i]=c;
	}
	file.close();
}


void *myfunc(void *args){
	int i = (long int) args;
	if (i != my_id) {
	//~ if (i == 1) {
	open_write_port(i);
	open_read_port(i);
	//~ std::ofstream file;
	//~ switch(i) {
		//~ case 0 : file.open("1.txt",ios::beg); break;
		//~ case 1 : file.open("2.txt",ios::beg); break;
		//~ case 2 : file.open("3.txt",ios::beg); break;
		//~ case 3 : file.open("4.txt",ios::beg); break;
		//~ case 4 : file.open("5.txt",ios::beg); break;
		//~ case 5 : file.open("6.txt",ios::beg);
		//~ }
	while (true)
	{
		udp_read(i);
		//~ file << udp_read(i);
		//~ file << "\n";
	}
	}
}


int main(int argc, char **argv) 
{ system("g++ -w -o ai4 ai4.cpp -lglut -lGL -lGLU -lGLEW");
 system("./ai4");
	 aifile.open("ai4.txt");
	 	 aifile2.open("ai4.txt");
	 aifile3.open("ai4.txt");

	 //aifile.open("ai.txt");
read_players();
 my_id = 3;
 TOTAL_PLAYERS=atoi(argv[2]);
 printf("My id is %d \n",my_id);
 set_read_ports();
 set_write_ports();
 //read_maze();
 //~ my_id = 1;
 //~ switch (my_id){
		//~ case 0: START_X_AT = -10.0f; break;
		//~ case 1: START_X_AT = -10.0f; START_Y_AT = -12.0f; break;
		//~ case 2: START_X_AT = 15.0f; START_Y_AT = -12.0f; break;
		//~ case 3: START_X_AT = 15.0f; START_Y_AT = 24.0f; break;
		//~ case 4: START_X_AT = 40.0f; break;
		//~ case 5: START_X_AT = 40.0f; START_Y_AT = 12.0f; 
		//~ }
		//x_at[my_id]=START_X_AT;
		//y_at[my_id]=START_Y_AT;
		//x_at=START_X_AT;
		//y_at=START_Y_AT;
 pthread_t t1,t2,t3,t4,t5,t6;
 pthread_create(&t1, NULL, myfunc, (void *)0);
 pthread_create(&t2, NULL, myfunc, (void *)1);
 pthread_create(&t3, NULL, myfunc, (void *)2);
 pthread_create(&t4, NULL, myfunc, (void *)3);
 pthread_create(&t5, NULL, myfunc, (void *)4);
 pthread_create(&t6, NULL, myfunc, (void *)5);
 
read_maze();
 /* The number of our GLUT window */
 GLuint window; 
 
 glutInit(&argc, argv); 
 glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH); 
 glDisable(GLUT_ALPHA);
 glutInitWindowSize(windowwidth(),windowheight()); 
 std::cout<<windowheight()<<std::endl;
 glutInitWindowPosition(WINDOW_STARTX, WINDOW_STARTY); 

 glutCreateWindow("AI4");
for (int i=0;i<XSIZE;i++)
	{for (int j=0;j<YSIZE;j++)
		{visited[i][j]=0;}}

//x_at[1]=START_X_AT+0.5f;
//y_at[1]=START_Y_AT;
//rot_player[1]=START_ROT;
 //~ GLUI *edit = GLUI_Master.create_glui("Help on GLUI Widgets", 0);
  //~ main_window = edit->get_glut_window_id();
  //~ GLUI_Panel *ep = new GLUI_Panel(edit,"",true);
  //~ new GLUI_StaticText(ep,"Widget Information:");
  //~ new GLUI_StaticText(ep,"Open Text File:");
  //~ GLUI_Master.set_glutIdleFunc( idle );
  //~ GLUI_Master.set_glutIdleFunc( NULL );
	//~ //edit->hide();
	//~ glui->set_main_gfx_window( main_window );

 glutDisplayFunc(&drawscene); 
 glutIdleFunc(&drawscene);
 glutReshapeFunc(&resizer);
 glutSpecialFunc(&arrows); //"Special" key presses
 glutKeyboardFunc(&keypress); //"Special" key presses
 glutPassiveMotionFunc(&mouse);
 initgl(windowwidth(),windowheight());
 glewInit();

 srand(time(0));

 /*make_solution2();
 make_solution3();
 obfuscate_maze();
 write_maze();*/
 
 //decide_hint();
 glutMainLoop(); 

 return 0;
}
