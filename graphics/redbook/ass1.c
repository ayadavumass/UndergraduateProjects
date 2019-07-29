/*
 * Copyright (c) 1993-1997, Silicon Graphics, Inc.
 * ALL RIGHTS RESERVED 
 * Permission to use, copy, modify, and distribute this software for 
 * any purpose and without fee is hereby granted, provided that the above
 * copyright notice appear in all copies and that both the copyright notice
 * and this permission notice appear in supporting documentation, and that 
 * the name of Silicon Graphics, Inc. not be used in advertising
 * or publicity pertaining to distribution of the software without specific,
 * written prior permission. 
 *
 * THE MATERIAL EMBODIED ON THIS SOFTWARE IS PROVIDED TO YOU "AS-IS"
 * AND WITHOUT WARRANTY OF ANY KIND, EXPRESS, IMPLIED OR OTHERWISE,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY OR
 * FITNESS FOR A PARTICULAR PURPOSE.  IN NO EVENT SHALL SILICON
 * GRAPHICS, INC.  BE LIABLE TO YOU OR ANYONE ELSE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY
 * KIND, OR ANY DAMAGES WHATSOEVER, INCLUDING WITHOUT LIMITATION,
 * LOSS OF PROFIT, LOSS OF USE, SAVINGS OR REVENUE, OR THE CLAIMS OF
 * THIRD PARTIES, WHETHER OR NOT SILICON GRAPHICS, INC.  HAS BEEN
 * ADVISED OF THE POSSIBILITY OF SUCH LOSS, HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE
 * POSSESSION, USE OR PERFORMANCE OF THIS SOFTWARE.
 * 
 * US Government Users Restricted Rights 
 * Use, duplication, or disclosure by the Government is subject to
 * restrictions set forth in FAR 52.227.19(c)(2) or subparagraph
 * (c)(1)(ii) of the Rights in Technical Data and Computer Software
 * clause at DFARS 252.227-7013 and/or in similar or successor
 * clauses in the FAR or the DOD or NASA FAR Supplement.
 * Unpublished-- rights reserved under the copyright laws of the
 * United States.  Contractor/manufacturer is Silicon Graphics,
 * Inc., 2011 N.  Shoreline Blvd., Mountain View, CA 94039-7311.
 *
 * OpenGL(R) is a registered trademark of Silicon Graphics, Inc.
 */

/*  bezsurf.c
 *  This program renders a wireframe Bezier surface,
 *  using two-dimensional evaluators.
 */
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glui.h>

int main_window;
typedef struct point
{
  float x;
  float y;
  float z;
}point;

GLfloat ctrlpoints[4][4][3] = {
   {{-1.5, -1.5, 4.0}, {-0.5, -1.5, 2.0}, 
    {0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}}, 
   {{-1.5, -0.5, 1.0}, {-0.5, -0.5, 3.0}, 
    {0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}}, 
   {{-1.5, 0.5, 4.0}, {-0.5, 0.5, 0.0}, 
    {0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}}, 
   {{-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0}, 
    {0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};

/*point ctrlpoints[4][4]={
   {{-1.5, -1.5, 4.0}, {-0.5, -1.5, 2.0}, 
    {0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}}, 
   {{-1.5, -0.5, 1.0}, {-0.5, -0.5, 3.0}, 
    {0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}}, 
   {{-1.5, 0.5, 4.0}, {-0.5, 0.5, 0.0}, 
    {0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}}, 
   {{-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0}, 
    {0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};*/


/*

*/

/*point ctrlpoints[4][4]={
  {{-1.0, 0.0, 2.0},  {0.0, 1.0, 2.0}, {1.0, 0.0, 2.0}, {0.0, -1.0, 2.0}},
 {{-1.0, 0.0, 1.0},  {0.0, 1.0, 1.0}, {1.0, 0.0, 1.0}, {0.0, -1.0, 1.0}},  
 {{-1.0, 0.0, 0.0},  {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}},
  {{-1.0, 0.0, -1.0},  {0.0, 1.0, -1.0}, {1.0, 0.0, -1.0}, {0.0, -1.0, -1.0}}
};*/

void lerp (point *dest, point *a, point *b, float t)
{
  dest->x = a->x + (b->x-a->x)*t;
  dest->y = a->y + (b->y-a->y)*t;
  dest->z = a->z + (b->z-a->z)*t;
}

void bezier (point *dest, float t,point *Ptr)
{
	//printf("Value %lf %lf %lf \n",Ptr[1].x,Ptr[1].y,Ptr[1].z);
  point ab,bc,cd,abbc,bccd;
  lerp (&ab, &Ptr[0],&Ptr[1],t);           // point between a and b (green)
  lerp (&bc, &Ptr[1],&Ptr[2],t);           // point between b and c (green)
  lerp (&cd, &Ptr[2],&Ptr[3],t);           // point between c and d (green)
  lerp (&abbc, &ab,&bc,t);       // point between ab and bc (blue)
  lerp (&bccd, &bc,&cd,t);       // point between bc and cd (blue)
  lerp (dest, &abbc,&bccd,t);   // point on the bezier-curve (black)
}



GLfloat HBctrlpoints[4][4][3];
GLfloat BHctrlpoints[4][4][3];
void funcHB()
{
	int i=0;
	for(i=0;i<4;i++)
	{
		int j=0;
		for(j=0;j<4;j++)
		{
			switch(j)
			{
				case 0:
					{
						HBctrlpoints[i][j][0]=ctrlpoints[i][j].x;
						HBctrlpoints[i][j][1]=ctrlpoints[i][j].y;
						HBctrlpoints[i][j][2]=ctrlpoints[i][j].z;
						
					}
				break;
				case 1:
					{
						HBctrlpoints[i][j][0]=ctrlpoints[i][0].x+ctrlpoints[i][1].x/3;
						HBctrlpoints[i][j][1]=ctrlpoints[i][0].y+ctrlpoints[i][1].y/3;
						HBctrlpoints[i][j][2]=ctrlpoints[i][0].z+ctrlpoints[i][1].z/3;
						
					}
				break;
				case 2:
					{
						HBctrlpoints[i][j][0]=ctrlpoints[i][3].x-ctrlpoints[i][2].x/3;
						HBctrlpoints[i][j][1]=ctrlpoints[i][3].y-ctrlpoints[i][2].y/3;
						HBctrlpoints[i][j][2]=ctrlpoints[i][3].z-ctrlpoints[i][2].z/3;
						
					}
				break;
				case 3:
					{
						HBctrlpoints[i][j][0]=ctrlpoints[i][j].x;
						HBctrlpoints[i][j][1]=ctrlpoints[i][j].y;
						HBctrlpoints[i][j][2]=ctrlpoints[i][j].z;
					}
				break;
			}
		}
	}
}

void funcBH()
{
	int i=0;
	for(i=0;i<4;i++)
	{
		int j=0;
		for(j=0;j<4;j++)
		{
			switch(j)
			{
				case 0:
					{
						BHctrlpoints[i][j][0]=HBctrlpoints[i][j][0];
						BHctrlpoints[i][j][1]=HBctrlpoints[i][j][1];
						BHctrlpoints[i][j][2]=HBctrlpoints[i][j][2];
						
					}
				break;
				case 1:
					{
						BHctrlpoints[i][j][0]=3*(HBctrlpoints[i][1][0]-HBctrlpoints[i][0][0]);
						BHctrlpoints[i][j][1]=3*(HBctrlpoints[i][1][1]-HBctrlpoints[i][0][1]);
						BHctrlpoints[i][j][2]=3*(HBctrlpoints[i][1][2]-HBctrlpoints[i][0][2]);
						
					}
				break;
				case 2:
					{
						BHctrlpoints[i][j][0]=3*(HBctrlpoints[i][3][0]-HBctrlpoints[i][2][0]);
						BHctrlpoints[i][j][1]=3*(HBctrlpoints[i][3][1]-HBctrlpoints[i][2][1]);
						BHctrlpoints[i][j][2]=3*(HBctrlpoints[i][3][2]-HBctrlpoints[i][2][2]);
						
					}
				break;
				case 3:
					{
						BHctrlpoints[i][j][0]=HBctrlpoints[i][j][0];
						BHctrlpoints[i][j][1]=HBctrlpoints[i][j][1];
						BHctrlpoints[i][j][2]=HBctrlpoints[i][j][2];
						
					}
				break;
			}
		}
	}
}





void display(void)
{
   int i, j;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);
   glPushMatrix ();
   glRotatef(85.0, 1.0, 1.0, 1.0);
   for (j = 0; j <= 8; j++) {
      glBegin(GL_LINE_STRIP);
      for (i = 0; i <= 30; i++)
         {
		point qi[4],p;
		bezier (&qi[0], (GLfloat)j/8.0,ctrlpoints[0]);
		bezier (&qi[1], (GLfloat)j/8.0,ctrlpoints[1]);
		bezier (&qi[2], (GLfloat)j/8.0,ctrlpoints[2]);
		bezier (&qi[3], (GLfloat)j/8.0,ctrlpoints[3]);
		bezier (&p, (GLfloat)i/30.0,qi);
		glVertex3fv((GLfloat*)&p);
		//glEvalCoord2f((GLfloat)i/30.0, (GLfloat)j/8.0);
	 }
      glEnd();
      glBegin(GL_LINE_STRIP);
      for (i = 0; i <= 30; i++)
         {
		point qi[4],p;
		bezier (&qi[0], (GLfloat)i/30.0,ctrlpoints[0]);
		bezier (&qi[1], (GLfloat)i/30.0,ctrlpoints[1]);
		bezier (&qi[2], (GLfloat)i/30.0,ctrlpoints[2]);
		bezier (&qi[3], (GLfloat)i/30.0,ctrlpoints[3]);
		bezier (&p, (GLfloat)j/8.0,qi);
		glVertex3fv((GLfloat*)&p);
		//glEvalCoord2f((GLfloat)j/8.0, (GLfloat)i/30.0);
	 }
      glEnd();
   }
   glPopMatrix ();

   glPointSize(5.0);
   glColor3f(1.0, 1.0, 0.0);
   glBegin(GL_POINTS);
      for (i = 0; i < 4; i++)
	for(j=0;j<4;j++) 
         glVertex3fv((GLfloat*)&ctrlpoints[i][j]);
   glEnd();
   glFlush();
}

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
           0, 1, 12, 4, (GLfloat*)&ctrlpoints[0][0]);
   glEnable(GL_MAP2_VERTEX_3);
   glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
}

void reshape(int w, int h)
{
	//int tx, ty, tw, th;
//GLUI_Master.get_viewport_area (&tx, &ty, &tw, &th );

//glViewport( tx, ty, tw, th );

   //glViewport(0, 0, (GLsizei) w, (GLsizei) h);

GLUI_Master.auto_set_viewport();

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   if (w <= h)
      glOrtho(-4.0, 4.0, -4.0*(GLfloat)h/(GLfloat)w, 
              4.0*(GLfloat)h/(GLfloat)w, -4.0, 4.0);
   else
      glOrtho(-4.0*(GLfloat)w/(GLfloat)h, 
              4.0*(GLfloat)w/(GLfloat)h, -4.0, 4.0, -4.0, 4.0);
   glMatrixMode(GL_MODELVIEW);
   glLoadIdentity();
}

void keyboard(unsigned char key, int x, int y)
{
   switch (key) {
      case 27:
         exit(0);
         break;
   }
}

void myGlutIdle( void ) {
	glutSetWindow(main_window);
	glutPostRedisplay();
}

int main(int argc, char** argv)
{
	//funcHB();
	//funcBH();
char text[100]="dasd";
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   main_window =glutCreateWindow (argv[0]);
   glutDisplayFunc(display);
   //glutReshapeFunc(reshape);
   //glutKeyboardFunc(keyboard);
   init ();

  // GLUI *glui = GLUI_Master.create_glui ("GLUI", 0 );
  GLUI *glui = GLUI_Master.create_glui_subwindow (main_window,GLUI_SUBWINDOW_LEFT);
                                        
  glui->add_statictext( "Simple GLUI Example" );
  glui->add_separator();
  //glui->add_checkbox( "Wireframe", &wireframe, 1, control_cb );
  //GLUI_Spinner *segment_spinner =
    //glui->add_spinner( "Segments:",GLUI_SPINNER_INT, &segments );
  //segment_spinner->set_int_limits( 3, 60, GLUI_LIMIT_WRAP );
  GLUI_EditText *edittext =
      glui->add_edittext( "", GLUI_EDITTEXT_FLOAT, text );
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
  //GLUI_Master.set_glutMouseFunc (myGlutMouse );
  GLUI_Master.set_glutReshapeFunc (reshape );

   glutMainLoop();
   return 0;
}

