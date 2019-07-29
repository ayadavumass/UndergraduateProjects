
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
#include<stdio.h>
#include "Bresenham.h"
#include<math.h>
#include<unistd.h>

//GLfloat light_position[] = {0.0, 0.0, -1.0, 0.0};
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */

GLfloat FB[winWidth][winHeight][3];

GLdouble ModelMatrix[16];



// function declaration
void Scan_Conv_Bresen();


void drawBox(void)
{
  int i;
  glPushMatrix();
  glRotatef(45,1.0,0.0,1.0);


  glGetDoublev(GL_MODELVIEW_MATRIX,ModelMatrix);      // calculating here, others are calculated in Scan COnversion Fnc

glColor3f (0.0, 1.0, 0.0);
  for (i = 0; i < 6; i++)
  //i=4;
  {

    glBegin(GL_LINE_LOOP);
    glNormal3fv(&n[i][0]);


    glVertex3fv(&v[faces[i][0]][0]);



    glVertex3fv(&v[faces[i][1]][0]);



    glVertex3fv(&v[faces[i][2]][0]);

     glVertex3fv(&v[faces[i][3]][0]);
     glEnd();
  }
  glPopMatrix();
}






void display(void)
{
   int i, j;

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // glColor3f(1.0, 1.0, 0.0);
  // glRotatef(45,1.0,0.0,1.0);
//   glutWireCube(2);
//glLineWidth(10);
glLoadIdentity();
//
//glRotatef(40,1.0,0.0,1.0);
drawBox();
glFlush();
sleep(5);
 Scan_Conv_Bresen();


   glFlush();
}

void init(void)
{
    /* Setup cube vertex data. */
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;


   glClearColor (0.0, 0.0, 0.0, 0.0);
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


   //glLoadIdentity();
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
   glutInitWindowSize (winWidth, winHeight);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutMainLoop();
   return 0;
}


void Scan_Conv_Bresen()
{
    GLdouble ProjMatrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX,ProjMatrix);

    GLdouble ModelMatrix_Ori[16];
    glGetDoublev(GL_MODELVIEW_MATRIX,ModelMatrix_Ori);

    GLint ViewPort[4];
    glGetIntegerv(GL_VIEWPORT,ViewPort);

     GLdouble ObjCoord1[3],WinCoord1[3],ObjCoord2[3],WinCoord2[3];
     GLdouble Pix_Origin_Obj[3];

      gluUnProject(0,0,0.1,ModelMatrix_Ori,ProjMatrix,ViewPort,&Pix_Origin_Obj[0],&Pix_Origin_Obj[1],&Pix_Origin_Obj[2]);

       printf("Origin %lf,%lf,%lf\n",Pix_Origin_Obj[0],Pix_Origin_Obj[1],Pix_Origin_Obj[2]);   //gives square pixels values, its is same in x and y





    int i=0,j=0;

  //   GLfloat *FB=(GLfloat *)malloc(sizeof(GLfloat)*winWidth*winHeight*3);
     glReadPixels(0,0,winWidth,winHeight,GL_RGB,GL_FLOAT,FB);          //reads frame buffer

    for(i=0;i<6;i++)
    //i=4;
    {
        for(j=0;j<4;j++)
        {
        ObjCoord1[0]=v[faces[i][j]][0];
        ObjCoord1[1]=v[faces[i][j]][1];
        ObjCoord1[2]=v[faces[i][j]][2];


        ObjCoord2[0]=v[faces[i][(j+1)%4]][0];
        ObjCoord2[1]=v[faces[i][(j+1)%4]][1];
        ObjCoord2[2]=v[faces[i][(j+1)%4]][2];

        /*gluProject(ObjCoord[0],ObjCoord[1],ObjCoord[2],ModelMatrix,ProjMatrix,ViewPort,&WinCoord[0],&WinCoord[1],&WinCoord[2]);
        printf("%lf,%lf,%lf\n",WinCoord[0],WinCoord[1],WinCoord[2]);
        */


        gluProject(ObjCoord1[0],ObjCoord1[1],ObjCoord1[2],ModelMatrix,ProjMatrix,ViewPort,&WinCoord1[0],&WinCoord1[1],&WinCoord1[2]);
        printf("%lf,%lf,%lf\n",WinCoord1[0],WinCoord1[1],WinCoord1[2]);   //gives square pixels values, its is same in x and y

        gluProject(ObjCoord2[0],ObjCoord2[1],ObjCoord2[2],ModelMatrix,ProjMatrix,ViewPort,&WinCoord2[0],&WinCoord2[1],&WinCoord2[2]);
        printf("%lf,%lf,%lf\n",WinCoord2[0],WinCoord2[1],WinCoord2[2]);   //gives square pixels values, its is same in x and y

        Bresenham_Line(WinCoord1, WinCoord2);


        }
          printf("\n\n");
    }

     //glRasterPos3fv((GLfloat *)Pix_Origin_Obj);
    //int OriX=round(Pix_Origin_Obj[0]);
   // int OriY=round(Pix_Origin_Obj[1]);

    int OriX=Pix_Origin_Obj[0];
    int OriY=Pix_Origin_Obj[1];
 /*glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();*/
printf("Origin X %d %d\n",OriX,OriY);
  //  glRasterPos2i(OriX,OriY);
  glRasterPos2d(Pix_Origin_Obj[0],Pix_Origin_Obj[1]);

     glDrawPixels(winWidth,winHeight,GL_RGB,GL_FLOAT,FB);     //stores changed values in frmae buffer
glutSwapBuffers();


}
