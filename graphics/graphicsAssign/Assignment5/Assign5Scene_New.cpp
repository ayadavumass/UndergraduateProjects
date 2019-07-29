

#include <GL/glut.h>
#include<stdio.h>
#include<math.h>
#include <unistd.h>

#define VIEWPOS 1   // can be 0,1,2
#define PART 3      // can be 1 2 3
#define NUMOBJECTS      4
int winWidth=800,winHeight=800;



GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* Red diffuse light. */
//GLfloat light_position[] = {-1.0, -1.0, -1.0, 0.0};  /* Infinite light location. */
GLfloat light_position[] = {4.0, 4.0, 4.0, 1.0};

// parameters
GLfloat blackcolor[]={0.0,0.0,0.0,1.0};
GLfloat whitecolor[]={1.0,1.0,1.0,1.0};
GLfloat diffuseCoeff1[]={1.0,0.0,0.0,1.0};
GLfloat diffuseCoeff2[]={1.0,1.0,1.0,1.0};
GLfloat diffuseCoeff3[]={0.0,0.0,1.0,1.0};
GLfloat diffuseCoeff4[]={0.0,1.0,0.0,1.0};
GLfloat specularCoeff[]={1.0,1.0,1.0,1.0};
GLfloat Shine=100.0;

GLfloat EyePos[3][3]={{0,0.5,2},{2,2,2},{1.0,2.0,0.5}};
GLfloat LookAtPoint[3][3]={{0,0.5,0.0},{0,0.5,0.5},{0.0,0.5,0.5}};
GLfloat ViewUpVector[3][3]={{0.0,1.0,0.0},{0,0,1},{0,0,1}};


GLfloat Obj1Coord[4][3]={{0.0,0.0,0.0},{0.0,1.0,0.0},{0.0,1.0,1.0},{0.0,0.0,1.0}};

GLfloat Obj2Coord[4][3]={{1.0,0.0,0.0},{1.0,1.0,0.0},{1.0,1.0,1.0},{1.0,0.0,1.0}};

GLfloat Obj3aCoord[3][3]={{1.0,0.5,0.5},{2.0,0.5,0.0},{2.0,0.5,1.0}};

GLfloat Obj3bCoord[3][3]={{0.0,0.5,0.5},{1.0,0.5,0.0},{1.0,0.5,1.0}};

GLfloat Obj3cCoord[3][3]={{0.0,1.5,0.5},{0.0,0.5,0.0},{0.0,0.5,1.0}};

GLfloat Obj4aCoord[4][3]={{2.0,0.5,0.0},{2.0,0.5,1.0},{3.0,0.5,1.0},{3.0,0.5,0.0}};

GLfloat Obj4bCoord[4][3]={{1.0,0.5,0.0},{1.0,0.5,1.0},{2.0,0.5,1.0},{2.0,0.5,0.0}};

GLfloat Obj4cCoord[4][3]={{0.0,0.5,0.0},{0.0,0.5,1.0},{1.0,0.5,1.0},{1.0,0.5,0.0}};

GLfloat AverageNormal[NUMOBJECTS][3];





void
drawScene(void)
{

glPushMatrix();       // object 1

glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff1);


//glColor3f (1.0, 0.0, 0.0);
glBegin(GL_POLYGON);
int i=0;
for(i=0;i<4;i++)
    glVertex3fv(Obj1Coord[i]);
glEnd();
glPopMatrix();


glPushMatrix();       //object 2
glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff2);
//glColor3f (1.0, 1.0, 1.0);
glBegin(GL_POLYGON);

  for(i=0;i<4;i++)
  glVertex3fv(Obj2Coord[i]);
glEnd();
glPopMatrix();



glPushMatrix();       //object 3 s1
glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff3);
//glColor3f (0.0, 0.0, 1.0);



glBegin(GL_POLYGON);
  //  for(i=0;i<3;i++)
    //   glVertex3fv(Obj3aCoord[i]);
    if(PART==1)
    {

       for(i=0;i<3;i++)
       glVertex3fv(Obj3aCoord[i]);
    }
    if(PART==2)
    {
       // glTranslatef(-1.0,0.0,0.0);

        for(i=0;i<3;i++)
        glVertex3fv(Obj3bCoord[i]);
    }
    if(PART==3)
    {
      //  glTranslatef(-1.0,0.0,0.0);
        //glRotatef(90,0.0,0.0,1.0);
        //glTranslatef(0.0,0.5,0.5);
         for(i=0;i<3;i++)
        glVertex3fv(Obj3cCoord[i]);
    }
glEnd();
glPopMatrix();



glPushMatrix();       //object 3 s2
glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff4);
//glColor3f (0.0, 1.0, 0.0);
glBegin(GL_POLYGON);
    if(PART==1)
    {
              int i=0;
for(i=0;i<4;i++)
    glVertex3fv(Obj4aCoord[i]);
    }
    if(PART==2)
    {
        int i=0;
        for(i=0;i<4;i++)
        glVertex3fv(Obj4bCoord[i]);
    }
    if(PART==3)
    {
        int i=0;
        for(i=0;i<4;i++)
        glVertex3fv(Obj4cCoord[i]);

    }
glEnd();
glPopMatrix();

}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawScene();        //original
  glFlush();
  glutSwapBuffers();
}

void init()
{
  //  glClearColor(1,1,1,1);

    glEnable(GL_NORMALIZE);
  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);



  glLightfv(GL_LIGHT0,GL_AMBIENT,blackcolor);
  glLightfv(GL_LIGHT0,GL_SPECULAR,whitecolor);


 glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specularCoeff);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Shine);


  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
   glLoadIdentity ();
  gluPerspective( /* field of view in degree */ 120.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */10.0);

   //glFrustum(0, 3, 0.0, 1.0, 1.0, 10.0);

//   glOrtho(0,3,0,1,0,1);

// glOrtho(-0.5,1,-0.5,1,2.0,10.0);






glMatrixMode(GL_MODELVIEW);
glLoadIdentity ();
gluLookAt(EyePos[VIEWPOS][0],EyePos[VIEWPOS][1],EyePos[VIEWPOS][2],
LookAtPoint[VIEWPOS][0],LookAtPoint[VIEWPOS][1],LookAtPoint[VIEWPOS][2],
ViewUpVector[VIEWPOS][0],ViewUpVector[VIEWPOS][1],ViewUpVector[VIEWPOS][2]);



  glShadeModel (GL_SMOOTH);
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (winWidth, winHeight);
  glutCreateWindow("Assignment 5");

   init();
  glutDisplayFunc(display);


  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}

