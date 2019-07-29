
/* Copyright (c) Mark J. Kilgard, 1997. */

/* This program is freely distributable without licensing fees
   and is provided without guarantee or warrantee expressed or
   implied. This program is -not- in the public domain. */

/* This program was requested by Patrick Earl; hopefully someone else
   will write the equivalent Direct3D immediate mode program. */

#include <GL/glut.h>
#include<stdio.h>
#include<math.h>
#include <unistd.h>
#include<vector>
#include<iostream>
#include "ScanLine.h"

using namespace std;


vector<Point> VertexTable[NUMOBJECTS];
vector<Info> EdgeTable[NUMOBJECTS];
vector<Info> SurfaceTable[NUMOBJECTS];
vector<Info> ActiveEdgeList;



GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
GLfloat light_position[] = {-1.0, -1.0, -1.0, 0.0};  /* Infinite light location. */
//GLfloat light_position[] = {1.0, 1.0, 1.0, 1.0};

//my parameters
GLfloat blackcolor[]={0.0,0.0,0.0,1.0};
GLfloat whitecolor[]={1.0,1.0,1.0,1.0};
GLfloat diffuseCoeff[]={1.0,0.0,0.0,1.0};
GLfloat specularCoeff[]={1.0,1.0,1.0,1.0};
GLfloat Shine=100.0;

GLfloat EyePos[3]={0.0,0.0,5.0};

GLdouble DeterminantArray[3][3];



//GLfloat light_position[] = {0.0, 0.0, -1.0, 0.0};
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
//GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  //{0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
 // {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
 GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {5, 6, 2, 1},{0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0},  {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */

GLfloat Colors1[8][3]={{0,0,0},{0,0,1},{0,1,0},{0,1,1},{1,0,0},{1,0,1},{1,1,0},{1,1,1}};// colors at each vertex
GLfloat Colors2[8][3]={{1,1,1},{1,1,0},{1,0,1},{1,0,0},{0,1,1},{0,1,0},{0,0,1},{0,0,0}};// colors at each vertex

const int winWidth=500,winHeight=500;
GLfloat FB[winWidth][winHeight][3];

//function declaration
bool checkDuplicate(int ObjNum,Info Obj,int *Num,int SurfaceNum);
void StoreInfo(int ObjNum);
void ScanLineInterface();

void InsertPoint( vector<int> *UniquePoints,int Point);
GLdouble Cal3Determinant();
void SurfaceEq(Info * SurfaceObj,int ObjNum);
void InfoPrint();

void drawBox(int ObjNum)
{
  int i;
  //glColor3f (1.0, 0.0, 0.0);
  for (i = 0; i < 6; i++)
  //i=0;
  {
    glBegin(GL_POLYGON);
    glNormal3fv(&n[i][0]);

    //glColor3f (1.0, 0.0, 0.0);


    glColor3fv(Colors1[faces[i][0]]);

    glVertex3fv(&v[faces[i][0]][0]);
    //glColor3f (0.0, 1.0, 0.0);

    glColor3fv(Colors1[faces[i][1]]);

    glVertex3fv(&v[faces[i][1]][0]);
    //glColor3f (0.0, 0.0, 1.0);

    glColor3fv(Colors1[faces[i][2]]);

    glVertex3fv(&v[faces[i][2]][0]);
    //glColor3f (1.0, 0.0, 0.0);

    glColor3fv(Colors1[faces[i][3]]);

    glVertex3fv(&v[faces[i][3]][0]);

    glEnd();
  }
}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glPushMatrix();

  glTranslatef(-0.25,-0.35,2);
  glRotatef(45, 1.0, 0.0, 1.0);
//  glRotatef(-40, 0.0,3 0.0, 1.0);

  glScalef(0.3,0.3,0.3);

  VertexTable[0].clear();
  EdgeTable[0].clear();
  SurfaceTable[0].clear();

  StoreInfo(0);

  cout<<"Size Matters "<<VertexTable[0].size()<<" "<<EdgeTable[0].size()<<" "<<SurfaceTable[0].size();


  glColor3f(1.0,0.0,0.0);
  //drawBox(0);        //original

  glPopMatrix();

   glPushMatrix();
   glTranslatef(-0.25,-0.05,0);
   glRotatef(45, 1.0, 0.0, 1.0);
//glRotatef(-40, 0.0,3 0.0, 1.0);

  glScalef(0.3,0.3,0.3);

   VertexTable[1].clear();
  EdgeTable[1].clear();
  SurfaceTable[1].clear();

  StoreInfo(1);
  cout<<"Size Matters "<<VertexTable[1].size()<<" "<<EdgeTable[1].size()<<" "<<SurfaceTable[1].size();

glColor3f(0.0,1.0,0.0);
   // drawBox(1);        //original

  glPopMatrix();
  glutSwapBuffers();
  glFlush();
  //sleep(5);

ScanLineInterface();
  //GetModelViewMatrix();
//sleep(2);
//Clipping();   //clipped
glFlush();

}

void init()
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



 // glEnable(GL_LIGHT0);
  //glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  //glEnable(GL_DEPTH_TEST);

//  glEnable(GL_CULL_FACE);
 // glCullFace(GL_BACK);


  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity ();

  gluPerspective( /* field of view in degree */ 40.0,
    /* aspect ratio */ 1.0,
    /* Z near */ 2.0, /* Z far */ 10.0);

 // glOrtho(-1,1,-1,1,2.0,10.0);






glMatrixMode(GL_MODELVIEW);

/*glLoadIdentity();
glTranslatef(0.25,0.35,0);
ModelViewMatrix();
glPopMatrix();*/


  gluLookAt(EyePos[0], EyePos[1], EyePos[2],  /* eye is at (0,0,5) */
    0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    0.0, 1.0, 0.0);      /* up is in positive Y direction */

  /* Adjust cube position to be asthetic angle. */



  //glRotatef(45, 1.0, 0.0, 1.0);
  //glRotatef(-40, 0.0,3 0.0, 1.0);
 // glTranslatef(0.25,0.35,0);
  //glScalef(0.5,0.5,0.5);


  glShadeModel (GL_SMOOTH);
}

int main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (winWidth, winHeight);
   glutInitWindowPosition (100, 100);
  glutCreateWindow("red 3D lighted cube");
   init();
  glutDisplayFunc(display);

  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}


void ScanLineInterface()
{
      GLdouble ProjMatrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX,ProjMatrix);

    GLdouble ModelMatrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX,ModelMatrix);

    GLint ViewPort[4];
    glGetIntegerv(GL_VIEWPORT,ViewPort);

     GLdouble ObjCoord1[3],WinCoord1[3],ObjCoord2[3],WinCoord2[3];
     GLdouble Pix_Origin_Obj[3];

      gluUnProject(0,0,0.1,ModelMatrix,ProjMatrix,ViewPort,&Pix_Origin_Obj[0],&Pix_Origin_Obj[1],&Pix_Origin_Obj[2]);

       printf("Origin %lf,%lf,%lf\n",Pix_Origin_Obj[0],Pix_Origin_Obj[1],Pix_Origin_Obj[2]);   //gives square pixels values, its is same in x and y

     glReadPixels(0,0,winWidth,winHeight,GL_RGB,GL_FLOAT,FB);          //reads frame buffer
     InfoPrint();

     ScanLineAlgo();


















    int OriX=Pix_Origin_Obj[0];
    int OriY=Pix_Origin_Obj[1];
 /*glMatrixMode(GL_MODELVIEW);
 glLoadIdentity();

 glMatrixMode(GL_PROJECTION);
 glLoadIdentity();*/
printf("Origin X %d %d\n",OriX,OriY);
    glRasterPos2d(Pix_Origin_Obj[0],Pix_Origin_Obj[1]);
   //glRasterPos2f(-0.8,-0.8);
     glDrawPixels(winWidth,winHeight,GL_RGB,GL_FLOAT,FB);     //stores changed values in frmae buffer
glutSwapBuffers();




}

void StoreInfo(int ObjNum)
{
    GLdouble ProjMatrix[16];            //wahi sab
    glGetDoublev(GL_PROJECTION_MATRIX,ProjMatrix);

    GLdouble ModelMatrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX,ModelMatrix);

    GLint ViewPort[4];
    glGetIntegerv(GL_VIEWPORT,ViewPort);

    GLdouble ObjCoord1[3],WinCoord1[3],ObjCoord2[3],WinCoord2[3];



     int i=0,j=0;

     for(i=0;i<8;i++)
     {
         Point Obj;
         Obj.Num=i;
         ObjCoord1[0]=v[i][0];
         ObjCoord1[1]=v[i][1];
         ObjCoord1[2]=v[i][2];

         if(ObjNum==0)
         {
             Obj.Color[0]=Colors1[i][0];     //assigning color to vertex
             Obj.Color[1]=Colors1[i][1];
             Obj.Color[2]=Colors1[i][2];
         }
         if(ObjNum==1)
         {
             Obj.Color[0]=Colors2[i][0];     //assigning color to vertex
             Obj.Color[1]=Colors2[i][1];
             Obj.Color[2]=Colors2[i][2];
         }
         gluProject(ObjCoord1[0],ObjCoord1[1],ObjCoord1[2],ModelMatrix,ProjMatrix,ViewPort,&Obj.x,&Obj.y,&Obj.z);
         VertexTable[ObjNum].push_back(Obj);
     }


    for(i=0;i<6;i++)
   // i=0;
    {

        Info SurfObj;

        for(j=0;j<4;j++)
        {
        /*ObjCoord1[0]=v[faces[i][j]][0];
        ObjCoord1[1]=v[faces[i][j]][1];
        ObjCoord1[2]=v[faces[i][j]][2];


        ObjCoord2[0]=v[faces[i][(j+1)%4]][0];
        ObjCoord2[1]=v[faces[i][(j+1)%4]][1];
        ObjCoord2[2]=v[faces[i][(j+1)%4]][2];*/

        Info Obj;
        Obj.InfoVect.push_back(faces[i][j]);
        Obj.InfoVect.push_back(faces[i][(j+1)%4]);

        int Num;


        if(!checkDuplicate(ObjNum,Obj,&Num,i))
        {
            Obj.Num=EdgeTable[ObjNum].size();
            Obj.SurfaceVect.push_back(i);
            Obj.ObjNum=ObjNum;

            GLdouble X1,X2,Y1,Y2,IS;
            X1=VertexTable[ObjNum].at(faces[i][j]).x;
            Y1=VertexTable[ObjNum].at(faces[i][j]).y;

            X2=VertexTable[ObjNum].at(faces[i][(j+1)%4]).x;
            Y2=VertexTable[ObjNum].at(faces[i][(j+1)%4]).y;

            IS=(X1-X2)/(Y1-Y2);
            Obj.InverseSlope=IS;

            EdgeTable[ObjNum].push_back(Obj);
            SurfObj.InfoVect.push_back(Obj.Num);


        }else{     //Duplicate
            SurfObj.InfoVect.push_back(Num);
        }
      }
      SurfObj.ObjNum=ObjNum;
      SurfaceEq(&SurfObj,ObjNum);
      SurfaceTable[ObjNum].push_back(SurfObj);


    }
}

bool checkDuplicate(int ObjNum,Info Obj,int *Num,int SurfaceNum)
{
    vector<Info>::iterator it;
    bool found=false;
    for ( it=EdgeTable[ObjNum].begin() ; it < EdgeTable[ObjNum].end(); it++ )
    {
        if(( (*it).InfoVect.at(0)==Obj.InfoVect.at(0) && (*it).InfoVect.at(1)==Obj.InfoVect.at(1) ) || ( (*it).InfoVect.at(0)==Obj.InfoVect.at(1) && (*it).InfoVect.at(1)==Obj.InfoVect.at(0) ))
        {
            *Num=(*it).Num;
            (*it).SurfaceVect.push_back(SurfaceNum);
            found=true;
            break;
        }
    }
    return found;
}

void SurfaceEq(Info * SurfaceObj,int ObjNum) // wiki page on determinant and Planes
{
    GLdouble Vertex[4][3];
    vector<int> UniquePoints;
    UniquePoints.clear();

    vector<int>::iterator it;
    for ( it=SurfaceObj->InfoVect.begin() ; it < SurfaceObj->InfoVect.end(); it++ ) // every edge of surface
    {
        InsertPoint(&UniquePoints,(EdgeTable[ObjNum].at((*it))).InfoVect.at(0));      //two points of surface and remving duplication
        InsertPoint(&UniquePoints,(EdgeTable[ObjNum].at((*it))).InfoVect.at(1));
    }
    int i=0,j=0;


     for ( it=UniquePoints.begin() ; it < UniquePoints.end(); it++ ) // 4 points now
     {
        Vertex[i][0]=VertexTable[ObjNum].at((*it)).x;
        Vertex[i][1]=VertexTable[ObjNum].at((*it)).y;
        Vertex[i][2]=VertexTable[ObjNum].at((*it)).z;
        i++;
     }

     GLdouble D,A1,B1,C1;

     for(i=0;i<3;i++)
     {
         for(j=0;j<3;j++)
         {
             DeterminantArray[i][j]=Vertex[i][j];
         }
     }
     D=Cal3Determinant();

     DeterminantArray[0][0]=1.0;
     DeterminantArray[1][0]=1.0;
     DeterminantArray[2][0]=1.0;

     A1=Cal3Determinant();
     A1=-A1/D;    //assuming d=1

     for(i=0;i<3;i++)
     {
         for(j=0;j<3;j++)
         {
             DeterminantArray[i][j]=Vertex[i][j];
         }
     }

     DeterminantArray[0][1]=1.0;
     DeterminantArray[1][1]=1.0;
     DeterminantArray[2][1]=1.0;

     B1=Cal3Determinant();
     B1=-B1/D;

      for(i=0;i<3;i++)
     {
         for(j=0;j<3;j++)
         {
             DeterminantArray[i][j]=Vertex[i][j];
         }
     }

     DeterminantArray[0][2]=1.0;
     DeterminantArray[1][2]=1.0;
     DeterminantArray[2][2]=1.0;

     C1=Cal3Determinant();
     C1=-C1/D;

     SurfaceObj->A=A1;
     SurfaceObj->B=B1;
     SurfaceObj->C=C1;
     SurfaceObj->D=1;
}

GLdouble Cal3Determinant()
{
    GLdouble a,b,c,d,e,f,g,h,i;
    a=DeterminantArray[0][0];
    b=DeterminantArray[0][1];
    c=DeterminantArray[0][2];

    d=DeterminantArray[1][0];
    e=DeterminantArray[1][1];
    f=DeterminantArray[1][2];

    g=DeterminantArray[2][0];
    h=DeterminantArray[2][1];
    i=DeterminantArray[2][2];

    GLdouble Val=a*e*i+b*f*g+c*d*h-a*f*h-b*d*i-c*e*g;
    return Val;
}

void InsertPoint( vector<int> *UniquePoints,int Point)
{
     vector<int>::iterator it;
    bool found=false;
    for ( it=UniquePoints->begin() ; it < UniquePoints->end(); it++ )
    {
        if((*it)==Point)
        {
            found=true;
            break;
        }
    }
    if(!found)
    {
        UniquePoints->push_back(Point);
    }
}

void InfoPrint()
{

    int i=0;
     vector<Point>::iterator it1;
     vector<Info>::iterator it2;
    for(i=0;i<NUMOBJECTS;i++)
    {
         cout<<"Surface "<<i<<endl;
        cout<<"Printing Vertex "<<endl;

        for ( it1=VertexTable[i].begin() ; it1 < VertexTable[i].end(); it1++ )
        {
            cout<<"X "<<(*it1).x<<"y "<<(*it1).y<<"z "<<(*it1).z<<endl;
        }

        cout<<"Printing Edge "<<endl;
        for ( it2=EdgeTable[i].begin() ; it2 < EdgeTable[i].end(); it2++ )
        {
            cout<<"V1 "<<(*it2).InfoVect.at(0)<<"V2 "<<(*it2).InfoVect.at(1)<<endl;
        }

        cout<<"Printing Surface "<<endl;
        for ( it2=SurfaceTable[i].begin() ; it2 < SurfaceTable[i].end(); it2++ )
        {
            cout<<"E1 "<<(*it2).InfoVect.at(0)<<"E2 "<<(*it2).InfoVect.at(1)<<"E3 "<<(*it2).InfoVect.at(2)<<"E4 "<<(*it2).InfoVect.at(3)<<endl;
        }

    }


}


