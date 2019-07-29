/*
* assignscene.h
*/

#ifndef assignscene_H
#define assignscene_H

#define SXY	1
#define SYX	2
#define SYZ	3
#define SZY	4
#define SXZ	5
#define SZX	6


extern void myinit (void);
extern void mydisplay (void);
extern void keyboard(unsigned char key, int x, int y);
extern void reshape(int w, int h);

extern void myRotate(GLfloat degree,GLfloat x,GLfloat y,GLfloat z);

extern void myScale(GLfloat x,GLfloat y,GLfloat z);
extern void myTranslate(GLfloat x,GLfloat y,GLfloat z);
extern void myShear(int ShearType,GLfloat degree);




#endif
