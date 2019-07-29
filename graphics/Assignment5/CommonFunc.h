#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef COMMONFUNC_H_INCLUDED
#define COMMONFUNC_H_INCLUDED
#define PI 3.14159265
#define MATRIX_SIZE 4
 void CrossProduct(GLdouble *V1,GLdouble *V2,GLdouble *V3);

 void Normalize(GLdouble * N);
 void ConvertColumnMajor(GLfloat *PtrSrc,GLfloat *PtrDst);
 void DotProduct(GLfloat *V1,GLfloat *V2,GLfloat *Output);
 static void idle(void);
 void Normalize(GLfloat * N);
 void CrossProduct(GLfloat *V1,GLfloat *V2,GLfloat *V3);       //evaluates V1xV2 stores in V3

#endif // COMMONFUNC_H_INCLUDED
