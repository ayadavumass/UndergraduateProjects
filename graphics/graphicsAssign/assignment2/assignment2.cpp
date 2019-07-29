
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glu.h>
#include <stdio.h>
#include "assignscene.h"
#include <GL/gl.h>
#include <GL/glui.h>						// Header File For The OpenGL32 Library
#include <time.h>
#include<math.h>



#define ID1	1
#define ID2	2
#define ID3	3
#define ID4	4
#define ID5	5
#define ID6	6
#define ID7	7
#define ID8	8
#define ID9	9
#define ID10	10
#define ID11	11
#define ID12	12
#define ID13	13
#define ID14	14
#define ID15	15
#define ID16	16


GLint main_window;
GLint main_win2;



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

	glutPostRedisplay();
	sleep(1);
}

void UpdateGraphicsWindow(int ID)
{
	switch(ID)
		{
			case ID1:
			glutSetWindow(main_window);
			glutPostRedisplay();
			glMatrixMode(GL_MODELVIEW);
   			glLoadIdentity();
			  glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glutDisplayFunc(mydisplay);
			break;


			case ID2:
			glutSetWindow(main_window);
			glutPostRedisplay();
			glMatrixMode(GL_MODELVIEW);
   			glLoadIdentity();
			 /* clear the color buffer */
  //glClear(GL_COLOR_BUFFER_BIT);
 			 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
 
			glutDisplayFunc(cubedisplay);
			break;

			case ID3:
			glutSetWindow(main_window);
			glutPostRedisplay();
			glMatrixMode(GL_MODELVIEW);
   			//glLoadIdentity();
			 /* clear the color buffer */
  //glClear(GL_COLOR_BUFFER_BIT);
 			 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			myRotate(20.0,1.0,0.0,0.0);      //our method fails if both y and z are zero
 
			glutDisplayFunc(cubedisplay);

			
			break;


			case ID4:
			glutSetWindow(main_window);
			glutPostRedisplay();
			glMatrixMode(GL_MODELVIEW);
   			//glLoadIdentity();
			 /* clear the color buffer */
  //glClear(GL_COLOR_BUFFER_BIT);
 			 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			myRotate(20.0,0.0,1.0,0.0);      //our method fails if both y and z are zero
 
			glutDisplayFunc(cubedisplay);

			
			break;

			case ID5:
			glutSetWindow(main_window);
			glutPostRedisplay();
			glMatrixMode(GL_MODELVIEW);
   			//glLoadIdentity();
			 /* clear the color buffer */
  //glClear(GL_COLOR_BUFFER_BIT);
 			 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			myRotate(20.0,0.0,0.0,1.0);      //our method fails if both y and z are zero
 
			glutDisplayFunc(cubedisplay);
			break;


			case ID6:
			glutSetWindow(main_window);
			glutPostRedisplay();
			glMatrixMode(GL_MODELVIEW);
   			//glLoadIdentity();
			 /* clear the color buffer */
  //glClear(GL_COLOR_BUFFER_BIT);
 			 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			myScale(2.0,2.0,2.0);      //our method fails if both y and z are zero
 
			glutDisplayFunc(cubedisplay);
			break;

			case ID7:
			glutSetWindow(main_window);
			glutPostRedisplay();
			glMatrixMode(GL_MODELVIEW);
   			//glLoadIdentity();
			 /* clear the color buffer */
  //glClear(GL_COLOR_BUFFER_BIT);
 			 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			myScale(0.5,0.5,0.5);      //our method fails if both y and z are zero
 
			glutDisplayFunc(cubedisplay);
			break;


			case ID8:
			glutSetWindow(main_window);
			glutPostRedisplay();
			glMatrixMode(GL_MODELVIEW);
   			//glLoadIdentity();
			 /* clear the color buffer */
  //glClear(GL_COLOR_BUFFER_BIT);
 			 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			myTranslate(1,0,0);      //our method fails if both y and z are zero
 
			glutDisplayFunc(cubedisplay);
			break;

			case ID9:
			glutSetWindow(main_window);
			glutPostRedisplay();
			glMatrixMode(GL_MODELVIEW);
   			//glLoadIdentity();
			 /* clear the color buffer */
  //glClear(GL_COLOR_BUFFER_BIT);
 			 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			myTranslate(0,1,0);      //our method fails if both y and z are zero
 
			glutDisplayFunc(cubedisplay);
			break;

			case ID10:
			glutSetWindow(main_window);
			glutPostRedisplay();
			glMatrixMode(GL_MODELVIEW);
   			//glLoadIdentity();
			 /* clear the color buffer */
  //glClear(GL_COLOR_BUFFER_BIT);
 			 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			myTranslate(0,0,1);      //our method fails if both y and z are zero
 
			glutDisplayFunc(cubedisplay);
			break;



			case ID11:
			glutSetWindow(main_window);
			glutPostRedisplay();
			glMatrixMode(GL_MODELVIEW);
   			//glLoadIdentity();
			 /* clear the color buffer */
  //glClear(GL_COLOR_BUFFER_BIT);
 			 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			myShear(SXY,20);      //our method fails if both y and z are zero
 
			glutDisplayFunc(cubedisplay);
			break;


			case ID12:
			glutSetWindow(main_window);
			glutPostRedisplay();
			glMatrixMode(GL_MODELVIEW);
   			//glLoadIdentity();
			 /* clear the color buffer */
  //glClear(GL_COLOR_BUFFER_BIT);
 			 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			myShear(SYZ,20);      //our method fails if both y and z are zero
 
			glutDisplayFunc(cubedisplay);
			break;


			case ID13:
			glutSetWindow(main_window);
			glutPostRedisplay();
			glMatrixMode(GL_MODELVIEW);
   			//glLoadIdentity();
			 /* clear the color buffer */
  //glClear(GL_COLOR_BUFFER_BIT);
 			 glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			
			myShear(SZX,20);      //our method fails if both y and z are zero
 
			glutDisplayFunc(cubedisplay);
			break;
		}
	
}

int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   main_window =glutCreateWindow ("Graphics Assignment II");
   glutDisplayFunc(display_normal);
   //glutReshapeFunc(reshape);
   //glutKeyboardFunc(keyboard);
   myinit ();
 //GLUI *glui = GLUI_Master.create_glui ("GLUI", 0 );
  
  GLUI *glui = GLUI_Master.create_glui_subwindow (main_window,GLUI_SUBWINDOW_LEFT);
                                        
  glui->add_statictext( "User Interface" );
  glui->add_separator();
  //glui->add_checkbox( "Wireframe", &wireframe, 1, control_cb );
  //GLUI_Spinner *segment_spinner =
    //glui->add_spinner( "Segments:",GLUI_SPINNER_INT, &segments );
  //segment_spinner->set_int_limits( 3, 60, GLUI_LIMIT_WRAP );
  GLUI_Button *Scene=glui->add_button("Scene",ID1,UpdateGraphicsWindow);

GLUI_Panel *Transform_Panel=glui->add_panel( "Transform Panel",GLUI_PANEL_EMBOSSED);
GLUI_Button *Object=glui->add_button_to_panel(Transform_Panel,"Display Object",ID2,UpdateGraphicsWindow);

GLUI_Panel *Rotate_Panel=glui->add_panel_to_panel( Transform_Panel,"Rotate Panel",GLUI_PANEL_EMBOSSED);
GLUI_Button *RX=glui->add_button_to_panel(Rotate_Panel,"Rotate X",ID3,UpdateGraphicsWindow);
GLUI_Button *RY=glui->add_button_to_panel(Rotate_Panel,"Rotate Y",ID4,UpdateGraphicsWindow);
GLUI_Button *RZ=glui->add_button_to_panel(Rotate_Panel,"Rotate Z",ID5,UpdateGraphicsWindow);


GLUI_Panel *Scale_Panel=glui->add_panel_to_panel( Transform_Panel,"Scale Panel",GLUI_PANEL_EMBOSSED);
GLUI_Button *SU=glui->add_button_to_panel(Scale_Panel,"Scale UP",ID6,UpdateGraphicsWindow);
GLUI_Button *SD=glui->add_button_to_panel(Scale_Panel,"Scale Down",ID7,UpdateGraphicsWindow);


GLUI_Panel *Translate_Panel=glui->add_panel_to_panel( Transform_Panel,"Scale Panel",GLUI_PANEL_EMBOSSED);
GLUI_Button *TX=glui->add_button_to_panel(Translate_Panel,"Translate X",ID8,UpdateGraphicsWindow);
GLUI_Button *TY=glui->add_button_to_panel(Translate_Panel,"Translate Y",ID9,UpdateGraphicsWindow);
GLUI_Button *TZ=glui->add_button_to_panel(Translate_Panel,"Translate Z",ID10,UpdateGraphicsWindow);


GLUI_Panel *Shear_Panel=glui->add_panel_to_panel( Transform_Panel,"Scale Panel",GLUI_PANEL_EMBOSSED);
GLUI_Button *bSXY=glui->add_button_to_panel(Shear_Panel,"Shear XY",ID11,UpdateGraphicsWindow);
GLUI_Button *bSYZ=glui->add_button_to_panel(Shear_Panel,"Shear YZ",ID12,UpdateGraphicsWindow);
GLUI_Button *bSZX=glui->add_button_to_panel(Shear_Panel,"Shear ZX",ID13,UpdateGraphicsWindow);

/*  GLUI_Button *Eval_Curve_Button=glui->add_button("Eval Curve",ID3,UpdateGraphicsWindow);
  GLUI_Button *BezCurve_Button=glui->add_button("Bez Surface",ID6,UpdateGraphicsWindow);
  GLUI_Button *Specify_Button=glui->add_button("Specify Points",ID4,UpdateGraphicsWindow);
  GLUI_Button *Draw_Button=glui->add_button("Draw Curve",ID5,UpdateGraphicsWindow);
  GLUI_Button *Rotate_ButtonP=glui->add_button("Rotate +",ID7,UpdateGraphicsWindow);
  GLUI_Button *Rotate_ButtonM=glui->add_button("Rotate -",ID8,UpdateGraphicsWindow);
*/

	
  //GLUI_Rotation* Rotor=glui->add_rotation( "Rotate",Rotate_Matrix ,ID7,UpdateGraphicsWindow );

  
  //GLUI_EditText *edittext =glui->add_edittext( "", GLUI_EDITTEXT_FLOAT,&Var  );
  //glui->add_column(true);          /** Begin new column - 'true' indicates  **/
                                  /* * a vertical bar should be drawn       **/
  //GLUI_Panel *obj_panel = glui->add_panel ("Object Type" );
                                           
  //GLUI_RadioGroup *group1 =
    //  glui->add_radiogroup_to_panel(obj_panel,&obj,3,control_cb);
  //glui->add_radiobutton_to_group( group1, "Sphere" );
  //glui->add_radiobutton_to_group( group1, "Torus" );
  //glui->add_button( "Quit", 0,(GLUI_Update_CB)exit );
  /** Tell GLUI window which other window to recognize as the main gfx window **/
  glui->set_main_gfx_window( main_window );
  /** Register the Idle callback with GLUI (instead of with GLUT)      **/
  GLUI_Master.set_glutIdleFunc (myGlutIdle);
  
  GLUI_Master.set_glutKeyboardFunc (keyboard );
  //GLUI_Master.set_glutSpecialFunc (myGlutSpecial );
 // GLUI_Master.set_glutMouseFunc (myMouse );
  GLUI_Master.set_glutReshapeFunc (reshape );

  glutMainLoop();
  return 0;
}
