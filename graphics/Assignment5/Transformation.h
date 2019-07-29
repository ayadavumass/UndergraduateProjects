/*
* Transforamtion.h
*/

#ifndef Transformation_H
#define Transformation_H

#define SXY	1
#define SYX	2
#define SYZ	3
#define SZY	4
#define SXZ	5
#define SZX	6




extern void myRotate(GLfloat degree,GLfloat x,GLfloat y,GLfloat z);

extern void myScale(GLfloat x,GLfloat y,GLfloat z);
extern void myTranslate(GLfloat x,GLfloat y,GLfloat z);
extern void myShear(int ShearType,GLfloat degree);




#endif
