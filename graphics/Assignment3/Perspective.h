#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#ifndef PERSPECTIVE_H_INCLUDED
#define PERSPECTIVE_H_INCLUDED
#define PI 3.14159265
 void myglFrustum(GLfloat xwmin,GLfloat xwmax,GLfloat ywmin,GLfloat ywmax,GLfloat dnear,GLfloat dfar);
 void myglPerspective(GLfloat theta,GLfloat aspect,GLfloat dnear,GLfloat dfar);
 void WorldToViewTransform(GLfloat x0,GLfloat y0,GLfloat z0,GLfloat xref,GLfloat yref,GLfloat zref,GLfloat Vx,GLfloat Vy,GLfloat Vz);

#endif // PERSPECTIVE_H_INCLUDED
