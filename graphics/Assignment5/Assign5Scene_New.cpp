#include <GL/glut.h>
#include<stdio.h>
#include<math.h>
#include <unistd.h>
#include<vector>
#include<iostream>
#include"Transformation.h"
#include"Clipping.h"
#include"ScanLine.h"
#include"CommonFunc.h"
#include"LightingModel.h"
#include"Perspective.h"

#define VIEWPOS 2   // can be 0,1,2
#define PART  3     // can be 1 2 3

#define MAXPTS                  6              // maxpts were a triangle can intersect cuboid

#define OPENGL                  0      // 1 for opengl, 0 for implemntation
using namespace std;

const int winWidth=500,winHeight=500;
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};  /* White diffuse light. */
//GLfloat light_position[] = {-1.0, -1.0, -1.0, 0.0};  /* Infinite light location. */
GLfloat light_position[] = {4.0, 4.0, 4.0, 1.0};

// parameters
GLfloat blackcolor[]={0.0,0.0,0.0,1.0};
GLfloat whitecolor[]={1.0,1.0,1.0,1.0};
/*GLfloat diffuseCoeff1[]={1.0,0.0,0.0,1.0};
GLfloat diffuseCoeff2[]={1.0,1.0,1.0,1.0};
GLfloat diffuseCoeff3[]={0.0,0.0,1.0,1.0};
GLfloat diffuseCoeff4[]={0.0,1.0,0.0,1.0};*/

GLfloat diffuseCoeff[4][4]={{1.0,0.0,0.0,1.0},{1.0,1.0,1.0,1.0},{0.0,0.0,1.0,1.0},{0.0,1.0,0.0,1.0}};
GLfloat specularCoeff[]={1.0,1.0,1.0,1.0};
GLfloat Shine=10.0;

GLdouble EyePos[3][3]={{1.2,3,2},{2,2,2},{-1.0,1,3}};
GLfloat LookAtPoint[3][3]={{1.2,0.5,0.5},{0,0.5,0.5},{1.0,0.5,0.5}};
GLfloat ViewUpVector[3][3]={{0.0,1.0,0.0},{0,0,1},{0,0,1}};

GLfloat Obj1Coord[4][3]={{0.0,0.0,0.0},{0.0,1.0,0.0},{0.0,1.0,1.0},{0.0,0.0,1.0}};
GLfloat Obj2Coord[4][3]={{1.0,0.0,0.0},{1.0,1.0,0.0},{1.0,1.0,1.0},{1.0,0.0,1.0}};
GLfloat Obj3aCoord[3][3]={{2.0,0.5,0.0},{1.0,0.5,0.5},{2.0,0.5,1.0}};
GLfloat Obj3cCoord[3][3]={{0.0,0.5,0.0},{0.1,1.5,0.5},{0.0,0.5,1.0}};
GLfloat Obj4aCoord[4][3]={{2.0,0.5,0.0},{2.0,0.5,1.0},{3.0,0.5,1.0},{3.0,0.5,0.0}};

GLdouble ProjMatrix[4][4];
GLdouble ModelMatrix[4][4];
GLdouble ViewPortMatrix[4][4];


GLdouble AverageNormal[NUMOBJECTS][3];
GLdouble TransformedNormal[NUMOBJECTS][3];

GLfloat FB[winWidth][winHeight][3];

GLfloat OpenNormal[NUMOBJECTS][3]={{1.0,0.0,0.0},{1.0,0.0,0.0},{0.0,1.0,0.0},{0.0,1.0,0.0}};

typedef struct HomoPoint{
    GLdouble x,y,z,h;
}HomoPoint;
vector<HomoPoint> ObjPoints[NUMOBJECTS];
vector<Point> ModeViewPoints[NUMOBJECTS];
vector<HomoPoint> ProjPoints[NUMOBJECTS];
vector<HomoPoint> ClippedPoints[NUMOBJECTS];
vector<HomoPoint> ViewPortPoints[NUMOBJECTS];

vector<Point> VertexTable[NUMOBJECTS];
vector<Info> EdgeTable[NUMOBJECTS];
vector<Info> SurfaceTable[NUMOBJECTS];
vector<Info> ActiveEdgeList;

GLdouble DeterminantArray[3][3];     // storing determinant calculations





//function declarations
void CalculateVertex(int ObjNum);
void Clipping();
void TranformVertex(GLfloat *Vertex,GLfloat *Output);
void InputPoints();
void EnterClippedPoints(int ObjNum,GLdouble * Pt) ;
void StoreInfo();
void SurfaceEq(Info * SurfaceObj,int ObjNum); // wiki page on determinant and Planes
GLdouble Cal3Determinant();
void InsertPoint( vector<int> *UniquePoints,int Point);
void ScanLineInterface();


void ViewPortTranformVertex(GLfloat *Vertex,GLfloat *Output);
void ClippedToViewPortVertex();

bool FloatComparison(GLdouble x1,GLdouble x2);   // true if same, otherwise false

void ViewPortToModelView();
void CalculateNormal();

void LightEachVertex();
void AvgNormal(GLfloat * V1,int i,GLfloat *Normal);

void
drawScene(void)
{

glPushMatrix();       // object 1

glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff[0]);

CalculateVertex(0);

//glColor3f (1.0, 0.0, 0.0);
glBegin(GL_POLYGON);
glNormal3fv(OpenNormal[0]);
int i=0;
for(i=0;i<4;i++)
    if(OPENGL==1)
    glVertex3fv(Obj1Coord[i]);
glEnd();

//Clipping(0);
//ClippedToViewPortVertex(0);
//ViewPortToModelView(0);

glPopMatrix();


glPushMatrix();       //object 2

CalculateVertex(1);
glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff[1]);
//glColor3f (1.0, 1.0, 1.0);
glBegin(GL_POLYGON);
glNormal3fv(OpenNormal[1]);
  for(i=0;i<4;i++)
  if(OPENGL==1)
  glVertex3fv(Obj2Coord[i]);
glEnd();

//Clipping(1);
//ClippedToViewPortVertex(1);
//ViewPortToModelView(1);

glPopMatrix();



glPushMatrix();       //object 3 s1
glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff[2]);
//glColor3f (0.0, 0.0, 1.0);
if(PART==2)
    {
       // glTranslatef(-1.0,0.0,0.0);
        myTranslate(-1.0,0.0,0.0);
    }
   /* if(PART==3)
    {
        GLfloat cx=(Obj3aCoord[0][0]+Obj3aCoord[1][0]+Obj3aCoord[2][0])/3;
        GLfloat cy=(Obj3aCoord[0][1]+Obj3aCoord[1][1]+Obj3aCoord[2][1])/3;
        GLfloat cz=(Obj3aCoord[0][2]+Obj3aCoord[1][2]+Obj3aCoord[2][2])/3;
       // glTranslatef(-cx,-cy,-cz);

        //glTranslatef(0.0,0.0,0.0);
        //glRotatef(90,0.0,0.0,1.0);

        cx=(Obj3cCoord[0][0]+Obj3cCoord[1][0]+Obj3cCoord[2][0])/3;
        cy=(Obj3cCoord[0][1]+Obj3cCoord[1][1]+Obj3cCoord[2][1])/3;
        cz=(Obj3cCoord[0][2]+Obj3cCoord[1][2]+Obj3cCoord[2][2])/3;

        // glTranslatef(cx,cy,cz);
        //for(i=0;i<3;i++)
        //glVertex3fv(Obj3cCoord[i]);
    }*/

CalculateVertex(2);


//Clipping(2);
//ClippedToViewPortVertex(2);
//ViewPortToModelView(2);

    glBegin(GL_POLYGON);
    glNormal3fv(OpenNormal[2]);
    for(i=0;i<3;i++)
    {
        if(PART==3)
        {
            if(OPENGL==1)
            glVertex3fv(Obj3cCoord[i]);
            continue;
        }
        if(OPENGL==1)
        glVertex3fv(Obj3aCoord[i]);
    }
    glEnd();
glPopMatrix();



glPushMatrix();       //object 3 s2


glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff[3]);
//glColor3f (0.0, 1.0, 0.0);
if(PART==2)
    {
       // glTranslatef(-1.0,0.0,0.0);
        myTranslate(-1.0,0.0,0.0);
    }
    if(PART==3)
    {
       // glTranslatef(-2.0,0.0,0.0);
        myTranslate(-2.0,0.0,0.0);
    }

CalculateVertex(3);

//Clipping(3);
//ClippedToViewPortVertex(3);
//ViewPortToModelView(3);

glBegin(GL_POLYGON);
glNormal3fv(OpenNormal[3]);

 for(i=0;i<4;i++)
 if(OPENGL==1)
 glVertex3fv(Obj4aCoord[i]);

glEnd();
glPopMatrix();

}

void display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawScene();        //original


    if(OPENGL==0)
    {
        Clipping();       // does the clipping
        ClippedToViewPortVertex();
        StoreInfo();     // makes vertex, edge, surface table for scan line algo
        ViewPortToModelView();
        CalculateNormal();
        LightEachVertex();

        ScanLineInterface();

    }

  glFlush();
  glutSwapBuffers();
}

void init()
{
    if(OPENGL==1)
    {
  //  glClearColor(1,1,1,1);

    glEnable(GL_NORMALIZE);
  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);



  glLightfv(GL_LIGHT0,GL_AMBIENT,blackcolor);
  glLightfv(GL_LIGHT0,GL_SPECULAR,whitecolor);


 glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specularCoeff);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,Shine);


  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);
  glShadeModel (GL_SMOOTH);
    }

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
   glLoadIdentity ();
 // gluPerspective( /* field of view in degree */ 60.0,
   // /* aspect ratio */ 1.0,
   // /* Z near */ 1.0, /* Z far */10.0);
   //myglPerspective( 60.0,1.0,1.0,10.0);

   glOrtho(0,3,-4,3,0,20);



   //glFrustum(0, 3, 0.0, 1.0, 1.0, 10.0);

//   glOrtho(0,3,0,1,0,1);

// glOrtho(-0.5,1,-0.5,1,2.0,10.0);

glMatrixMode(GL_MODELVIEW);
glLoadIdentity ();
//gluLookAt(EyePos[VIEWPOS][0],EyePos[VIEWPOS][1],EyePos[VIEWPOS][2],
//LookAtPoint[VIEWPOS][0],LookAtPoint[VIEWPOS][1],LookAtPoint[VIEWPOS][2],
//ViewUpVector[VIEWPOS][0],ViewUpVector[VIEWPOS][1],ViewUpVector[VIEWPOS][2]);

WorldToViewTransform(EyePos[VIEWPOS][0],EyePos[VIEWPOS][1],EyePos[VIEWPOS][2],
LookAtPoint[VIEWPOS][0],LookAtPoint[VIEWPOS][1],LookAtPoint[VIEWPOS][2],
ViewUpVector[VIEWPOS][0],ViewUpVector[VIEWPOS][1],ViewUpVector[VIEWPOS][2]);


}

int main(int argc, char **argv)
{
    InputPoints();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize (winWidth, winHeight);
  glutCreateWindow("Assignment 5");

   init();
  glutDisplayFunc(display);



  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}







void InputPoints()
{
    HomoPoint Obj;

    Obj.x=0.0;     //object1
    Obj.y=0.0;
    Obj.z=0.0;
    ObjPoints[0].push_back(Obj);

    Obj.x=0.0;
    Obj.y=1.0;
    Obj.z=0.0;
    ObjPoints[0].push_back(Obj);

    Obj.x=0.0;
    Obj.y=1.0;
    Obj.z=1.0;
    ObjPoints[0].push_back(Obj);

    Obj.x=0.0;
    Obj.y=0.0;
    Obj.z=1.0;
    ObjPoints[0].push_back(Obj);



    Obj.x=1.0;     //object2
    Obj.y=0.0;
    Obj.z=0.0;
    ObjPoints[1].push_back(Obj);

    Obj.x=1.0;
    Obj.y=1.0;
    Obj.z=0.0;
    ObjPoints[1].push_back(Obj);

    Obj.x=1.0;
    Obj.y=1.0;
    Obj.z=1.0;
    ObjPoints[1].push_back(Obj);

    Obj.x=1.0;
    Obj.y=0.0;
    Obj.z=1.0;
    ObjPoints[1].push_back(Obj);



    if(PART==3)
    {

    Obj.x=0.0;       //object3
    Obj.y=0.5;
    Obj.z=0.0;
    ObjPoints[2].push_back(Obj);

    Obj.x=0.1;
    Obj.y=1.5;
    Obj.z=0.5;
    ObjPoints[2].push_back(Obj);



    Obj.x=0.0;
    Obj.y=0.5;
    Obj.z=1.0;
    ObjPoints[2].push_back(Obj);
    }else{



    Obj.x=2.0;      //object3
    Obj.y=0.5;
    Obj.z=0.0;
    ObjPoints[2].push_back(Obj);

     Obj.x=1.0;
    Obj.y=0.5;
    Obj.z=0.5;
    ObjPoints[2].push_back(Obj);

    Obj.x=2.0;
    Obj.y=0.5;
    Obj.z=1.0;
    ObjPoints[2].push_back(Obj);

    }



    Obj.x=2.0;     //object4
    Obj.y=0.5;
    Obj.z=0.0;
    ObjPoints[3].push_back(Obj);

    Obj.x=2.0;
    Obj.y=0.5;
    Obj.z=1.0;
    ObjPoints[3].push_back(Obj);

    Obj.x=3.0;
    Obj.y=0.5;
    Obj.z=1.0;
    ObjPoints[3].push_back(Obj);

    Obj.x=3.0;
    Obj.y=0.5;
    Obj.z=0.0;
    ObjPoints[3].push_back(Obj);
}



void Clipping()
{
    printf("New Clipped Vertex\n");
    int i=0;
    GLfloat TriangVertex1[4],TriangVertex2[4],TriangVertex3[4];
    GLfloat Output1[MAXPTS][4];
    GLfloat Output2[MAXPTS][4];
    GLdouble OutputD[4];
    GLint Num1;
    GLint Num2;
    glColor3f (0.0, 0.0, 1.0);
int k=0;
for(i=0;i<NUMOBJECTS;i++)
//i=ObjNum;
{
     vector<HomoPoint> Triangulate=ProjPoints[i];
    while(Triangulate.size()>=3)
    {
    Num1=0;

    cout<<"SIZE "<<Triangulate.size()<<endl;
    TriangVertex1[0]=Triangulate.at(0).x;
    TriangVertex1[1]=Triangulate.at(0).y;
    TriangVertex1[2]=Triangulate.at(0).z;
    TriangVertex1[3]=Triangulate.at(0).h;

    TriangVertex2[0]=Triangulate.at(1).x;
    TriangVertex2[1]=Triangulate.at(1).y;
    TriangVertex2[2]=Triangulate.at(1).z;
    TriangVertex2[3]=Triangulate.at(1).h;

    TriangVertex3[0]=Triangulate.at(2).x;
    TriangVertex3[1]=Triangulate.at(2).y;
    TriangVertex3[2]=Triangulate.at(2).z;
    TriangVertex3[3]=Triangulate.at(2).h;

    Suther_Hodg2D(TriangVertex1,TriangVertex2,TriangVertex3,&Output1[0][0],&Num1);

  /*    glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();     // glvertex again multiplies with model view matrix, which we don't want, jugaad
       glMatrixMode(GL_PROJECTION);        // glvertex again multiplies with projection matrix, which we don't want, jugaad
       glLoadIdentity();*/

    int j=0;
    glPointSize(10);
   // glBegin(GL_POLYGON);
   glBegin(GL_POINT);
    for(j=0;j<Num1;j++)
    {
      //  glVertex3f(Output1[j][0]/Output1[j][3],Output1[j][1]/Output1[j][3],Output1[j][2]/Output1[j][3]);
        printf("Vertex %d %lf %lf %lf %lf\n",j,Output1[j][0],Output1[j][1],Output1[j][2],Output1[j][3]);
        OutputD[0]=(GLdouble)Output1[j][0];
        OutputD[1]=(GLdouble)Output1[j][1];
        OutputD[2]=(GLdouble)Output1[j][2];
        OutputD[3]=(GLdouble)Output1[j][3];
        printf("Vertex %d %lf %lf %lf %lf\n",j,OutputD[0],OutputD[1],OutputD[2],OutputD[3]);
        EnterClippedPoints(i,OutputD);
    }
    glEnd();

    Triangulate.erase(Triangulate.begin()+1);

    //k++;
    }

    }
}

void CalculateVertex(int ObjNum)
{
    printf("TRANSFORMED VERTEX\n");
    glGetDoublev(GL_PROJECTION_MATRIX,&ProjMatrix[0][0]);
    glGetDoublev(GL_MODELVIEW_MATRIX,&ModelMatrix[0][0]);

    vector<HomoPoint>::iterator it;
    for ( it=ObjPoints[ObjNum].begin() ; it < ObjPoints[ObjNum].end(); it++ )
    {
        GLfloat Vertex0[4];
        GLfloat Output0[4];
        Vertex0[0]=(*it).x;
        Vertex0[1]=(*it).y;
        Vertex0[2]=(*it).z;
        Vertex0[3]=1;
        TranformVertex(Vertex0,Output0);

        HomoPoint Obj;
        Obj.x=Output0[0];
        Obj.y=Output0[1];
        Obj.z=Output0[2];
        Obj.h=Output0[3];

        ProjPoints[ObjNum].push_back(Obj);



        printf("Vertex %lf %lf %lf %lf\n",Obj.x,Obj.y,Obj.z,Obj.h);
    }
    printf("\n\n");
}

void TranformVertex(GLfloat *Vertex,GLfloat *Output)
{
    int i=0,j=0;
    GLfloat Temp1[4];

    for(i=0;i<4;i++)
    {
        Temp1[i]=0;
        for(j=0;j<4;j++)
        {
            Temp1[i]+=ModelMatrix[j][i]*Vertex[j];      //in column major form

        }
        //Output[i]=Temp1[i];
    }

    for(i=0;i<4;i++)
    {
        Output[i]=0;
        for(j=0;j<4;j++)
        {
            Output[i]+=ProjMatrix[j][i]*Temp1[j];      //in column major form

        }
    }
}



void EnterClippedPoints(int ObjNum,GLdouble * Pt)   //check for duplicate
{
    bool flag=false;
    /*vector<HomoPoint>::iterator it;
    for ( it=ClippedPoints[ObjNum].begin() ; it < ClippedPoints[ObjNum].end(); it++ )
    {
       // if((Pt[0]==(*it).x)&&(Pt[1]==(*it).y)&&(Pt[2]==(*it).z)&&(Pt[3]==(*it).h))
        if(FloatComparison(Pt[0],(*it).x)&&FloatComparison(Pt[1],(*it).y)&&FloatComparison(Pt[2],(*it).z)&&FloatComparison(Pt[3],(*it).h))
        {
            flag=true;
            break;
        }
    }*/
    if(!flag)
    {
        HomoPoint Obj;
        Obj.x=Pt[0];
        Obj.y=Pt[1];
        Obj.z=Pt[2];
        Obj.h=Pt[3];
        ClippedPoints[ObjNum].push_back(Obj);
    }
    cout<<"CLIPPED SIZE "<<ClippedPoints[ObjNum].size()<<endl;
}


void StoreInfo()
{
    int i=0,j=0;
    for(i=0;i<NUMOBJECTS;i++)
    {
        j=0;
        vector<HomoPoint>::iterator it;
        for ( it=ViewPortPoints[i].begin() ; it < ViewPortPoints[i].end(); it++ )         // making vertex table
        {
            Point Obj;
            Obj.Num=j;
            Obj.x=((*it).x)/((*it).h);
            Obj.y=((*it).y)/((*it).h);
            Obj.z=((*it).z)/((*it).h);

            VertexTable[i].push_back(Obj);
        }

        for(j=0;j<VertexTable[i].size();j++)        // making edge table
        {
            Info Obj;
            Obj.Num=j;
            Obj.InfoVect.push_back(j);
            Obj.InfoVect.push_back((j+1)%VertexTable[i].size());
            Obj.SurfaceVect.push_back(0);     // assumingthis scene has one surface per object
            Obj.ObjNum=i;



            GLdouble X1,X2,Y1,Y2,IS;
            X1=VertexTable[i].at(j).x;
            Y1=VertexTable[i].at(j).y;

            X2=VertexTable[i].at((j+1)%VertexTable[i].size()).x;
            Y2=VertexTable[i].at((j+1)%VertexTable[i].size()).y;

            IS=(X1-X2)/(Y1-Y2);
            Obj.InverseSlope=IS;

            EdgeTable[i].push_back(Obj);
        }


        Info ObjSurf;
        ObjSurf.Num=0;

        for(j=0;j<EdgeTable[i].size();j++)        // making surface table
        {
            ObjSurf.InfoVect.push_back(j);
            ObjSurf.ObjNum=i;
        }
        SurfaceEq(&ObjSurf,i);
        SurfaceTable[i].push_back(ObjSurf);


    }
}

void SurfaceEq(Info * SurfaceObj,int ObjNum) // wiki page on determinant and Planes
{
    GLdouble Vertex[4][3];         //4 because  after clipping vertex increases
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

        if(i==4)   // max 4 vertexes
        break;
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


void ScanLineInterface()
{
    glMatrixMode(GL_MODELVIEW);
       glLoadIdentity();     // glvertex again multiplies with model view matrix, which we don't want, jugaad
       glMatrixMode(GL_PROJECTION);        // glvertex again multiplies with projection matrix, which we don't want, jugaad
       glLoadIdentity();


    GLdouble ProjMatrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX,ProjMatrix);

    GLdouble ModelMatrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX,ModelMatrix);

    GLint ViewPort[4];
    glGetIntegerv(GL_VIEWPORT,ViewPort);



    GLdouble ObjCoord1[3],WinCoord1[3],ObjCoord2[3],WinCoord2[3];
    GLdouble Pix_Origin_Obj[3];



      gluUnProject(0,0,0.5,ModelMatrix,ProjMatrix,ViewPort,&Pix_Origin_Obj[0],&Pix_Origin_Obj[1],&Pix_Origin_Obj[2]);

       printf("Origin %lf,%lf,%lf\n",Pix_Origin_Obj[0],Pix_Origin_Obj[1],Pix_Origin_Obj[2]);   //gives square pixels values, its is same in x and y

     glReadPixels(0,0,winWidth,winHeight,GL_RGB,GL_FLOAT,FB);          //reads frame buffer
     //InfoPrint();

     ScanLineAlgo();


     int OriX=Pix_Origin_Obj[0];
     int OriY=Pix_Origin_Obj[1];
    /*glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();*/
    printf("Origin X %d %d\n",OriX,OriY);
    glRasterPos2i(Pix_Origin_Obj[0],Pix_Origin_Obj[1]);
  //  glRasterPos2d(1,1);
    glDrawPixels(winWidth,winHeight,GL_RGB,GL_FLOAT,FB);     //stores changed values in frmae buffer
glutSwapBuffers();
}


void ClippedToViewPortVertex()
{
    printf("ViewPort VERTEX\n");
    GLint ViewPort[4];
    glGetIntegerv(GL_VIEWPORT,ViewPort);

    ViewPortMatrix[0][0]=(ViewPort[2]-ViewPort[0])/2.0;
    ViewPortMatrix[0][1]=0;
    ViewPortMatrix[0][2]=0;
    ViewPortMatrix[0][3]=(ViewPort[2]+ViewPort[0])/2.0;

    ViewPortMatrix[1][0]=0;
    ViewPortMatrix[1][1]=(ViewPort[3]-ViewPort[1])/2.0;
    ViewPortMatrix[1][2]=0;
    ViewPortMatrix[1][3]=(ViewPort[3]+ViewPort[1])/2.0;

    ViewPortMatrix[2][0]=0;
    ViewPortMatrix[2][1]=0;
    ViewPortMatrix[2][2]=1.0/2.0;
    ViewPortMatrix[2][3]=1.0/2.0;

    ViewPortMatrix[3][0]=0;
    ViewPortMatrix[3][1]=0;
    ViewPortMatrix[3][2]=0;
    ViewPortMatrix[3][3]=1;


    int ObjNum=0;

    for(ObjNum=0;ObjNum<NUMOBJECTS;ObjNum++)
  // ObjNum=3;
    {
        vector<HomoPoint>::iterator it;
        for ( it=ClippedPoints[ObjNum].begin() ; it < ClippedPoints[ObjNum].end(); it++ )
        {
        GLfloat Vertex0[4];
        GLfloat Output0[4];
        Vertex0[0]=(*it).x;
        Vertex0[1]=(*it).y;
        Vertex0[2]=(*it).z;
        Vertex0[3]=(*it).h;
        ViewPortTranformVertex(Vertex0,Output0);

        HomoPoint Obj;
        Obj.x=Output0[0];
        Obj.y=Output0[1];
        Obj.z=Output0[2];
        Obj.h=Output0[3];

        ViewPortPoints[ObjNum].push_back(Obj);

        printf("Vertex %lf %lf %lf %lf\n",Obj.x,Obj.y,Obj.z,Obj.h);
        }
        printf("\n\n");
    }
}

void ViewPortTranformVertex(GLfloat *Vertex,GLfloat *Output)
{
    int i=0,j=0;
    GLfloat Temp1[4];

    for(i=0;i<4;i++)
    {
        Temp1[i]=0;
        for(j=0;j<4;j++)
        {
            Temp1[i]+=ViewPortMatrix[i][j]*Vertex[j];      //in row  major form

        }
        Output[i]=Temp1[i];
    }

    /*for(i=0;i<4;i++)
    {
        Output[i]=0;
        for(j=0;j<4;j++)
        {
            Output[i]+=ProjMatrix[j][i]*Temp1[j];      //in column major form

        }
    }*/
}


bool FloatComparison(GLdouble x1,GLdouble x2)   // true if same, otherwise false
{
    if(fabs(x1-x2)>=0.001)
    {
        return false;
    }else{
        return true;
    }
}

void ViewPortToModelView()
{
    cout<< "WORLD COORDINATES OF CLIPPED POINTS"<<endl;
    GLdouble ProjMatrix[16];
    glGetDoublev(GL_PROJECTION_MATRIX,ProjMatrix);

    GLdouble ModelMatrix[16];
    glGetDoublev(GL_MODELVIEW_MATRIX,ModelMatrix);

    GLint ViewPort[4];
    glGetIntegerv(GL_VIEWPORT,ViewPort);



    GLdouble ModelViewPoint[3];
    int ObjNum=0;
    for(ObjNum=0;ObjNum<NUMOBJECTS;ObjNum++)
    {
        vector<HomoPoint>::iterator it;
        for ( it=ViewPortPoints[ObjNum].begin() ; it < ViewPortPoints[ObjNum].end(); it++ )
        {
        gluUnProject((*it).x/(*it).h,(*it).y/(*it).h,(*it).z/(*it).h,ModelMatrix,ProjMatrix,ViewPort,&ModelViewPoint[0],&ModelViewPoint[1],&ModelViewPoint[2]);
        Point Obj;
        Obj.x=ModelViewPoint[0];
        Obj.y=ModelViewPoint[1];
        Obj.z=ModelViewPoint[2];
        cout<<"Vertex "<<ObjNum<<" "<<Obj.x<<" "<<Obj.y<<" "<<Obj.z<<endl;
        ModeViewPoints[ObjNum].push_back(Obj);
        }
    }
}


void CalculateNormal()
{
    //printf("TRANSFORMED NORMAL\n");
    int i=0;
    for(i=0;i<NUMOBJECTS;i++)
    {
        if(ModeViewPoints[i].size()<3)
        {
            continue;
        }
        GLdouble Output0[4],Output1[4],Output2[4];

        Output0[0]=ModeViewPoints[i].at(0).x;
        Output0[1]=ModeViewPoints[i].at(0).y;
        Output0[2]=ModeViewPoints[i].at(0).z;

        Output1[0]=ModeViewPoints[i].at(1).x;
        Output1[1]=ModeViewPoints[i].at(1).y;
        Output1[2]=ModeViewPoints[i].at(1).z;

        Output2[0]=ModeViewPoints[i].at(2).x;
        Output2[1]=ModeViewPoints[i].at(2).y;
        Output2[2]=ModeViewPoints[i].at(2).z;




        GLdouble Vector1[3]={Output1[0]-Output0[0],Output1[1]-Output0[1],Output1[2]-Output0[2]};
        GLdouble Vector2[3]={Output2[0]-Output0[0],Output2[1]-Output0[1],Output2[2]-Output0[2]};

        GLdouble Normal[3];

        CrossProduct(Vector1,Vector2,Normal);
        Normalize(Normal);


        TransformedNormal[i][0]=Normal[0];
        TransformedNormal[i][1]=Normal[1];
        TransformedNormal[i][2]=Normal[2];

        printf("Face %d %lf %lf %lf\n",i,TransformedNormal[i][0],TransformedNormal[i][1],TransformedNormal[i][2]);
    }
    //printf("\n\n");
}


void LightEachVertex()
{
    printf("LIGHT INTENSITIES\n");
    /*int i=0;
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
    printf("\n\n");*/

    int index=0,i=0;

    for(i=0;i<NUMOBJECTS;i++)
    {
        index=0;
        vector<Point>::iterator it;
        for ( it=ModeViewPoints[i].begin() ; it < ModeViewPoints[i].end(); it++ )
        {
            GLdouble ViewerVector[3];
            ViewerVector[0]=EyePos[VIEWPOS][0]-(*it).x;
            ViewerVector[1]=EyePos[VIEWPOS][1]-(*it).y;
            ViewerVector[2]=EyePos[VIEWPOS][2]-(*it).z;
            Normalize(ViewerVector);

            GLfloat ViewerVectorF[3];
            ViewerVectorF[0]=ViewerVector[0];
            ViewerVectorF[1]=ViewerVector[1];
            ViewerVectorF[2]=ViewerVector[2];
            Normalize(ViewerVector);

            GLfloat TranformedCoord[3];

            TranformedCoord[0]=(*it).x;
            TranformedCoord[1]=(*it).y;
            TranformedCoord[2]=(*it).z;

            GLfloat Normal[3];
            Normal[0]=TransformedNormal[i][0];
            Normal[1]=TransformedNormal[i][1];
            Normal[2]=TransformedNormal[i][2];

            if(i==2)
            {
                AvgNormal(TranformedCoord,3,Normal);

            }

            if(i==3)    // calculating average normal for object 2 and 3
            {
                AvgNormal(TranformedCoord,2,Normal);

            }


            GLfloat LightAtVertex[3];

            LightCalculations(TranformedCoord,diffuseCoeff[i],blackcolor,diffuseCoeff[i],light_diffuse, Normal,light_position,specularCoeff,whitecolor,ViewerVectorF,Shine,LightAtVertex);

            cout<<"Light Intensity "<<i<<" "<<index<<" "<<LightAtVertex[0]<<" "<<LightAtVertex[1]<<" "<<LightAtVertex[2]<<endl;

            VertexTable[i].at(index).Color[0]=LightAtVertex[0];
            VertexTable[i].at(index).Color[1]=LightAtVertex[1];
            VertexTable[i].at(index).Color[2]=LightAtVertex[2];


            index++;
        }
    }
}

void AvgNormal(GLfloat * V1,int i,GLfloat *Normal)
{
    vector<Point>::iterator it;
    bool found=false;
    for ( it=ModeViewPoints[i].begin() ; it < ModeViewPoints[i].end(); it++ )
    {
        if(((V1[0]-(*it).x)<0.1)&&((V1[1]-(*it).y)<0.1)&&((V1[2]-(*it).z)<0.1))
        {
            found=true;
        }
    }
    if(found)
    {
        cout<<"Average Normal"<<endl;
    Normal[0]+=TransformedNormal[i][0];
    Normal[1]+=TransformedNormal[i][1];
    Normal[2]+=TransformedNormal[i][2];

    Normal[0]/=2;      // average normal
    Normal[1]/=2;
    Normal[2]/=2;
    cout<<"Avg "<<Normal[0]<<" "<<Normal[1]<<" "<<Normal[2]<<endl;
    }
}
