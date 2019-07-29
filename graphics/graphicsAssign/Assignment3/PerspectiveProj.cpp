#include<GL/glut.h>
#include<math.h>
#include<stdio.h>

#include <GL/gl.h>





#define MATRIX_SIZE 4
#define PI 3.14159265

//function decalaration
void CrossProduct(GLfloat *V1,GLfloat *V2,GLfloat *V3);
void WorldToViewTransform(GLfloat x0,GLfloat y0,GLfloat z0,GLfloat xref,GLfloat yref,GLfloat zref,GLfloat Vx,GLfloat Vy,GLfloat Vz);
void Normalize(GLfloat * N);
void ConvertColumnMajor(GLfloat *PtrSrc,GLfloat *PtrDst);
void myglFrustum(GLfloat xwmin,GLfloat xwmax,GLfloat ywmin,GLfloat ywmax,GLfloat dnear,GLfloat dfar);
void myglPerspective(GLfloat theta,GLfloat aspect,GLfloat dnear,GLfloat dfar);
void cubedisplay (void);
void myinit (void);

//global varibales
GLfloat light_diffuse[] = {1.0, 0.0, 0.0, 1.0};  /* Red diffuse light. */
//GLfloat light_position[] = {1.0, 1.0, 1.0, 0.0};  /* Infinite light location. */
GLfloat light_position[] = {1.0, 1.0, 1.0, 1.0};

//my parameters
GLfloat blackcolor[]={0.0,0.0,0.0,1.0};
GLfloat whitecolor[]={1.0,1.0,1.0,1.0};
GLfloat diffuseCoeff[]={1.0,0.4,0.9,1.0};
GLfloat specularCoeff[]={1.0,1.0,1.0,1.0};

//GLfloat light_position[] = {0.0, 0.0, -1.0, 0.0};
GLfloat n[6][3] = {  /* Normals for the 6 faces of a cube. */
  {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
  {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0} };
GLint faces[6][4] = {  /* Vertex indices for the 6 faces of a cube. */
  {0, 1, 2, 3}, {3, 2, 6, 7}, {7, 6, 5, 4},
  {4, 5, 1, 0}, {5, 6, 2, 1}, {7, 4, 0, 3} };
GLfloat v[8][3];  /* Will be filled in with X,Y,Z vertexes. */

GLfloat ColorStore[8][3];
GLdouble WindowCoord[8][3];
GLdouble ModelMatrix[16];



void MatrixMult(GLfloat *Mat2,GLfloat *Output);
void FindBackFaces();
void DotProduct(GLfloat *V1,GLfloat *V2,GLfloat *Output);

void
drawBox(void)
{
  int i;


  for (i = 0; i < 6; i++) {

      glBegin(GL_QUADS);
    glNormal3fv(&n[i][0]);

    glColor3f (1.0, 0.0, 0.0);
    glVertex3fv(&v[faces[i][0]][0]);

    glColor3f (0.0, 1.0, 0.0);
    glVertex3fv(&v[faces[i][1]][0]);

    glColor3f (0.0, 0.0, 1.0);
    glVertex3fv(&v[faces[i][2]][0]);

    glColor3f (0.0, 1.0, 1.0);
    glVertex3fv(&v[faces[i][3]][0]);
    glEnd();

  }
 // printf("Normal %lf\n",n[0][0]);
}



void
display(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  drawBox();
 // FindBackFaces();
  glutSwapBuffers();
}


void
init(void)
{
  /* Setup cube vertex data. */
  v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
  v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
  v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
  v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
  v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
  v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;

  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);



  /*glLightfv(GL_LIGHT0,GL_AMBIENT,blackcolor);
  glLightfv(GL_LIGHT0,GL_SPECULAR,whitecolor);

  glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,diffuseCoeff);
  glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,specularCoeff);
  glMaterialf(GL_FRONT_AND_BACK,GL_SHININESS,100.0);*/



  glEnable(GL_LIGHT0);
  //glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
  //gluPerspective( /* field of view in degree */ 40.0,
   // /* aspect ratio */ 1.0,
   // /* Z near */ 1.0, /* Z far */ 10.0);

    myglPerspective(/* field of view in degree */ 40.0,
                    /* aspect ratio */ 1.0,
                    /* Z near */ 1.0, /* Z far */ 10.0);



  glMatrixMode(GL_MODELVIEW);


  //gluLookAt(0.0, 0.0, 8.0,  /* eye is at (0,0,5) */
    //0.0, 0.0, 0.0,      /* center is at (0,0,0) */
    //0.0, 1.0, 0.0);      /* up is in positive Y direction */

    WorldToViewTransform(0.0, 0.0, 8.0,      /* eye is at (0,0,5) */
                        0.0, 0.0, 0.0,         /* center is at (0,0,0) */
                        0.0, 1.0, 0.0           /* up is in positive Y direction */
                        );


  /* Adjust cube position to be asthetic angle. */
  glTranslatef(0.0, 0.0, -2.0);        //these functions also need to be replaced by ours equivalent functions
  glRotatef(60, 1.0, 0.0, 0.0);
  glRotatef(-20, 0.0, 0.0, 1.0);
  glShadeModel (GL_SMOOTH);
}


int
main(int argc, char **argv)
{
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutCreateWindow("red 3D lighted cube");
  glutDisplayFunc(display);
  init();
  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}



void CrossProduct(GLfloat *V1,GLfloat *V2,GLfloat *V3)       //evaluates V1xV2 stores in V3
{
    V3[0]=V1[1]*V2[2]-V1[2]*V2[1];
    V3[1]=V1[2]*V2[0]-V1[0]*V2[2];
    V3[2]=V1[0]*V2[1]-V1[1]*V2[0];
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


void Normalize(GLfloat * N)
{
    GLfloat mod=sqrt(pow(N[0],2)+pow(N[1],2)+pow(N[2],2));
    N[0]/=mod;
    N[1]/=mod;
    N[2]/=mod;
}


void ConvertColumnMajor(GLfloat *PtrSrc,GLfloat *PtrDst)
{
	int i=0,j=0;
	for(i=0;i<MATRIX_SIZE;i++)
	{
		for(j=0;j<MATRIX_SIZE;j++)
		{
			*(PtrDst+MATRIX_SIZE*j+i)=*(PtrSrc+MATRIX_SIZE*i+j);
		}
	}
}

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


/*void FindBackFaces()
{


    glGetDoublev(GL_MODELVIEW_MATRIX,ModelMatrix);
    GLfloat Output1[4],Output2[4],Output3;
int i=0;
for(i=0;i<6;i++)
{
    GLfloat Point[]={n[i][0],n[i][1],n[i][2],1};
    MatrixMult(Point,Output1);

    printf("Point %lf %lf %lf %lf\n",Output1[0],Output1[1],Output1[2],Output1[3]);

GLfloat Ori[]={0,0,-1,1};
    MatrixMult(Ori,Output2);

    printf("Point %lf %lf %lf %lf\n",Output2[0],Output2[1],Output2[2],Output2[3]);

    DotProduct(Output1,Output2,&Output3);
    if(Output3<0)

    printf("COS %lf %d \n",Output3,i);
}
}

void MatrixMult(GLfloat *Mat2,GLfloat *Output)
{
    int i=0,j=0;
    GLfloat ModView[4][4];
    for(i=0;i<4;i++)
    {
        for(j=0;j<4;j++)
        {
            ModView[i][j]=ModelMatrix[i*4+j];

        }
    }

    for(i=0;i<4;i++)
    {
        Output[i]=0;
        for(j=0;j<4;j++)
        {
            Output[i]+=ModView[i][j]*Mat2[j];

        }
    }
}

void DotProduct(GLfloat *V1,GLfloat *V2,GLfloat *Output)
{
    *Output=V1[0]*V2[0]+V1[1]*V2[1]+V1[2]*V2[2];
}*/


