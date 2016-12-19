//LIBRARIES INCLUDED
#include <math.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <GL/glut.h>
#include <unistd.h>


//HEADER FILES INCLUDED
#include "main.h"
#include "buttons.h"
#include "display.h"
#include "tcp_server.h"
#include "tcp_client.h"

//CONSTANTS
#define WIDTH 1000
#define HEIGHT 1000
#define LENGTH 1000


GLUI* glui;
GLUI_EditText   *edittext;
GLUI* glui1;
GLUI* glui2;
GLUI_RadioGroup *radio;

int width=WIDTH;
int height=HEIGHT;
int length=LENGTH;
int main_window;
int displayed=0;
int   obj = 0;            //For Difficulty glui
int TOTAL_PLAYERS=1;
std::string input_ip = "1.1.1.1";

//Objects of class Ball and struct Mouse created
Mouse TheMouse = {0,0,0,0,0};

int my_id=0;



/*
 * \brief	This is the Glut Idle Function 
 */
 
void idle()
{
	if ( glutGetWindow() != main_window ) 
	    glutSetWindow(main_window); 
   glutPostRedisplay();
}


/*
 * \brief	The Main Function
 */
 

void control_cb( int control )
{
  /********************************************************************
    Here we'll print the user id of the control that generated the
    callback, and we'll also explicitly get the values of each control.
    Note that we really didn't have to explicitly get the values, since
    they are already all contained within the live variables:
    'wireframe',  'segments',  'obj',  and 'text'  
    ********************************************************************/

		
		std::cout<<username<<std::endl;

}

void control_cb0( int control )
{
  /********************************************************************
    Here we'll print the user id of the control that generated the
    callback, and we'll also explicitly get the values of each control.
    Note that we really didn't have to explicitly get the values, since
    they are already all contained within the live variables:
    'wireframe',  'segments',  'obj',  and 'text'  
    ********************************************************************/

		
		std::cout<<input_ip<<std::endl;
		
		
		
		char *cstr = new char[input_ip.length() + 1];
		strcpy(cstr, input_ip.c_str());
		client_conn(cstr);
		std::cout<<displayed<<std::endl;
}

void control_cb4( int control )
{
  /********************************************************************
    Here we'll print the user id of the control that generated the
    callback, and we'll also explicitly get the values of each control.
    Note that we really didn't have to explicitly get the values, since
    they are already all contained within the live variables:
    'wireframe',  'segments',  'obj',  and 'text'  
    ********************************************************************/

		
		std::cout<<"Difficulty"<<obj<<std::endl;
		

}


void control_cb1(int control)
{
	glui->hide();
}


void control_cb2(int control)
{
	glui1->hide();
}

void control_cb3(int control)
{
	glui2->hide();
}

int main(int argc, char *argv[])
{

  
    glutInit (&argc, argv);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutInitWindowPosition(250, 250);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    main_window = glutCreateWindow("ScreenSaver");
   
    init();
	 

	
    glutDisplayFunc(display);
	glutMouseFunc(MouseButton);	
	glutPassiveMotionFunc(MousePassiveMotion);



	glui = GLUI_Master.create_glui( "Username", 0, 400, 50 );
	edittext = new GLUI_EditText( glui, "Username:", username, 3, control_cb );
	new GLUI_Button( glui, "OK", 0,control_cb1);
	glui->set_main_gfx_window( main_window );

  /* We register the idle callback with GLUI, *not* with GLUT */
  GLUI_Master.set_glutIdleFunc( idle );
  GLUI_Master.set_glutIdleFunc( NULL );
	glui->hide();



	glui1 = GLUI_Master.create_glui( "IP Address", 0, 400, 50 );
	edittext = new GLUI_EditText( glui1, "IP:", input_ip, 3, control_cb0 );
	new GLUI_Button( glui1, "OK", 0,control_cb2);
	glui->set_main_gfx_window( main_window );

  /* We register the idle callback with GLUI, *not* with GLUT */
  GLUI_Master.set_glutIdleFunc( idle );
  GLUI_Master.set_glutIdleFunc( NULL );
	glui1->hide();  
    



	glui2 = GLUI_Master.create_glui( "Difficulty", 0, 400, 50 );
	GLUI_Panel *obj_panel = new GLUI_Panel( glui2, "Difficulty Level" );
	radio = new GLUI_RadioGroup( obj_panel,&obj,4,control_cb4 );
  	new GLUI_RadioButton( radio, "Easy" );
  	new GLUI_RadioButton( radio, "Medium" );
  	new GLUI_RadioButton( radio, "Hard" );
	new GLUI_Button(glui2,"OK",0,control_cb3);
	glui2->set_main_gfx_window( main_window );

  /* We register the idle callback with GLUI, *not* with GLUT */
  GLUI_Master.set_glutIdleFunc( idle );
  GLUI_Master.set_glutIdleFunc( NULL );
	glui2->hide(); 

    glutMainLoop();
    return 0;
}
