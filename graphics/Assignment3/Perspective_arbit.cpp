#include<GL/glut.h>
#include<math.h>
#include<stdio.h>

#include <GL/gl.h>





#define MATRIX_SIZE 4
#define PI 3.14159265

//function decalaration
void CrossProduct(GLfloat *V1,GLfloat *V2,GLfloat *V3);
void WorldToViewTransform(GLfloat x0,GLfloat y0,GLfloat z0,GLfloat xref,GLfloat yref,GLfloat zref,GLfloat Vx,GLfloat Vy,GLfloat Vz);
void Normalize(GLfloat * N);
void ConvertColumnMajor(GLfloat *PtrSrc,GLfloat *PtrDst);
void myglFrustum(GLfloat xwmin,GLfloat xwmax,GLfloat ywmin,GLfloat ywmax,GLfloat dnear,GLfloat dfar);
void myglPerspective(GLfloat theta,GLfloat aspect,GLfloat dnear,GLfloat dfar);
void cubedisplay (void);
void myinit (void);

//global varibales
GLint winWidth=600,winHeight=600;
GLfloat x0=100.0,Y0=50.0,z0=50.0;
GLfloat xref=50.0,yref=50.0,zref=0.0;
GLfloat Vx=0.0,Vy=1.0,Vz=0.0;

GLfloat xwMin=-40.0,ywMin=-60.0,xwMax=40.0,ywMax=60.0;

GLfloat dnear =25.0,dfar=125.0;

void init()
{
    //glClearColor(1.0,1.0,1.0,0.0);

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glMatrixMode(GL_MODELVIEW);
    //gluLookAt(x0,Y0,z0,xref,yref,zref,Vx,Vy,Vz);
  WorldToViewTransform(x0,Y0,z0,xref,yref,zref,Vx,Vy,Vz);

    glMatrixMode(GL_PROJECTION);
    //glFrustum(xwMin,xwMax,ywMin,ywMax,dnear,dfar);
    myglFrustum(xwMin,xwMax,ywMin,ywMax,dnear,dfar);

     glShadeModel (GL_SMOOTH);

}

void displayFcn()
{
    glClear(GL_COLOR_BUFFER_BIT);




    glColor3f(0.0,1.0,0.0);
    glPolygonMode(GL_FRONT,GL_FILL);
    glPolygonMode(GL_BACK,GL_LINE);
    glBegin(GL_QUADS);
     glColor3f (1.0, 0.0, 0.0);
        glVertex3f(0.0,0.0,0.0);
        glColor3f (0.0, 1.0, 0.0);
        glVertex3f(100.0,0.0,0.0);
         glColor3f (0.0, 0.0, 1.0);
        glVertex3f(100.0,100.0,0.0);
        glColor3f (0.0, 1.0, 1.0);
        glVertex3f(0.0,100.0,0.0);
    glEnd();

    GLfloat Buffer[winWidth][winHeight][3];

    glReadPixels(0,0,winWidth,winHeight,GL_RGB,GL_FLOAT,Buffer);

    glRasterPos2i(-100,-100);

    printf("Display\n");

    glDrawPixels(winWidth,winHeight,GL_RGB,GL_FLOAT,Buffer);


  glutSwapBuffers();


//    glFlush();

}


void reshapeFcn(GLint newWidth,GLint newHeight)
{
    glViewport(0,0,newWidth,newHeight);
    winWidth=newWidth;
    winHeight=newHeight;
  //  glutPostRedisplay();
}

int main(int argc,char **argv)
{
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_SINGLE|GLUT_RGB);
    glutInitWindowPosition(50,50);
    glutInitWindowSize(winWidth,winHeight);
    glutCreateWindow("Perspective");

    init();
    glutDisplayFunc(displayFcn);
    glutReshapeFunc(reshapeFcn);
    glutMainLoop();
    return 0;
}

void CrossProduct(GLfloat *V1,GLfloat *V2,GLfloat *V3)
{
    V3[0]=V1[1]*V2[2]-V1[2]*V2[1];
    V3[1]=V1[2]*V2[0]-V1[0]*V2[2];
    V3[2]=V1[0]*V2[1]-V1[1]*V2[0];
}

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

