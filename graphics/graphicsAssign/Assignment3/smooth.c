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

//Global Varaibles
GLfloat Color[3][3]={{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,0.0,1.0}};
GLfloat Points[3][2]={{5.0,5.0},{25.0,5.0},{5.0,25.0}};
GLint winWidth=500;
GLint winHeight=500;
GLfloat Buffer[500][500][3];


//function declarations
void myTriangle();
void myGouroud(GLfloat *LB,GLfloat *LT,GLfloat *RB,GLfloat *RT,GLint min,GLint max);


void init(void)
{
   glClearColor (0.0, 0.0, 0.0, 0.0);
   glShadeModel (GL_SMOOTH);
}


void triangle(void)
{
   glBegin (GL_QUADS);
   glColor3f (1.0, 0.0, 0.0);
   glVertex3f (5.0, 5.0,0.0);
   glColor3f (0.0, 1.0, 0.0);
   glVertex3f (25.0, 5.0,0.0);
   glColor3f (0.0, 0.0, 1.0);
   glVertex3f (25.0, 25.0,0.0);
   glColor3f (0.0, 1.0, 1.0);
   glVertex3f (5.0, 25.0,0.0);

   glEnd();
}

void display(void)
{
   glClear (GL_COLOR_BUFFER_BIT);
   triangle ();
  //myTriangle();




    glReadPixels(0,0,winWidth,winHeight,GL_RGB,GL_FLOAT,Buffer);          //reads frame buffer


    printf("Display\n");

    //glDrawPixels(winWidth,winHeight,GL_RGB,GL_FLOAT,Buffer);




    GLdouble ProjMatrix[16];

    glGetDoublev(GL_PROJECTION_MATRIX,ProjMatrix);

    GLdouble ModelMatrix[16];

    glGetDoublev(GL_MODELVIEW_MATRIX,ModelMatrix);

    GLint ViewPort[4];

    glGetIntegerv(GL_VIEWPORT,ViewPort);

    GLdouble ObjCoord[3]={5.0,5.0,0.0};
    GLdouble WinCoord[3];

    /*gluProject(ObjCoord[0],ObjCoord[1],ObjCoord[2],ModelMatrix,ProjMatrix,ViewPort,&WinCoord[0],&WinCoord[1],&WinCoord[2]);

    printf("%lf,%lf,%lf\n",WinCoord[0],WinCoord[1],WinCoord[2]);
*/

    ObjCoord[1]=25.0;

     gluProject(ObjCoord[0],ObjCoord[1],ObjCoord[2],ModelMatrix,ProjMatrix,ViewPort,&WinCoord[0],&WinCoord[1],&WinCoord[2]);

    printf("%lf,%lf,%lf\n",WinCoord[0],WinCoord[1],WinCoord[2]);   //gives square pixels values, its is same in x and y


     /*ObjCoord[0]=25.0;
     ObjCoord[1]=5.0;


     gluProject(ObjCoord[0],ObjCoord[1],ObjCoord[2],ModelMatrix,ProjMatrix,ViewPort,&WinCoord[0],&WinCoord[1],&WinCoord[2]);
*/
    //printf("%lf,%lf,%lf\n",WinCoord[0],WinCoord[1],WinCoord[2]);

    int min=(int)WinCoord[0];
    int max=(int)WinCoord[1];
    /*int i=0,j=0;
    for(i=min;i<=max;i++)
    {
        for(j=min;j<=max;j++)
        {
            Buffer[j][i][0]=1;
            Buffer[j][i][1]=0;
            Buffer[j][i][2]=0;

        }
    }*/
    GLfloat LB[3]={1.0,0.0,0.0},LT[3]={0.0,1.0,1.0},RB[3]={0.0,1.0,0.0},RT[3]={0.0,0.0,1.0};


sleep(5);       //initial goroud then our goroud

    myGouroud(LB,LT,RB,RT,min,max);

 glRasterPos2i(0,0);


glDrawPixels(winWidth,winHeight,GL_RGB,GL_FLOAT,Buffer);     //stores changed values in frmae buffer













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

void myGouroud(GLfloat *LB,GLfloat *LT,GLfloat *RB,GLfloat *RT,GLint min,GLint max)
{
    int i=0,j=0;
    GLfloat LQ[3],RQ[3],HP[3];

    LQ[0]=LB[0];
    LQ[1]=LB[1];
    LQ[2]=LB[2];

    RQ[0]=RB[0];
    RQ[1]=RB[1];
    RQ[2]=RB[2];

    for(i=min;i<=max;i++)
    {
        if(i!=min)
        {
            LQ[0]+=(LT[0]-LB[0])/fabs(max-min);
            LQ[1]+=(LT[1]-LB[1])/fabs(max-min);
            LQ[2]+=(LT[2]-LB[2])/fabs(max-min);

            RQ[0]+=(RT[0]-RB[0])/fabs(max-min);
            RQ[1]+=(RT[1]-RB[1])/fabs(max-min);
            RQ[2]+=(RT[2]-RB[2])/fabs(max-min);
        }
        HP[0]=LQ[0];
        HP[1]=LQ[1];
        HP[2]=LQ[2];


        for(j=min;j<=max;j++)
        {
            if(j!=min)
            {
                HP[0]+=(RQ[0]-LQ[0])/fabs(max-min);
                HP[1]+=(RQ[1]-LQ[1])/fabs(max-min);
                HP[2]+=(RQ[2]-LQ[2])/fabs(max-min);
            }

            Buffer[j][i][0]=HP[0];
            Buffer[j][i][1]=HP[1];
            Buffer[j][i][2]=HP[2];

        }
    }
}


void myTriangle()
{

    int i=5,j=5;
    glBegin (GL_POINTS);
    glColor3f (1.0, 0.0, 0.0);
    for(i=5;i<=25;i++)
    {
        for(j=5;j<=25;j++)
        {
             glVertex2f (i, j);
        }
    }
    glEnd();

}
