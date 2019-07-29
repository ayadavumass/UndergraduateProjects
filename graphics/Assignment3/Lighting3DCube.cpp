/* Copyright (c) Mark J. Kilgard, 1997. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

/* This program was requested by Patrick Earl; hopefully someone else
   will write the equivalent Direct3D immediate mode program. */

#include <GL/glut.h>
#include<stdio.h>
#include<math.h>
#include"Lighting3DCube.h"
#include"CommonFunc.h"
#include"LightingModel.h"
#include"Perspective.h"
#include"Transform.h"


GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
//GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 1.0};

//my parameters
GLfloat blackcolor[]={0.0,0.0,0.0,1.0};
GLfloat whitecolor[]={1.0,1.0,1.0,1.0};
GLfloat diffuseCoeff[]={1.0,0.4,0.9,1.0};
GLfloat specularCoeff[]={1.0,1.0,1.0,1.0};
GLfloat Shine=100.0;
GLfloat LightAtVertex[8][3];
GLfloat ViewerVector[8][3];
GLfloat EyePos[3]={0.0,0.0,5.0};



//GLfloat light_position[] = {0.0, 0.0, -1.0, 0.0};
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */

GLfloat TranformedCoord[8][3];
GLfloat TransformedNormal[6][3];
GLfloat AvgNormal[8][3];  //at each vertex

GLfloat ColorStore[8][3];
GLdouble WindowCoord[8][3];
GLdouble ModelViewMat[4][4];


//function declarations
void GetModelViewMatrix();
void CrossProduct(GLfloat *V1,GLfloat *V2,GLfloat *V3) ;
void Normalize(GLfloat * N);
void TranformVertex(GLfloat *Vertex,GLfloat *Output);
void CalculateNormal();
void DotProduct(GLfloat *V1,GLfloat *V2,GLfloat *Output);
void SpecularLightCalc(GLfloat *Vertex,GLfloat *Coeff,GLfloat *Intensity,GLfloat *ViewerVect,GLfloat *LightPos,GLfloat *NormalVect,GLfloat Shine,GLfloat *Output);
void DiffuseLightCalc(GLfloat *Vertex,GLfloat * Coeff,GLfloat *Intensity,GLfloat *NormalVect,GLfloat *LightPos,GLfloat *Output);
void AmbientLightCalc(GLfloat * Coeff,GLfloat *Intensity,GLfloat *Output);
void LightCalculations(GLfloat * Vertex,GLfloat * AmbCoeff,GLfloat *AmbIntensity,GLfloat * DiffCoeff,GLfloat *DiffIntensity,GLfloat *NormalVect,GLfloat *LightPos,GLfloat *SpecCoeff,GLfloat *SpecIntensity,GLfloat *ViewerVect,GLfloat Shine,GLfloat *Output);
void CalculateAvgNormal();
void CalculateVertex();
void LightEachVertex();


void
drawBox(void)
{
  int i;

  for (i = 0; i < 6; i++) {
    glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);
    // glColor3f (1.0, 0.0, 0.0);
     /*ColorStore[faces[i][0]][0]=1.0;
     ColorStore[faces[i][0]][1]=0.0;
     ColorStore[faces[i][0]][2]=0.0;*/

    glVertex3fv(&v[faces[i][0]][0]);
    //glColor3f (0.0, 1.0, 0.0);

     /*ColorStore[faces[i][1]][0]=0.0;
     ColorStore[faces[i][1]][1]=1.0;
     ColorStore[faces[i][1]][2]=0.0;*/

    glVertex3fv(&v[faces[i][1]][0]);
     //glColor3f (0.0, 0.0, 1.0);

     /*ColorStore[faces[i][2]][0]=0.0;
     ColorStore[faces[i][2]][1]=0.0;
     ColorStore[faces[i][2]][2]=1.0;*/

    glVertex3fv(&v[faces[i][2]][0]);
   //  glColor3f (0.0, 1.0, 1.0);

     /*ColorStore[faces[i][3]][0]=0.0;
     ColorStore[faces[i][3]][1]=1.0;
     ColorStore[faces[i][3]][2]=1.0;*/

    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();
  }
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawBox();
  //GetModelViewMatrix();
  CalculateVertex();
  CalculateNormal();
  CalculateAvgNormal();
  LightEachVertex();


    glutSwapBuffers();
}

void CubeInit(GLfloat x,GLfloat y,GLfloat z)
{
  /* Setup cube vertex data. */
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;



glEnable(GL_NORMALIZE);
  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);



  glLightfv(GL_LIGHT0,GL_AMBIENT,blackcolor);
  glLightfv(GL_LIGHT0,GL_SPECULAR,whitecolor);

  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specularCoeff);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Shine);



  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 40.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 1.0, /* Z far */ 10.0);


 glMatrixMode(GL_MODELVIEW);
 // glTranslatef(x, y, z);
 myTranslate(x,y,z);
    //find only transformation matrix, because for lighting we need corrdinates in world

    glPushMatrix();
    myTranslate(0.0, 0.0, -2.0);

  myRotate(60, 1.0, 0.0, 0.0);
  myRotate(-40, 0.0, 0.0, 1.0);
  GetModelViewMatrix();
  glPopMatrix();







  gluLookAt(EyePos[0], EyePos[1], EyePos[2],  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.0);      /* up is in positive Y direction */

  /* Adjust cube position to be asthetic angle. */
  glTranslatef(0.0, 0.0, -2.0);
  glRotatef(60, 1.0, 0.0, 0.0);
  glRotatef(-40, 0.0, 0.0, 1.0);
  glShadeModel (GL_SMOOTH);
}

/*int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("red 3D lighted cube");
  glutDisplayFunc(display);
  init();
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
//}*/



void GetModelViewMatrix()
{
    printf("TRANSFORMATION MATRIX\n");
    glGetDoublev(GL_MODELVIEW_MATRIX,&ModelViewMat[0][0]);

    int i=0,j=0;
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            printf("%lf ",ModelViewMat[i][j]);
        }
        printf("\n");
    }
    printf("\n\n");
}

void CalculateVertex()
{
    //printf("TRANSFORMED VERTEX\n");
    int i=0;
    for(i=0;i<8;i++)
    {
          GLfloat Vertex0[4];
          GLfloat Output0[4];
          Vertex0[0]=v[i][0];
          Vertex0[1]=v[i][1];
          Vertex0[2]=v[i][2];
          Vertex0[3]=1;
          TranformVertex(Vertex0,Output0);

          TranformedCoord[i][0]=Output0[0];
          TranformedCoord[i][1]=Output0[1];
          TranformedCoord[i][2]=Output0[2];

      //    printf("Vertex %d %lf %lf %lf\n",i,TranformedCoord[i][0],TranformedCoord[i][1],TranformedCoord[i][2]);


    }
    //printf("\n\n");

}

void CalculateNormal()
{

    //printf("TRANSFORMED NORMAL\n");
    int i=0;
    for(i=0;i<6;i++)
    {

        GLfloat Output0[4],Output1[4],Output2[4];

        Output0[0]=TranformedCoord[faces[i][0]][0];
        Output0[1]=TranformedCoord[faces[i][0]][1];
        Output0[2]=TranformedCoord[faces[i][0]][2];

          Output1[0]=TranformedCoord[faces[i][1]][0];
        Output1[1]=TranformedCoord[faces[i][1]][1];
        Output1[2]=TranformedCoord[faces[i][1]][2];

          Output2[0]=TranformedCoord[faces[i][2]][0];
        Output2[1]=TranformedCoord[faces[i][2]][1];
        Output2[2]=TranformedCoord[faces[i][2]][2];




        GLfloat Vector1[3]={Output1[0]-Output0[0],Output1[1]-Output0[1],Output1[2]-Output0[2]};
        GLfloat Vector2[3]={Output2[0]-Output0[0],Output2[1]-Output0[1],Output2[2]-Output0[2]};

        GLfloat Normal[3];

        CrossProduct(Vector1,Vector2,Normal);
        Normalize(Normal);


        TransformedNormal[i][0]=Normal[0];
        TransformedNormal[i][1]=Normal[1];
        TransformedNormal[i][2]=Normal[2];

        //printf("Face %d %lf %lf %lf\n",i,TransformedNormal[i][0],TransformedNormal[i][1],TransformedNormal[i][2]);
    }
    //printf("\n\n");
}



void TranformVertex(GLfloat *Vertex,GLfloat *Output)
{
    int i=0,j=0;

    for(i=0;i<4;i++)
    {
        Output[i]=0;
        for(j=0;j<4;j++)
        {
            Output[i]+=ModelViewMat[j][i]*Vertex[j];      //in column major form

        }
    }
}


void CalculateAvgNormal()
{

    //printf("AVERAGE NORMAL\n");
    int i=0,j=0,Count=0,k=0;

    for(i=0;i<8;i++)    //finding how many faces this vertex belngs
    {
        Count=0;
        AvgNormal[i][0]=0;
        AvgNormal[i][1]=0;
        AvgNormal[i][2]=0;

        for(j=0;j<6;j++)
        {
            for(k=0;k<4;k++)
            {
                if(faces[j][k]==i)    //vertex is in face
                {
                    AvgNormal[i][0]+=TransformedNormal[j][0];
                    AvgNormal[i][1]+=TransformedNormal[j][1];
                    AvgNormal[i][2]+=TransformedNormal[j][2];
                    Count++;
                    break;   //one vertex occur only once in face
                }
            }
        }

        AvgNormal[i][0]/=Count;     //averaging, in square Count ==3
        AvgNormal[i][1]/=Count;
        AvgNormal[i][2]/=Count;

        //printf("Vertex %d %lf %lf %lf\n",i,AvgNormal[i][0],AvgNormal[i][1],AvgNormal[i][2]);
    }
   // printf("\n\n");
}






void LightEachVertex()
{
    printf("LIGHT INTENSITIES\n");
    int i=0;
    for(i=0;i<8;i++)
    {
        ViewerVector[i][0]=EyePos[0]-TranformedCoord[i][0];
        ViewerVector[i][1]=EyePos[1]-TranformedCoord[i][1];
        ViewerVector[i][2]=EyePos[2]-TranformedCoord[i][2];
        Normalize(ViewerVector[i]);

        LightCalculations(TranformedCoord[i],diffuseCoeff,blackcolor,diffuseCoeff,light_diffuse, AvgNormal[i],light_position,specularCoeff,whitecolor,ViewerVector[i],Shine,LightAtVertex[i]);
        printf("Vertex %d %lf %lf %lf\n",i,LightAtVertex[i][0],LightAtVertex[i][1],LightAtVertex[i][2]);
    }
    fflush(stdout);
    printf("\n\n");
}




