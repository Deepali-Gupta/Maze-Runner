//LIBRARIES INCLUDED
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <unistd.h>
#include <string>

//HEADER FILES INCLUDED
#include "display.h"
#include "main.h"
#include "buttons.h"
#include "tcp_server.h"
#include "tcp_client.h"



char* buf;
std::string correct_ip = "1.2.3.5";     //Actual values of the attributes
std::string username = "Ramneek";
std::string diff = "Easy";
const char* username1;                //Used to display on screen
const char* ip10;
const char* diff1;

 int flag=0;
 bool attributes_set = false;
 std::string ipaddress[6] = {"","","","","",""};
 std::string playername[6] = {username,"AI","AI","AI","AI","AI"};



extern Mouse TheMouse;

Button newgame = {-width/2+400,height/2-400, 200,50, 0,0, "New Game", ThenewgameCallback };
Button joingame = {-width/2+400,height/2-500, 200,50, 0,0, "Join Game", ThejoingameCallback };
Button exitgame = {-width/2+400,height/2-600, 200,50, 0,0, "Exit", TheexitgameCallback };
Button changeuser = {width/2-200,height/2-90, 100,25, 0,0, "Change Username", ThechangeuserCallback };

Button difficulty = {-120,height/2-550,100,25,0,0, "Difficulty", ThedifficultyCallback};
Button startgame = {-200,height/2-700, 200,50, 0,0, "Start Game", ThestartgameCallback };
Button cancelgamenew = {50,height/2-700, 200,50, 0,0, "Cancel Game", ThecancelgamenewCallback };




void Font2(void *font,const char *text,int x,int y)
{
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glRasterPos2i(x, y);
	while( *text != '\0' )
	{
		glutBitmapCharacter( font, *text );
		++text;
	}
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
}




void init_light(int light_source, float Lx, float Ly, float Lz, float Lr, float Lg, float Lb)
{
   // Light variables
   float light_position[] = { Lx, Ly, Lz, 0.0 };
   float light_color[] = { Lr, Lg, Lb, 1.0 };

   // Initialize light source
   glEnable(GL_LIGHTING);
   glEnable(light_source);
   glLightfv(light_source, GL_POSITION, light_position);
   glLightfv(light_source, GL_AMBIENT, light_color);
   glLightfv(light_source, GL_DIFFUSE, light_color);
   glLightfv(light_source, GL_SPECULAR, light_color);
   glLightf(light_source, GL_CONSTANT_ATTENUATION, 1.0);
   glLightf(light_source, GL_LINEAR_ATTENUATION, 0.0);
   glLightf(light_source, GL_QUADRATIC_ATTENUATION, 0.0);
   glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_FALSE);
   glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
}

/*
 * \brief	initialises the screen
 */
 
void init()
{
	#ifdef DEBUG
		printf("\nScreen Initialized\n");
	#endif
      // Initialize OpenGL
   glClearColor(0.0, 0.0, 0.0, 1.0);
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   glOrtho(-width/2, width/2, -height/2, height/2, -length/2, length/2);
   glEnable(GL_DEPTH_TEST);

   // Initialize smooth shading
   glShadeModel(GL_SMOOTH);
   init_light(GL_LIGHT0, 0, 1, 1, 1, 1, 1);
   init_light(GL_LIGHT1, 1, 0, 1, 1, 1, 1);
   init_light(GL_LIGHT2, 1, 1, 0, 1, 1, 1);   
}


void shownewgame()
{
	if (displayed==1)
		Font2(GLUT_BITMAP_TIMES_ROMAN_24,"New Game",-100,height/2-100);
	if (displayed==2)
		Font2(GLUT_BITMAP_TIMES_ROMAN_24,"Join Game",-100,height/2-100);
	username1 = playername[0].c_str();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0.0,1.0,0.0);
	Font2(GLUT_BITMAP_HELVETICA_18,username1,-200,height/2-200);
	glPopAttrib();

	username1 = playername[1].c_str();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1.0,0.0,0.0);
	Font2(GLUT_BITMAP_HELVETICA_18,username1,-200,height/2-250);
	glPopAttrib();

	username1 = playername[2].c_str();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0.0,0.0,1.0);
	Font2(GLUT_BITMAP_HELVETICA_18,username1,-200,height/2-300);
	glPopAttrib();

	username1 = playername[3].c_str();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1.0,1.0,1.0);
	Font2(GLUT_BITMAP_HELVETICA_18,username1,-200,height/2-350);
	glPopAttrib();

	username1 = playername[4].c_str();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1.0,1.0,0.0);
	Font2(GLUT_BITMAP_HELVETICA_18,username1,-200,height/2-400);
	glPopAttrib();

	username1 = playername[5].c_str();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0.0,1.0,1.0);
	Font2(GLUT_BITMAP_HELVETICA_18,username1,-200,height/2-450);
	glPopAttrib();
	
	ip10 = ipaddress[0].c_str();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0.0,1.0,0.0);
	Font2(GLUT_BITMAP_HELVETICA_18,ip10,50,height/2-200);
	glPopAttrib();

	ip10 = ipaddress[1].c_str();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1.0,0.0,0.0);
	Font2(GLUT_BITMAP_HELVETICA_18,ip10,50,height/2-250);
	glPopAttrib();

	ip10 = ipaddress[2].c_str();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0.0,0.0,1.0);
	Font2(GLUT_BITMAP_HELVETICA_18,ip10,50,height/2-300);
	glPopAttrib();

	ip10 = ipaddress[3].c_str();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1.0,1.0,1.0);
	Font2(GLUT_BITMAP_HELVETICA_18,ip10,50,height/2-350);
	glPopAttrib();

	ip10 = ipaddress[4].c_str();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(1.0,1.0,0.0);
	Font2(GLUT_BITMAP_HELVETICA_18,ip10,50,height/2-400);
	glPopAttrib();

	ip10 = ipaddress[5].c_str();
	glPushAttrib(GL_CURRENT_BIT);
	glColor3f(0.0,1.0,1.0);
	Font2(GLUT_BITMAP_HELVETICA_18,ip10,50,height/2-450);
	glPopAttrib();
	
	if (displayed==1)
	{
	ButtonDraw(&difficulty);
	}
	if (obj==0)
		diff = "Easy";
	if (obj==1)
		diff = "Medium";
	if (obj==2)
		diff = "Hard";

	
	diff1 = diff.c_str();
	Font2(GLUT_BITMAP_HELVETICA_18,diff1,30,height/2-540);


	Font2(GLUT_BITMAP_HELVETICA_18,"Host IP",-120,height/2-600);
	ip10 = correct_ip.c_str();
	Font2(GLUT_BITMAP_HELVETICA_18,ip10,27,height/2-600);
	if (displayed==1)
		ButtonDraw(&startgame);

	ButtonDraw(&cancelgamenew);

}


/*
 * \brief	The Glut Display Function
 */
 
void display()
{   
	if (displayed==0)
   {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity(); 
	
	Font2(GLUT_BITMAP_TIMES_ROMAN_24,"Maze Runner",-width/2+100,height/2-100);
	std::string temp = "Welcome "+username;
	username1 = temp.c_str();
	Font(GLUT_BITMAP_HELVETICA_12,username1,width/2-200,height/2-50);
	

	//DrawCube();
	Draw2D();
  
	
	glutSwapBuffers();
	}
	if (displayed==1 or displayed==2)
	{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clean the screen and the depth buffer
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	shownewgame();
	glutSwapBuffers();
	}
}

/*
 * \brief	Draws the buttons
 */
 
void Draw2D()
{
	ButtonDraw(&newgame);
	ButtonDraw(&joingame);
	ButtonDraw(&exitgame);
	ButtonDraw(&changeuser);

}
