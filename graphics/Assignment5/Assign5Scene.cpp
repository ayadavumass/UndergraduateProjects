

#include <GL/glut.h>
#include<stdio.h>
#include<math.h>
#include <unistd.h>

#define VIEWPOS 1   // can be 0,1,2
#define PART 3      // can be 1 2 3
int winWidth=800,winHeight=800;



GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* Red diffuse light. */
//GLfloat light_position[] = {-1.0, -1.0, -1.0, 0.0};  /* Infinite light location. */
GLfloat light_position[] = {4.0, 4.0, 4.0, 1.0};

//my parameters
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











void
drawScene(void)
{

glPushMatrix();       // object 1

glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff1);


//glColor3f (1.0, 0.0, 0.0);
glBegin(GL_POLYGON);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(0.0,1.0,1.0);
    glVertex3f(0.0,0.0,1.0);
glEnd();
glPopMatrix();


glPushMatrix();       //object 2
glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff2);
//glColor3f (1.0, 1.0, 1.0);
glBegin(GL_POLYGON);
    glVertex3f(1.0,0.0,0.0);
    glVertex3f(1.0,1.0,0.0);
    glVertex3f(1.0,1.0,1.0);
    glVertex3f(1.0,0.0,1.0);
glEnd();
glPopMatrix();



glPushMatrix();       //object 3 s1
glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff3);
//glColor3f (0.0, 0.0, 1.0);
glBegin(GL_POLYGON);
    if(PART==1)
    {
        glVertex3f(1.0,0.5,0.5);
        glVertex3f(2.0,0.5,0.0);
        glVertex3f(2.0,0.5,1.0);
    }
    if(PART==2)
    {
        glVertex3f(0.0,0.5,0.5);
        glVertex3f(1.0,0.5,0.0);
        glVertex3f(1.0,0.5,1.0);
    }
    if(PART==3)
    {
        glVertex3f(0.0,1.5,0.5);
        glVertex3f(0.0,0.5,0.0);
        glVertex3f(0.0,0.5,1.0);

    }
glEnd();
glPopMatrix();



glPushMatrix();       //object 3 s2
glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff4);
//glColor3f (0.0, 1.0, 0.0);
glBegin(GL_POLYGON);
    if(PART==1)
    {
        glVertex3f(2.0,0.5,0.0);
        glVertex3f(2.0,0.5,1.0);
        glVertex3f(3.0,0.5,1.0);
        glVertex3f(3.0,0.5,0.0);
    }
    if(PART==2)
    {
        glVertex3f(1.0,0.5,0.0);
        glVertex3f(1.0,0.5,1.0);
        glVertex3f(2.0,0.5,1.0);
        glVertex3f(2.0,0.5,0.0);
    }
    if(PART==3)
    {
        glVertex3f(0.0,0.5,0.0);
        glVertex3f(0.0,0.5,1.0);
        glVertex3f(1.0,0.5,1.0);
        glVertex3f(1.0,0.5,0.0);

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

