/*
 * GLUT Shapes Demo
 *
 * Written by Nigel Stewart November 2003
 *
 * This program is test harness for the sphere, cone
 * and torus shapes in GLUT.
 *
 * Spinning wireframe and smooth shaded shapes are
 * displayed until the ESC or q key is pressed.  The
 * number of geometry stacks and slices can be adjusted
 * using the + and - keys.
 */

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define MATRIX_SIZE 4
#define PI 3.14159265
#include <stdlib.h>
#include<math.h>

static int slices = 16;
static int stacks = 16;


//function declarations
void CrossProduct(GLfloat *V1,GLfloat *V2,GLfloat *V3);
void WorldToViewTransform(GLfloat x0,GLfloat y0,GLfloat z0,GLfloat xref,GLfloat yref,GLfloat zref,GLfloat Vx,GLfloat Vy,GLfloat Vz);
void Normalize(GLfloat * N);
void ConvertColumnMajor(GLfloat *PtrSrc,GLfloat *PtrDst);
void myglFrustum(GLfloat xwmin,GLfloat xwmax,GLfloat ywmin,GLfloat ywmax,GLfloat dnear,GLfloat dfar);
void myglPerspective(GLfloat theta,GLfloat aspect,GLfloat dnear,GLfloat dfar);



//drawing functions
void drawBox(void);
void mySolidCube();
//cube data
    //GLfloat light_position[] = {0.0, 0.0, -1.0, 0.0};
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */






/* GLUT callback Handlers */
static void resize(int width, int height)
{
    const float ar = (float) width / (float) height;

    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    //glFrustum(-ar, ar, -1.0, 1.0, 2.0, 100.0);

    myglFrustum(-ar,ar,-1.0,1.0,2.0,100.0);
   // myglFrustum(-ar,ar,-1.0,1.0,2.0,10.0);


    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity() ;
}

static void display(void)
{
    const double t = glutGet(GLUT_ELAPSED_TIME) / 1000.0;
    const double a = t*90.0;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glColor3d(1,0,0);

    glPushMatrix();
        glTranslated(-2.4,1.2,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidSphere(1,slices,stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(0,1.2,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidCone(1,1,slices,stacks);
    glPopMatrix();

    glPushMatrix();
        glTranslated(2.4,1.2,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutSolidTorus(0.2,0.8,slices,stacks);
    glPopMatrix();

    glPushMatrix();
    //    glTranslated(2.4,1.2,-6);
    glTranslatef(0, -1.2, -6.0);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);

        //     //these functions also need to be replaced by ours equivalent functions
  //glRotatef(60, 1.0, 0.0, 0.0);
  //glRotatef(-20, 0.0, 0.0, 1.0);

        mySolidCube();
    glPopMatrix();


    /*glPushMatrix();
        glTranslated(-2.4,-1.2,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutWireSphere(1,slices,stacks);
    glPopMatrix();*/

    /*glPushMatrix();
        glTranslated(0,-1.2,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutWireCone(1,1,slices,stacks);
    glPopMatrix();*/

    /*glPushMatrix();
        glTranslated(2.4,-1.2,-6);
        glRotated(60,1,0,0);
        glRotated(a,0,0,1);
        glutWireTorus(0.2,0.8,slices,stacks);
    glPopMatrix();*/

    glutSwapBuffers();
}


static void key(unsigned char key, int x, int y)
{
    switch (key)
    {
        case 27 :
        case 'q':
            exit(0);
            break;

        case '+':
            slices++;
            stacks++;
            break;

        case '-':
            if (slices>3 && stacks>3)
            {
                slices--;
                stacks--;
            }
            break;
    }

    glutPostRedisplay();
}

static void idle(void)
{
    glutPostRedisplay();
}

const GLfloat light_ambient[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
const GLfloat light_diffuse[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat light_specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//const GLfloat light_position[] = { 2.0f, 5.0f, 5.0f, 0.0f };

const GLfloat light_position[] = { -2.0f, -5.0f, 1.0f, 0.0f };

const GLfloat mat_ambient[]    = { 0.7f, 0.7f, 0.7f, 1.0f };
const GLfloat mat_diffuse[]    = { 0.8f, 0.8f, 0.8f, 1.0f };
const GLfloat mat_specular[]   = { 1.0f, 1.0f, 1.0f, 1.0f };
const GLfloat high_shininess[] = { 100.0f };





/* Program entry point */

int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    glutInitWindowSize(640,480);
    glutInitWindowPosition(10,10);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutCreateWindow("GLUT Shapes");

    glutReshapeFunc(resize);
    glutDisplayFunc(display);
    glutKeyboardFunc(key);
    glutIdleFunc(idle);

    glClearColor(1,1,1,1);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHT0);
    glEnable(GL_NORMALIZE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);

    glLightfv(GL_LIGHT0, GL_AMBIENT,  light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE,  light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glMaterialfv(GL_FRONT, GL_AMBIENT,   mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, high_shininess);







    glutMainLoop();

    return EXIT_SUCCESS;
}


void mySolidCube()
{
 v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

drawBox();

}


void drawBox(void)
{
  int i;


  for (i = 0; i < 6; i++) {

      glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);

    glColor3f (1.0, 0.0, 0.0);
    glVertex3fv(&v[faces[i][0]][0]);

    glColor3f (0.0, 1.0, 0.0);
    glVertex3fv(&v[faces[i][1]][0]);

    glColor3f (0.0, 0.0, 1.0);
    glVertex3fv(&v[faces[i][2]][0]);

    glColor3f (0.0, 1.0, 1.0);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();

  }
 // printf("Normal %lf\n",n[0][0]);
}









/*Library Implementation*/

void CrossProduct(GLfloat *V1,GLfloat *V2,GLfloat *V3)       //evaluates V1xV2 stores in V3
{
    V3[0]=V1[1]*V2[2]-V1[2]*V2[1];
    V3[1]=V1[2]*V2[0]-V1[0]*V2[2];
    V3[2]=V1[0]*V2[1]-V1[1]*V2[0];
}


// equivalent to gluLookAt
void WorldToViewTransform(GLfloat x0,GLfloat y0,GLfloat z0,GLfloat xref,GLfloat yref,GLfloat zref,GLfloat Vx,GLfloat Vy,GLfloat Vz)
{
    GLfloat n[3],u[3],v[3],V[3];
    n[0]=x0-xref;
    n[1]=y0-yref;
    n[2]=z0-zref;
    Normalize(&n[0]);
    V[0]=Vx;
    V[1]=Vy;
    V[2]=Vz;
    CrossProduct(&V[0],&n[0],&u[0]);
    Normalize(&u[0]);
    CrossProduct(&n[0],&u[0],&v[0]);

    GLfloat WToVMatrix[4][4];
    WToVMatrix[0][0]=u[0];
    WToVMatrix[0][1]=u[1];
    WToVMatrix[0][2]=u[2];
    WToVMatrix[0][3]=-x0*u[0]-y0*u[1]-z0*u[2];

    WToVMatrix[1][0]=v[0];
    WToVMatrix[1][1]=v[1];
    WToVMatrix[1][2]=v[2];
    WToVMatrix[1][3]=-x0*v[0]-y0*v[1]-z0*v[2];

    WToVMatrix[2][0]=n[0];
    WToVMatrix[2][1]=n[1];
    WToVMatrix[2][2]=n[2];
    WToVMatrix[2][3]=-x0*n[0]-y0*n[1]-z0*n[2];

    WToVMatrix[3][0]=0;
    WToVMatrix[3][1]=0;
    WToVMatrix[3][2]=0;
    WToVMatrix[3][3]=1;

    GLfloat ColMajor[16];

    ConvertColumnMajor(&WToVMatrix[0][0],ColMajor);
    glMultMatrixf(ColMajor);
}


void Normalize(GLfloat * N)
{
    GLfloat mod=sqrt(pow(N[0],2)+pow(N[1],2)+pow(N[2],2));
    N[0]/=mod;
    N[1]/=mod;
    N[2]/=mod;
}


void ConvertColumnMajor(GLfloat *PtrSrc,GLfloat *PtrDst)
{
	int i=0,j=0;
	for(i=0;i<MATRIX_SIZE;i++)
	{
		for(j=0;j<MATRIX_SIZE;j++)
		{
			*(PtrDst+MATRIX_SIZE*j+i)=*(PtrSrc+MATRIX_SIZE*i+j);
		}
	}
}

void myglFrustum(GLfloat xwmin,GLfloat xwmax,GLfloat ywmin,GLfloat ywmax,GLfloat dnear,GLfloat dfar)
{
    GLfloat FrustumMatrix[4][4];
    GLfloat znear=-dnear;
    GLfloat zfar=-dfar;
    FrustumMatrix[0][0]=(-2*znear)/(xwmax-xwmin);
    FrustumMatrix[0][1]=0;
    FrustumMatrix[0][2]=(xwmax+xwmin)/(xwmax-xwmin);
    FrustumMatrix[0][3]=0;

    FrustumMatrix[1][0]=0;
    FrustumMatrix[1][1]=(-2*znear)/(ywmax-ywmin);
    FrustumMatrix[1][2]=(ywmax+ywmin)/(ywmax-ywmin);
    FrustumMatrix[1][3]=0;

    FrustumMatrix[2][0]=0;
    FrustumMatrix[2][1]=0;
    FrustumMatrix[2][2]=(znear+zfar)/(znear-zfar);
    FrustumMatrix[2][3]=(-2*znear*zfar)/(znear-zfar);

    FrustumMatrix[3][0]=0;
    FrustumMatrix[3][1]=0;
    FrustumMatrix[3][2]=-1;
    FrustumMatrix[3][3]=0;

    GLfloat ColMajor[16];
    ConvertColumnMajor(&FrustumMatrix[0][0],ColMajor);
    glMultMatrixf(ColMajor);
}

void myglPerspective(GLfloat theta,GLfloat aspect,GLfloat dnear,GLfloat dfar)
{
    GLfloat PerspectiveMatrix[4][4];
    GLfloat znear=-dnear;
    GLfloat zfar=-dfar;
    GLfloat cot=1/tan((theta/2)*PI/180);
    PerspectiveMatrix[0][0]=(cot)/(aspect);
    PerspectiveMatrix[0][1]=0;
    PerspectiveMatrix[0][2]=0;
    PerspectiveMatrix[0][3]=0;

    PerspectiveMatrix[1][0]=0;
    PerspectiveMatrix[1][1]=cot;
    PerspectiveMatrix[1][2]=0;
    PerspectiveMatrix[1][3]=0;

    PerspectiveMatrix[2][0]=0;
    PerspectiveMatrix[2][1]=0;
    PerspectiveMatrix[2][2]=(znear+zfar)/(znear-zfar);
    PerspectiveMatrix[2][3]=(-2*znear*zfar)/(znear-zfar);

    PerspectiveMatrix[3][0]=0;
    PerspectiveMatrix[3][1]=0;
    PerspectiveMatrix[3][2]=-1;
    PerspectiveMatrix[3][3]=0;

    GLfloat ColMajor[16];
    ConvertColumnMajor(&PerspectiveMatrix[0][0],ColMajor);
    glMultMatrixf(ColMajor);
}


