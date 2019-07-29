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

/*
 * smooth.c
 * This program demonstrates smooth shading.
 * A smooth shaded polygon is drawn in a 2-D projection.
 */
#include <GL/glut.h>
#include <stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<math.h>
#include"Bresenham.h"

//Global Varaibles
GLfloat Color[3][3]={{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
GLfloat Points[4][3]={{5.0,5.0,0.0},{15.0,5.0,0.0},{15.0,15.0,0.0},{5.0,15.0,0.0}};

GLfloat FB[500][500][3];
GLdouble Pix_Origin_Obj[3];


//Function declarations
void Scan_Conv_Bresen();



void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
}


void triangle(void)
{
   glBegin (GL_LINE_LOOP);
   glColor3f (1.0, 0.0, 0.0);
   glVertex3fv (Points[0]);
   glColor3f (0.0, 1.0, 0.0);
   glVertex3fv (Points[1]);
   glColor3f (0.0, 0.0, 1.0);
   glVertex3fv (Points[2]);
   glColor3f (0.0, 1.0, 1.0);
   glVertex3fv (Points[3]);

   glEnd();
   /*glBegin(GL_LINE);
glColor3f (1.0, 0.0, 0.0);
   glVertex3f (5.0, 5.0,0.0);
   glColor3f (0.0, 1.0, 0.0);
   glVertex3f (25.0, 5.0,0.0);
   glEnd();*/
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
    glLoadIdentity();
    glRotatef(10,0.0,0.0,1.0);
   triangle ();
  //myTriangle();




    glReadPixels(0,0,winWidth,winHeight,GL_RGB,GL_FLOAT,FB);          //reads frame buffer


    printf("Display\n");

    //glDrawPixels(winWidth,winHeight,GL_RGB,GL_FLOAT,Buffer);








sleep(5);       //initial goroud then our goroud

 //   myGouroud(LB,LT,RB,RT,min,max);
Scan_Conv_Bresen();

int OriX=Pix_Origin_Obj[0];
    int OriY=Pix_Origin_Obj[1];
 /*glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();*/

    glRasterPos2i(OriX,OriY);

// glRasterPos2i(0,0);


glDrawPixels(winWidth,winHeight,GL_RGB,GL_FLOAT,FB);     //stores changed values in frmae buffer













  glutSwapBuffers();
   //glFlush ();
}

void reshape (int w, int h)
{
   glViewport (0, 0, (GLsizei) w, (GLsizei) h);
   glMatrixMode (GL_PROJECTION);
   glLoadIdentity ();
   if (w <= h)
      gluOrtho2D (0.0, 30.0, 0.0, 30.0 * (GLfloat) h/(GLfloat) w);
   else
      gluOrtho2D (0.0, 30.0 * (GLfloat) w/(GLfloat) h, 0.0, 30.0);
   glMatrixMode(GL_MODELVIEW);
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
   glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
   glutInitWindowSize (winWidth, winHeight);
   glutInitWindowPosition (100, 100);
   glutCreateWindow (argv[0]);
   init ();
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutKeyboardFunc (keyboard);
   glutMainLoop();
   return 0;
}


void Scan_Conv_Bresen()
{
    GLdouble ProjMatrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX,ProjMatrix);

    GLdouble ModelMatrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX,ModelMatrix);

    GLint ViewPort[4];
    glGetIntegerv(GL_VIEWPORT,ViewPort);

     GLdouble ObjCoord1[3],WinCoord1[3],ObjCoord2[3],WinCoord2[3];


      gluUnProject(0,0,0,ModelMatrix,ProjMatrix,ViewPort,&Pix_Origin_Obj[0],&Pix_Origin_Obj[1],&Pix_Origin_Obj[2]);

       printf("Origin %lf,%lf,%lf\n",Pix_Origin_Obj[0],Pix_Origin_Obj[1],Pix_Origin_Obj[2]);   //gives square pixels values, its is same in x and y





    int i=0,j=0;

  //   GLfloat *FB=(GLfloat *)malloc(sizeof(GLfloat)*winWidth*winHeight*3);
     //glReadPixels(0,0,winWidth,winHeight,GL_RGB,GL_FLOAT,FB);          //reads frame buffer

    //for(i=0;i<6;i++)
    i=4;
    {
        for(j=0;j<4;j++)
        {
      /*  ObjCoord1[0]=v[faces[i][j]][0];
        ObjCoord1[1]=v[faces[i][j]][1];
        ObjCoord1[2]=v[faces[i][j]][2];


        ObjCoord2[0]=v[faces[i][(j+1)%4]][0];
        ObjCoord2[1]=v[faces[i][(j+1)%4]][1];
        ObjCoord2[2]=v[faces[i][(j+1)%4]][2];*/

        ObjCoord1[0]=Points[j%4][0];
        ObjCoord1[1]=Points[j%4][1];
        ObjCoord1[2]=Points[j%4][2];


        ObjCoord2[0]=Points[(j+1)%4][0];
        ObjCoord2[1]=Points[(j+1)%4][1];
        ObjCoord2[2]=Points[(j+1)%4][2];

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
  //  int OriX=Pix_Origin_Obj[0];
   // int OriY=Pix_Origin_Obj[1];
 /*glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();*/

    /*glRasterPos2i(OriX,OriY);
     glDrawPixels(winWidth,winHeight,GL_RGB,GL_FLOAT,FB);     //stores changed values in frmae buffer
glutSwapBuffers();*/


}

