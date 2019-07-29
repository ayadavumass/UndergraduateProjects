#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef CLIPPING_H_INCLUDED
#define CLIPPING_H_INCLUDED
void Suther_Hodg2D(GLfloat * TriangVertex1,GLfloat * TriangVertex2,GLfloat * TriangVertex3,GLfloat *Output,GLint *NumVertices);    //in anticlockwise order
void Inside_Outside(GLint j,GLfloat *Vertex1,GLfloat *Vertex2,GLint *Ret);    //true for inside, false for outside, Ret denotes case number
bool IntersectionPoint(GLint j,GLfloat * Vertex1,GLfloat *Vertex2,GLfloat *IP);
#endif
