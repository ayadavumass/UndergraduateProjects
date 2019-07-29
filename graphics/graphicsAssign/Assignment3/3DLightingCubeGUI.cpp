
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>

#include <GL/gl.h>
#include <GL/glui.h>						// Header File For The OpenGL32 Library
#include <time.h>
#include<math.h>
#include"Lighting3DCube.h"


#define ID1	1
#define ID2	2
#define ID3	3

GLfloat X=0,Y=0,Z=0;




GLint main_window;

void display_normal(void);


void display_normal(void)
{
   int i, j;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);
   glPushMatrix ();
   glPopMatrix ();
   glFlush();
}

void myGlutIdle( void ) {
	//if ( glutGetWindow() != main_window )
		glutSetWindow(main_window);

	//glutPostRedisplay();
	//sleep(1);
}

void UpdateGraphicsWindow(int ID)
{
	switch(ID)
		{
			case ID1:
			glutSetWindow(main_window);

			glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
			CubeInit(0,0,0);
			glutDisplayFunc(display);
			glutPostRedisplay();
			Z=0;

			break;


			case ID2:
			glutSetWindow(main_window);
            	glMatrixMode(GL_PROJECTION);
            glLoadIdentity();
            glMatrixMode(GL_MODELVIEW);
            glLoadIdentity();
            Z-=2;
            //if(Z>360)
            //Z=0;
			CubeInit(0,0,Z);
			glutDisplayFunc(display);
			glutPostRedisplay();

			break;

			case ID3:


			break;



		}

}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (640, 480);
   glutInitWindowPosition (10, 10);
   main_window =glutCreateWindow ("3D Lighting Calculations ");
   glClearColor(1,1,1,1);
   CubeInit(0,0,0);
   glutDisplayFunc(display);

 //  myinit ();
  // GLUI *glui = GLUI_Master.create_glui ("GLUI", 0 );

  GLUI *glui = GLUI_Master.create_glui_subwindow (main_window,GLUI_SUBWINDOW_LEFT);

  glui->add_statictext( "User Interface" );
  glui->add_separator();
  GLUI_Panel *Panel=glui->add_panel( "Panel",GLUI_PANEL_EMBOSSED);
  GLUI_Button *Object1=glui->add_button_to_panel(Panel,"Draw",ID1,UpdateGraphicsWindow);
  GLUI_Button *Object2=glui->add_button_to_panel(Panel,"Translate Z",ID2,UpdateGraphicsWindow);




  /** Tell GLUI window which other window to recognize as the main gfx window **/
  glui->set_main_gfx_window( main_window );
  /** Register the Idle callback with GLUI (instead of with GLUT)      **/
  GLUI_Master.set_glutIdleFunc (myGlutIdle);

  //GLUI_Master.set_glutKeyboardFunc (key );
  //GLUI_Master.set_glutSpecialFunc (myGlutSpecial );
 // GLUI_Master.set_glutMouseFunc (myMouse );
  //GLUI_Master.set_glutReshapeFunc (resize );

  glutMainLoop();
  return 0;
}
