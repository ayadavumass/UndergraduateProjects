#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef BRESENHAM_H_INCLUDED
#define BRESENHAM_H_INCLUDED
const GLint winWidth=500,winHeight=500;

void Bresenham_Line(GLdouble * Vertex1, GLdouble * Vertex2);
extern GLfloat FB[winWidth][winHeight][3];
#endif // BRESENHAM_H_INCLUDED
