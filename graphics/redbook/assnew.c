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

typedef struct point
{
  float x;
  float y;
  float z;
}point;

/*GLfloat ctrlpoints[4][4][3] = {
   {{-1.5, -1.5, 4.0}, {-0.5, -1.5, 2.0}, 
    {0.5, -1.5, -1.0}, {1.5, -1.5, 2.0}}, 
   {{-1.5, -0.5, 1.0}, {-0.5, -0.5, 3.0}, 
    {0.5, -0.5, 0.0}, {1.5, -0.5, -1.0}}, 
   {{-1.5, 0.5, 4.0}, {-0.5, 0.5, 0.0}, 
    {0.5, 0.5, 3.0}, {1.5, 0.5, 4.0}}, 
   {{-1.5, 1.5, -2.0}, {-0.5, 1.5, -2.0}, 
    {0.5, 1.5, 0.0}, {1.5, 1.5, -1.0}}
};*/

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


/*point ctrlpoints[4][4]={
  {{0.0, 0.0, 2.0},  {1.0, 0.0, 1.0}, {2.0, 0.0, 1.0}, {3.0, 0.0, 0.0}},
 {{0.0, 1.0, 1.0},  {1.0, 1.0, 2.0}, {2.0, 1.0, 2.0}, {3.0, 1.0, 1.0}},  
 {{0.0, 2.0, 1.0},  {1.0, 2.0, 2.0}, {2.0, 2.0, 2.0}, {3.0, 2.0, 1.0}},
  {{0.0, 3.0, 0.0},  {1.0, 3.0, 1.0}, {2.0, 3.0, 1.0}, {3.0, 3.0, 0.0}}
};*/

/*point ctrlpoints[4][4]={
  {{0.0, 0.0, -4.0},  {1.0, 0.0, -4.0}, {2.0, 0.0, -4.0}, {3.0, 0.0, -4.0}},
 {{0.0, 1.0, -4.0},  {1.0, 1.0, 1.0}, {2.0, 1.0, 1.0}, {3.0, 1.0, -4.0}},  
 {{0.0, 2.0, -4.0},  {1.0, 2.0, 1.0}, {2.0, 2.0, 1.0}, {3.0, 2.0, -4.0}},
  {{0.0, 3.0, -4.0},  {1.0, 3.0, -4.0}, {2.0, 3.0, -4.0}, {3.0, 3.0, -4.0}}
};*/

point ctrlpoints[4][4]={
  {{0.0, 0.0, 4.0},  {1.0, 0.0, 4.0}, {2.0, 0.0, 4.0}, {3.0, 0.0, 4.0}},
 {{0.0, 1.0, 4.0},  {1.0, 1.0, -4.0}, {2.0, 1.0, -4.0}, {3.0, 1.0, 4.0}},  
 {{0.0, 2.0, 4.0},  {1.0, 2.0, -4.0}, {2.0, 2.0, -4.0}, {3.0, 2.0, 4.0}},
  {{0.0, 3.0, 4.0},  {1.0, 3.0, 4.0}, {2.0, 3.0, 4.0}, {3.0, 3.0, 4.0}}
};


/*point ctrlpoints[4][4]={
  {{0.0, 0.0, 4.0},  {-1.0, 0.0, 4.0}, {-2.0, 0.0, 4.0}, {-3.0, 0.0, 4.0}},
 {{0.0, 1.0, 4.0},  {-1.0, 1.0, 0.0}, {-2.0, 1.0, 0.0}, {-3.0, 1.0, 4.0}},  
 {{0.0, 2.0, 4.0},  {-1.0, 2.0, 0.0}, {-2.0, 2.0, 0.0}, {-3.0, 2.0, 4.0}},
  {{0.0, 3.0, 4.0},  {-1.0, 3.0, 4.0}, {-2.0, 3.0, 4.0}, {-3.0, 3.0, 4.0}}
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








void display(void)
{
   int i, j;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glColor3f(1.0, 1.0, 1.0);
   glPushMatrix ();
   glRotatef(90.0, 0.2, 0.2, 0.2);
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
		glVertex3fv(&p);
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
		glVertex3fv(&p);
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
         glVertex3fv(&ctrlpoints[i][j]);
   glEnd();
   glFlush();
}

void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4,
           0, 1, 12, 4, &ctrlpoints[0][0]);
   glEnable(GL_MAP2_VERTEX_3);
   glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0);
   glEnable(GL_DEPTH_TEST);
   glShadeModel(GL_FLAT);
}

void reshape(int w, int h)
{
   glViewport(0, 0, (GLsizei) w, (GLsizei) h);
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

int main(int argc, char** argv)
{
	

   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
   glutInitWindowSize (500, 500);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}

