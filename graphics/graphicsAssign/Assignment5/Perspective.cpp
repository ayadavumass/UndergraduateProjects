#include"Perspective.h"
#include"CommonFunc.h"
#include"math.h"

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
