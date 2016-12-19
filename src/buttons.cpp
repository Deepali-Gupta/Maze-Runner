//LIBRARIES INCLUDED
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <GL/glut.h>
#include <GL/glui.h>
#include <unistd.h>
#include <string.h>
#include <fstream>
#include <sstream>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <resolv.h>
#include "openssl/ssl.h"
#include "openssl/err.h"

//HEADER FILES INCLUDED
#include "buttons.h"
#include "main.h"
#include "display.h"
#include "tcp_server.h"
#include "tcp_client.h"

using namespace std;

extern Button newgame;
extern Button joingame;
extern Button exitgame;
extern Button changeuser;
extern Button difficulty;
extern Button startgame;
extern Button cancelgamenew;

bool send_difficulty=false;
extern Mouse TheMouse;
std::string user = "Ramneek";
pthread_t t1;

void *server_func(void *args){
		server_conn();
}

void write_players(){
	printf("writing players");
	ofstream ffile;
    ffile.open("players.txt");
    for (int i=0;i<6;i++) {
    ffile<<ipaddress[i]<<" ";
    ffile<<playername[i]<<" "; }
    ffile.close();
}

void ThenewgameCallback()
{
	glui->hide();
	displayed=1;
	printf("%d\n",displayed);

	pthread_create(&t1,NULL,server_func,NULL);
	system("g++ -o maze makemaze.cpp");
	system("./maze");


}

void ThejoingameCallback()
{
		glui1->show();
		printf("\nIP entered\n");
		

}

void TheexitgameCallback()
{
		exit(0);
		printf("\nDecrease Called\n");
	
	
}

void ThechangeuserCallback()
{

		glui->show();

		printf("\nChange username\n");
	

	
}

void ThedifficultyCallback()
{
	glui2->show();
	printf("\nChange Difficulty\n");
}

void ThestartgameCallback()
{
	if (TOTAL_PLAYERS!=1)
	{
		send_difficulty = true;
		write_players();
		send_count=0;
		while (flag!=1) {};
	}
	else 
	{
		write_players();
	}
		stringstream ss;
		ss << "./game "<<my_id<<" "<<TOTAL_PLAYERS;
		printf("\nGame Started\n");
		system("g++ -w -o game openmaze.cpp udp_server.cpp udp_client.cpp -lglut -lGL -lGLU -lGLEW -lpthread");
		system(ss.str().c_str());	
}

void ThecancelgamenewCallback()
{
	displayed=0;
	printf("\nGame Cancelled\n");
}

/*
 * \brief	Detects if Mouse is over the button b
 * \param	b	-	The Button	whose click is to be detected
 * \param	x	-	The Mouse x Coordinate
 * \parma	x	-	The Mouse y Coordinate
 */
int ButtonClickTest(Button* b,int x,int y) 
{
	const char* change1 = "Difficulty";
	const char* change2 = "Start Game";
	const char* change3 = "Cancel Game";
	if( b) 
	{
		/*
		 *	If clicked within button area, then return true
		 */
		 x=x-width/2;	//synchronize the mouse and opengl coordinate system
		if (b->h==50)
		 y=(-1)*y+height/2-b->h-20;
		else if (strcmp(b->label,change1)==0)
			y=(-1)*y+height/2-b->h-60;
		else y=(-1)*y+height/2-b->h+10;
	    if( x > b->x && x < b->x+b->w && y > b->y  && y < b->y+b->h ) 
		{
			return 1;
		}
	}
	
	return 0;
}

/*
 * \brief	Detects if button is releases and restores the state of the button
 * \param	b	-	The Button	whose release is to be detected	
 * \param	x	-	The Mouse x Coordinate
 * \parma	x	-	The Mouse y Coordinate
 */

void ButtonRelease(Button *b,int x,int y)
{
	if(b) 
	{
		/*
		 *	If the mouse button was pressed within the button area
		 *	as well as being released on the button.....
		 */
		if( ButtonClickTest(b,TheMouse.xpress,TheMouse.ypress) && ButtonClickTest(b,x,y) )
		{
			/*
			 *	Then if a callback function has been set, call it.
			 */
			if (b->callbackFunction) 
			{
				b->callbackFunction();
			}
		}

		/*
		 *	Set state back to zero.
		 */
		b->state = 0;
	}
}

/*
 * \brief	Detects if button is pressed and changes the state of the button
 * \param	b	-	The Button	whose click is to be detected	
 * \param	x	-	The Mouse x Coordinate
 * \parma	x	-	The Mouse y Coordinate
 */

void ButtonPress(Button *b,int x,int y)
{
	if(b)
	{
		/*
		 *	if the mouse click was within the buttons client area, 
		 *	set the state to true.
		 */
		if( ButtonClickTest(b,x,y) )
		{
			b->state = 1;
		}
	}
}

/*
 * brief	Detects if mouse is over button and highlights it accordingly
 * \param	b	-	The button to be highlighted
 * \param	x	-	x coordinate of the mouse
 * \parma	y	-	y coordinate of the mouse
 */
void ButtonPassive(Button *b,int x,int y)
{
	if(b)
	{
		/*
		 *	if the mouse moved over the control
		 */
		if( ButtonClickTest(b,x,y) )
		{
			/*
			 *	If the cursor has just arrived over the control, set the highlighted flag
			 *	and force a redraw. The screen will not be redrawn again until the mouse
			 *	is no longer over this control
			 */
			if( b->highlighted == 0 ) {
				b->highlighted = 1;
				glutPostRedisplay();
			}
		}
		else

		/*
		 *	If the cursor is no longer over the control, then if the control
		 *	is highlighted (ie, the mouse has JUST moved off the control) then
		 *	we set the highlighting back to false, and force a redraw. 
		 */
		if( b->highlighted == 1 )
		{
			b->highlighted = 0;
			glutPostRedisplay();
		}
	}
}

/*
 * \brief	Defines the font of the button text
 * \param	font	-	The font of the text to be printed
 * \param	text	-	The text to be printed
 * \param	x		-	x coordinate of the text
 * \param	y		-	y cooridnate of the text
 */
void Font(void *font,const char *text,int x,int y)
{
	glRasterPos2i(x, y);
	while( *text != '\0' )
	{
		glutBitmapCharacter( font, *text );
		++text;
	}
}

void ButtonDraw(Button *b)
{
	const char* change = "Change Username";
	const char* change1 = "Difficulty";
	if (strcmp(b->label,change)==0 or strcmp(b->label,change1)==0)
		ButtonDraw1(b);
	else
	{
	
	int fontx;
	int fonty;
	if(b)
	{
		fontx = b->x + (b->w - glutBitmapLength(GLUT_BITMAP_HELVETICA_18,(const unsigned char*)b->label))/2 ;
		fonty = b->y + (b->h)/2;
			
		/*
		 *	if the button is pressed, make it look as though the string has been pushed
		 *	down. It's just a visual thing to help with the overall look....
		 */
		if (b->state) 
		{
			fontx+=2;
			fonty+=2;
		}

		/*
		 *	If the cursor is currently over the button we offset the text string and draw a shadow
		 */
		if(b->highlighted)
		{
			glColor3f(0,0,0);
			Font(GLUT_BITMAP_HELVETICA_18,b->label,fontx,fonty);
			fontx--;
			fonty--;
		}

		glColor3f(1,1,1);
		Font(GLUT_BITMAP_HELVETICA_18,b->label,fontx,fonty);
		/*
		 *	We will indicate that the mouse cursor is over the button by changing its
		 *	colour.
		 */
		if (b->highlighted) 
			glColor3f(0.7f,0.7f,0.8f);
		else 
			glColor3f(0.6f,0.6f,0.6f);

		/*
		 *	draw background for the button.
		 */
		glBegin(GL_QUADS);
		glVertex2i( b->x     , b->y      );
		glVertex2i( b->x     , b->y+b->h );
		glVertex2i( b->x+b->w, b->y+b->h );
		glVertex2i( b->x+b->w, b->y      );
		glEnd();

		/*
		 *	Draw an outline around the button with width 3
		 */
		glLineWidth(3);

		/*
		 *	The colours for the outline are reversed when the button is clicked
		 */
		if (b->state) 
			glColor3f(0.4f,0.4f,0.4f);
		else 
			glColor3f(0.8f,0.8f,0.8f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x+b->w, b->y      );
			glVertex2i( b->x     , b->y      );
			glVertex2i( b->x     , b->y+b->h );
		glEnd();

		if (b->state) 
			glColor3f(0.8f,0.8f,0.8f);
		else 
			glColor3f(0.4f,0.4f,0.4f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x     , b->y+b->h );
			glVertex2i( b->x+b->w, b->y+b->h );
			glVertex2i( b->x+b->w, b->y      );
		glEnd();

		glLineWidth(1);	
		}	
	}
}



void ButtonDraw1(Button *b)
{
	
	
	int fontx;
	int fonty;
	if(b)
	{
		fontx = b->x + (b->w - glutBitmapLength(GLUT_BITMAP_HELVETICA_10,(const unsigned char*)b->label))/2 ;
		fonty = b->y + (b->h)/2;
			
		/*
		 *	if the button is pressed, make it look as though the string has been pushed
		 *	down. It's just a visual thing to help with the overall look....
		 */
		if (b->state) 
		{
			fontx+=2;
			fonty+=2;
		}

		/*
		 *	If the cursor is currently over the button we offset the text string and draw a shadow
		 */
		if(b->highlighted)
		{
			glColor3f(0,0,0);
			Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);
			fontx--;
			fonty--;
		}

		glColor3f(1,1,1);
		Font(GLUT_BITMAP_HELVETICA_10,b->label,fontx,fonty);
		/*
		 *	We will indicate that the mouse cursor is over the button by changing its
		 *	colour.
		 */
		if (b->highlighted) 
			glColor3f(0.7f,0.7f,0.8f);
		else 
			glColor3f(0.6f,0.6f,0.6f);

		/*
		 *	draw background for the button.
		 */
		glBegin(GL_QUADS);
		glVertex2i( b->x     , b->y      );
		glVertex2i( b->x     , b->y+b->h );
		glVertex2i( b->x+b->w, b->y+b->h );
		glVertex2i( b->x+b->w, b->y      );
		glEnd();

		/*
		 *	Draw an outline around the button with width 3
		 */
		glLineWidth(3);

		/*
		 *	The colours for the outline are reversed when the button is clicked
		 */
		if (b->state) 
			glColor3f(0.4f,0.4f,0.4f);
		else 
			glColor3f(0.8f,0.8f,0.8f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x+b->w, b->y      );
			glVertex2i( b->x     , b->y      );
			glVertex2i( b->x     , b->y+b->h );
		glEnd();

		if (b->state) 
			glColor3f(0.8f,0.8f,0.8f);
		else 
			glColor3f(0.4f,0.4f,0.4f);

		glBegin(GL_LINE_STRIP);
			glVertex2i( b->x     , b->y+b->h );
			glVertex2i( b->x+b->w, b->y+b->h );
			glVertex2i( b->x+b->w, b->y      );
		glEnd();

		glLineWidth(1);	
			
	}
}


/*
 * \brief	Operates on click of mouse button
 * \param 	button	-	which button of mous eis pressed
 * \param	state	-	mouse is pressed or released
 * \param	x		-	mouse x coordinate
 * \param	y		-	mouse y coordinate
 */
 
void MouseButton(int button,int state,int x, int y)
{
	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;

	/*
	 *	has the button been pressed or released?
	 */
	if (state == GLUT_DOWN) 
	{
		#ifdef DEBUG
			printf("\nMouse clicked\n");
		#endif
		/*
		 *	This holds the location of the first mouse click
		 */
		if ( !(TheMouse.lmb) ) 
		{
			TheMouse.xpress = x;
			TheMouse.ypress = y;			
		}

		/*
		 *	Which button was pressed?
		 */
		 if(button==GLUT_LEFT_BUTTON)
		 {
			 TheMouse.lmb = 1;
			if (displayed==0){
			ButtonPress(&newgame,x,y);
			ButtonPress(&joingame,x,y);
			ButtonPress(&exitgame,x,y);
			ButtonPress(&changeuser,x,y);}
			else{
			ButtonPress(&difficulty,x,y);
			ButtonPress(&startgame,x,y);
			ButtonPress(&cancelgamenew,x,y);
			}	
		}
	}
	else 
	{
		/*
		 *	Which button was released?
		 */
		if(button==GLUT_LEFT_BUTTON)
		{
			TheMouse.lmb = 0;
			if (displayed==0){
			ButtonRelease(&newgame,x,y);
			ButtonRelease(&joingame,x,y);
			ButtonRelease(&exitgame,x,y);
			ButtonRelease(&changeuser,x,y);}
			else{
			ButtonRelease(&difficulty,x,y);
			ButtonRelease(&startgame,x,y);
			ButtonRelease(&cancelgamenew,x,y);
			}
		}
	}

	/*
	 *	Force a redraw of the screen. If we later want interactions with the mouse
	 *	and the 3D scene, we will need to redraw the changes.
	 */
	glutPostRedisplay();
}

/*
 * \brief	Operates on hovering of mouse
 * \param	x		-	mouse x coordinate
 * \param	y		-	mouse y coordinate
 */

void MousePassiveMotion(int x, int y)
{
	/*
	 *	Calculate how much the mouse actually moved
	 */
	int dx = x - TheMouse.x;
	int dy = y - TheMouse.y;

	/*
	 *	update the mouse position
	 */
	TheMouse.x = x;
	TheMouse.y = y;
	if (displayed==0){
	ButtonPassive(&newgame,x,y);
	ButtonPassive(&joingame,x,y);
	ButtonPassive(&exitgame,x,y);
	ButtonPassive(&changeuser,x,y);}
	else{
	ButtonPassive(&difficulty,x,y);
	ButtonPassive(&startgame,x,y);
	ButtonPassive(&cancelgamenew,x,y);
	}
}



