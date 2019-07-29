/* Copyright (c) Mark J. Kilgard, 1997. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

/* This program was requested by Patrick Earl; hopefully someone else
   will write the equivalent Direct3D immediate mode program. */

#include <GL/glut.h>
#include<stdio.h>
#include<math.h>
#include"Clipping.h"
#include <unistd.h>
#define MAXPTS  6

#define OR  2         //1 for original 2 our impl



GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {-1.0, -1.0, -1.0, 0.0};  /* Infinite light location. */
//GLfloat light_position[] = {1.0, 1.0, 1.0, 1.0};

//my parameters
GLfloat blackcolor[]={0.0,0.0,0.0,1.0};
GLfloat whitecolor[]={1.0,1.0,1.0,1.0};
GLfloat diffuseCoeff[]={1.0,0.0,0.0,1.0};
GLfloat specularCoeff[]={1.0,1.0,1.0,1.0};
GLfloat Shine=100.0;

GLfloat EyePos[3]={0.0,0.0,5.0};



//GLfloat light_position[] = {0.0, 0.0, -1.0, 0.0};
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */

GLfloat TransformedCoord[8][4];
GLdouble ProjMatrix[4][4];
GLdouble ModelMatrix[4][4];



//function declaratons
void CalculateVertex();
void TranformVertex(GLfloat *Vertex,GLfloat *Output);
void Clipping();


void ModelViewMatrix();


void
drawBox(void)
{
  int i;
glColor3f (1.0, 0.0, 0.0);
  for (i = 0; i < 6; i++)
  //i=4;
  {


    glBegin(GL_POLYGON);
    glNormal3fv(&n[i][0]);
    // glColor3f (1.0, 0.0, 0.0);
     /*ColorStore[faces[i][0]][0]=1.0;
     ColorStore[faces[i][0]][1]=0.0;
     ColorStore[faces[i][0]][2]=0.0;*/

    glVertex3fv(&v[faces[i][0]][0]);
    //glColor3f (0.0, 1.0, 0.0);


     /*ColorStore[faces[i][1]][0]=0.0;
     ColorStore[faces[i][1]][1]=1.0;
     ColorStore[faces[i][1]][2]=0.0;*/

    glVertex3fv(&v[faces[i][1]][0]);


     //glColor3f (0.0, 0.0, 1.0);

     /*ColorStore[faces[i][2]][0]=0.0;
     ColorStore[faces[i][2]][1]=0.0;
     ColorStore[faces[i][2]][2]=1.0;*/

    glVertex3fv(&v[faces[i][2]][0]);
   //  glColor3f (0.0, 1.0, 1.0);

     /*ColorStore[faces[i][3]][0]=0.0;
     ColorStore[faces[i][3]][1]=1.0;
     ColorStore[faces[i][3]][2]=1.0;*/
     glVertex3fv(&v[faces[i][3]][0]);






    glEnd();
  }
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  //drawBox();        //original
 // glColor3f (1.0, 1.0, 0.0);
  //GetModelViewMatrix();
//sleep(2);
Clipping();   //clipped

    glutSwapBuffers();
}

void init()
{
  /* Setup cube vertex data. */
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;



glEnable(GL_NORMALIZE);
  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);



  glLightfv(GL_LIGHT0,GL_AMBIENT,blackcolor);
  glLightfv(GL_LIGHT0,GL_SPECULAR,whitecolor);

  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specularCoeff);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Shine);



 // glEnable(GL_LIGHT0);
  //glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
 // gluPerspective( /* field of view in degree */ 40.0,
  //  /* aspect ratio */ 1.0,
  //  /* Z near */ 3.0, /* Z far */ 5.0);
   glLoadIdentity ();

  glOrtho(-2,2,-2,2,4.5,6.4);






glMatrixMode(GL_MODELVIEW);
glLoadIdentity();
/*glLoadIdentity();
glTranslatef(0.25,0.35,0);
ModelViewMatrix();
glPopMatrix();*/


  gluLookAt(EyePos[0], EyePos[1], EyePos[2],  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.0);      /* up is in positive Y direction */

  /* Adjust cube position to be asthetic angle. */

  //glRotatef(45, 1.0, 0.0, 1.0);
  //glRotatef(-40, 0.0,3 0.0, 1.0);
 // glTranslatef(0.25,0.35,0);
  //glScalef(0.5,0.5,0.5);

  CalculateVertex();

  glShadeModel (GL_SMOOTH);
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("red 3D lighted cube");
   init();
  glutDisplayFunc(display);

  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}



void Clipping()
{
    printf("New Clipped Vertex\n");
    int i=0;
    GLfloat TriangVertex1[4],TriangVertex2[4],TriangVertex3[4];
    GLfloat Output1[MAXPTS][4];
    GLfloat Output2[MAXPTS][4];
    GLint Num1;
    GLint Num2;
glColor3f (1.0, 1.0, 0.0);
    for(i=0;i<6;i++)
   //i=4;
    {
        //v[faces[i][0]][0]    triangulation algo will implemented later
        //TriangVertex1[0]=TransformedCoord[faces[i][0]][0];





        Suther_Hodg2D(&TransformedCoord[faces[i][0]][0],&TransformedCoord[faces[i][1]][0],&TransformedCoord[faces[i][2]][0],&Output1[0][0],&Num1);
       glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();     // glvertex again multiplies with model view matrix, which we don't want, jugaad
       glMatrixMode(GL_PROJECTION);        // glvertex again multiplies with projection matrix, which we don't want, jugaad
       glLoadIdentity();
       /* glPointSize(5);
        glBegin(GL_POINTS);
            glVertex3fv(&TransformedCoord[faces[i][0]][0]);
            glVertex3fv(&TransformedCoord[faces[i][1]][0]);
            glVertex3fv(&TransformedCoord[faces[i][2]][0]);

            //printf("Vertex %d %lf %lf %lf %lf\n",j,Output2[j][0],Output2[j][1],Output2[j][2],Output2[j][3]);
        glEnd();*/



        int j=0;
        glPointSize(5);
        glBegin(GL_POLYGON);
        for(j=0;j<Num1;j++)
        {

          //   glBegin(GL_POINTS);
          //   glBegin(GL_POLYGON);
             glVertex3fv(&Output1[j][0]);
            printf("Vertex %d %lf %lf %lf %lf\n",j,Output1[j][0],Output1[j][1],Output1[j][2],Output1[j][3]);
        //     glEnd();

        }
        glEnd();

        Suther_Hodg2D(&TransformedCoord[faces[i][0]][0],&TransformedCoord[faces[i][2]][0],&TransformedCoord[faces[i][3]][0],&Output2[0][0],&Num2);


        /*glPointSize(5);
        glBegin(GL_POINTS);
            glVertex3fv(&TransformedCoord[faces[i][0]][0]);
            glVertex3fv(&TransformedCoord[faces[i][2]][0]);
            glVertex3fv(&TransformedCoord[faces[i][3]][0]);

            //printf("Vertex %d %lf %lf %lf %lf\n",j,Output2[j][0],Output2[j][1],Output2[j][2],Output2[j][3]);
        glEnd();*/


        glPointSize(5);
         glBegin(GL_POLYGON);
        for(j=0;j<Num2;j++)
        {

             //glBegin(GL_POINTS);
             glVertex3fv(&Output2[j][0]);
             printf("Vertex %d %lf %lf %lf %lf\n",j,Output2[j][0],Output2[j][1],Output2[j][2],Output2[j][3]);
             //glEnd();

        }
         glEnd();




    }
}

void CalculateVertex()
{
    printf("TRANSFORMED VERTEX\n");
    glGetDoublev(GL_PROJECTION_MATRIX,&ProjMatrix[0][0]);
    glGetDoublev(GL_MODELVIEW_MATRIX,&ModelMatrix[0][0]);

    int i=0;
    for(i=0;i<8;i++)
    {
          GLfloat Vertex0[4];
          GLfloat Output0[4];
          Vertex0[0]=v[i][0];
          Vertex0[1]=v[i][1];
          Vertex0[2]=v[i][2];
          Vertex0[3]=1;
          TranformVertex(Vertex0,Output0);

          TransformedCoord[i][0]=Output0[0];
          TransformedCoord[i][1]=Output0[1];
          TransformedCoord[i][2]=Output0[2];
          TransformedCoord[i][3]=Output0[3];

          printf("Vertex %d %lf %lf %lf %lf\n",i,TransformedCoord[i][0],TransformedCoord[i][1],TransformedCoord[i][2],TransformedCoord[i][3]);
    }
    printf("\n\n");
}

void TranformVertex(GLfloat *Vertex,GLfloat *Output)
{
    int i=0,j=0;
    GLfloat Temp1[4];

    for(i=0;i<4;i++)
    {
        Temp1[i]=0;
        for(j=0;j<4;j++)
        {
            Temp1[i]+=ModelMatrix[j][i]*Vertex[j];      //in column major form

        }
        //Output[i]=Temp1[i];
    }

    for(i=0;i<4;i++)
    {
        Output[i]=0;
        for(j=0;j<4;j++)
        {
            Output[i]+=ProjMatrix[j][i]*Temp1[j];      //in column major form

        }
    }
}


void ModelViewMatrix()
{
     glGetDoublev(GL_MODELVIEW_MATRIX,&ModelMatrix[0][0]);
     int i=0,j=0;
     for(i=0;i<4;i++)
     {
         for(j=0;j<4;j++)
         {
             printf("%lf ",ModelMatrix[i][j]);

         }
         printf("\n");
     }
}



