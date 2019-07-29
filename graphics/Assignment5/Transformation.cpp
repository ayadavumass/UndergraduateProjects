
#include <GL/glut.h>
#include <stdlib.h>
#include<math.h>
#include<iostream>
#include "Transformation.h"
using namespace std;

/*  Initialize material property and light source.
 */
#define MATRIX_SIZE	4
#define PI 3.14159265





/*Global Variables*/
/*Roatation Matrix*/
GLfloat Rx[MATRIX_SIZE][MATRIX_SIZE];
GLfloat RxI[MATRIX_SIZE][MATRIX_SIZE];
GLfloat Ry[MATRIX_SIZE][MATRIX_SIZE];
GLfloat RyI[MATRIX_SIZE][MATRIX_SIZE];
GLfloat Rz[MATRIX_SIZE][MATRIX_SIZE];
GLfloat ColMajor[MATRIX_SIZE*MATRIX_SIZE];

/*Translation matrix*/
GLfloat TranslationM[MATRIX_SIZE][MATRIX_SIZE];

/*Scale Matrix*/
GLfloat ScaleM[MATRIX_SIZE][MATRIX_SIZE];

/*Shear Matrix*/
GLfloat ShearM[MATRIX_SIZE][MATRIX_SIZE];


/*Functions */
void Normalize(GLfloat *x,GLfloat *y,GLfloat *z);
void ConvertColumnMajor(GLfloat *PtrSerc,GLfloat *PtrDst);
void myRotate(GLfloat degree,GLfloat x,GLfloat y,GLfloat z);               // one fixed point is assumed to be origin, if wanted for arbitary point then first translate
void InverseByTranspose(GLfloat *PtrSrc,GLfloat *PtrDst);
void FillMatrixY(GLfloat ctheta,GLfloat stheta,GLfloat *Ptr);
void FillMatrixX(GLfloat ctheta,GLfloat stheta,GLfloat *Ptr);
void FillMatrixZ(GLfloat ctheta,GLfloat stheta,GLfloat *Ptr);
void printMatrix(GLfloat *Ptr);

void myTranslate(GLfloat x,GLfloat y,GLfloat z);
void FillTranslationM(GLfloat x,GLfloat y,GLfloat z,GLfloat *Ptr);

void myScale(GLfloat x,GLfloat y,GLfloat z);
void FillScaleM(GLfloat x,GLfloat y,GLfloat z,GLfloat *Ptr);

void myShear(int ShearType,GLfloat degree);
void FillShearM(GLfloat *Ptr);










void printMatrix(GLfloat *Ptr)
{
	int i=0,j=0;
	for(i=0;i<MATRIX_SIZE;i++)
	{
		for(j=0;j<MATRIX_SIZE;j++)
		{
			cout<<Ptr[MATRIX_SIZE*i+j]<<" ";
		}
	}
	cout<<endl;
}

void myShear(int ShearType,GLfloat degree)
{
	switch(ShearType)
	{
		case SXY:
		{
			GLfloat x = 1.0;
			GLfloat y = 0.0;
			GLfloat rotatedX = ( cos(degree*PI/180) * x ) - ( sin(degree*PI/180) * y );
			GLfloat rotatedY = ( sin(degree*PI/180) * x ) + ( cos(degree*PI/180) * y );

			// now normalize Y to find where it is at when X crosses over the 1.0 unit mark
			// this is the part I was missing before
			GLfloat shearDistanceY = ( 1.0f / rotatedX ) * rotatedY;
 			FillShearM(&ShearM[0][0]);
			ShearM[1][0]=shearDistanceY;
			ConvertColumnMajor(&ShearM[0][0],ColMajor);
			glMultMatrixf(ColMajor);


		}
		break;
		case SYX:
		{
			GLfloat x = 0.0;
			GLfloat y = 1.0;
			GLfloat rotatedX = ( cos(degree*PI/180) * x ) + ( sin(degree*PI/180) * y );
			GLfloat rotatedY = -( sin(degree*PI/180) * x ) + ( cos(degree*PI/180) * y );

			// now normalize Y to find where it is at when X crosses over the 1.0 unit mark
			// this is the part I was missing before
			GLfloat shearDistanceX = ( 1.0f / rotatedY ) * rotatedX;
 			FillShearM(&ShearM[0][0]);
			ShearM[0][1]=shearDistanceX;
			ConvertColumnMajor(&ShearM[0][0],ColMajor);
			glMultMatrixf(ColMajor);
		}
		break;
		case SYZ:
		{
			GLfloat y = 1.0;
			GLfloat z = 0.0;
			GLfloat rotatedY = ( cos(degree*PI/180) * y ) - ( sin(degree*PI/180) * z );
			GLfloat rotatedZ = ( sin(degree*PI/180) * y ) + ( cos(degree*PI/180) * z );

			// now normalize Y to find where it is at when X crosses over the 1.0 unit mark
			// this is the part I was missing before
			GLfloat shearDistanceY = ( 1.0f / rotatedY ) * rotatedZ;
 			FillShearM(&ShearM[0][0]);
			ShearM[2][1]=shearDistanceY;
			ConvertColumnMajor(&ShearM[0][0],ColMajor);
			glMultMatrixf(ColMajor);

		}
		break;
		case SZY:
		{
			GLfloat z = 1.0;
			GLfloat y = 0.0;
			GLfloat rotatedZ = ( cos(degree*PI/180) * z ) - ( sin(degree*PI/180) * y );
			GLfloat rotatedY = ( sin(degree*PI/180) * z ) + ( cos(degree*PI/180) * y );

			// now normalize Y to find where it is at when X crosses over the 1.0 unit mark
			// this is the part I was missing before
			GLfloat shearDistanceZ = ( 1.0f / rotatedZ ) * rotatedY;
 			FillShearM(&ShearM[0][0]);
			ShearM[1][2]=shearDistanceZ;
			ConvertColumnMajor(&ShearM[0][0],ColMajor);
			glMultMatrixf(ColMajor);
		}
		break;
		case SZX:
		{
			GLfloat z = 1.0;
			GLfloat x = 0.0;
			GLfloat rotatedZ = ( cos(degree*PI/180) * z ) - ( sin(degree*PI/180) * x );
			GLfloat rotatedX = ( sin(degree*PI/180) * z ) + ( cos(degree*PI/180) * x );

			// now normalize Y to find where it is at when X crosses over the 1.0 unit mark
			// this is the part I was missing before
			GLfloat shearDistanceZ = ( 1.0f / rotatedZ ) * rotatedX;
 			FillShearM(&ShearM[0][0]);
			ShearM[0][2]=shearDistanceZ;
			ConvertColumnMajor(&ShearM[0][0],ColMajor);
			glMultMatrixf(ColMajor);
		}
		break;
		case SXZ:
		{
			GLfloat x = 1.0;
			GLfloat z = 0.0;
			GLfloat rotatedX = ( cos(degree*PI/180) * x ) - ( sin(degree*PI/180) * z );
			GLfloat rotatedZ = ( sin(degree*PI/180) * x ) + ( cos(degree*PI/180) * z );

			// now normalize Y to find where it is at when X crosses over the 1.0 unit mark
			// this is the part I was missing before
			GLfloat shearDistanceX = ( 1.0f / rotatedX ) * rotatedZ;
 			FillShearM(&ShearM[0][0]);
			ShearM[2][0]=shearDistanceX;
			ConvertColumnMajor(&ShearM[0][0],ColMajor);
			glMultMatrixf(ColMajor);
		}
		break;
	}


}

void FillShearM(GLfloat *Ptr)
{
	int i=0,j=0;
	for(i=0;i<MATRIX_SIZE;i++)
	{
		for(j=0;j<MATRIX_SIZE;j++)
		{
			if(i==j)
			{
				*(Ptr+MATRIX_SIZE*i+j)=1;
			}else
			{
				*(Ptr+MATRIX_SIZE*i+j)=0;
			}
		}
	}

}
void myScale(GLfloat x,GLfloat y,GLfloat z)
{
	FillScaleM(x,y,z,&ScaleM[0][0]);
	ConvertColumnMajor(&ScaleM[0][0],ColMajor);
	glMultMatrixf(ColMajor);

}

void FillScaleM(GLfloat x,GLfloat y,GLfloat z,GLfloat *Ptr)
{
	int i=0,j=0;
	for(i=0;i<MATRIX_SIZE;i++)
	{
		for(j=0;j<MATRIX_SIZE;j++)
		{
			if(i==0&&j==0)
			{
				*(Ptr+MATRIX_SIZE*i+j)=x;
			}else if(i==1&&j==1)
			{
				*(Ptr+MATRIX_SIZE*i+j)=y;
			}else if(i==2&&j==2)
			{
				*(Ptr+MATRIX_SIZE*i+j)=z;
			}else if(i==3&&j==3)
			{
				*(Ptr+MATRIX_SIZE*i+j)=1;
			}else
			{
				*(Ptr+MATRIX_SIZE*i+j)=0;
			}
		}
	}


}
void myTranslate(GLfloat x,GLfloat y,GLfloat z)
{
	FillTranslationM(x,y,z,&TranslationM[0][0]);
	ConvertColumnMajor(&TranslationM[0][0],ColMajor);
	glMultMatrixf(ColMajor);
}

void FillTranslationM(GLfloat x,GLfloat y,GLfloat z,GLfloat *Ptr)
{
	int i=0,j=0;
	for(i=0;i<MATRIX_SIZE;i++)
	{
		for(j=0;j<MATRIX_SIZE;j++)
		{
			if(i==j)
			{
				*(Ptr+MATRIX_SIZE*i+j)=1;
			}else if(i==0&&j==3)
			{
				*(Ptr+MATRIX_SIZE*i+j)=x;
			}else if(i==1&&j==3)
			{
				*(Ptr+MATRIX_SIZE*i+j)=y;
			}else if(i==2&&j==3)
			{
				*(Ptr+MATRIX_SIZE*i+j)=z;
			}else
			{
				*(Ptr+MATRIX_SIZE*i+j)=0;
			}
		}
	}
}

void myRotate(GLfloat degree,GLfloat x,GLfloat y,GLfloat z)
{
	GLfloat ctheta,stheta;
	Normalize(&x,&y,&z);
	GLfloat d=sqrt(pow(y,2)+pow(z,2));

	ctheta=z/d;
	stheta=y/d;

	if(d!=0)
	{
		FillMatrixX(ctheta,stheta,&Rx[0][0]);
		//printMatrix(&Rx[0][0]);
	}
	ctheta=d;
	stheta=-x;
	FillMatrixY(ctheta,stheta,&Ry[0][0]);

	ctheta=cos(degree*PI/180);
	stheta=sin(degree*PI/180);
	FillMatrixZ(ctheta,stheta,&Rz[0][0]);

	if(d!=0)
	{
		InverseByTranspose(&Rx[0][0],&RxI[0][0]);
	}

	InverseByTranspose(&Ry[0][0],&RyI[0][0]);

	if(d!=0)
	{
		ConvertColumnMajor(&RxI[0][0],ColMajor);
		glMultMatrixf(ColMajor);
	}


	ConvertColumnMajor(&RyI[0][0],ColMajor);
	glMultMatrixf(ColMajor);

	ConvertColumnMajor(&Rz[0][0],ColMajor);
	glMultMatrixf(ColMajor);

	ConvertColumnMajor(&Ry[0][0],ColMajor);
	glMultMatrixf(ColMajor);

	if(d!=0)
	{
		ConvertColumnMajor(&Rx[0][0],ColMajor);
		glMultMatrixf(ColMajor);
	}

}

/*void ConvertColumnMajor(GLfloat *PtrSrc,GLfloat *PtrDst)
{
	int i=0,j=0;
	for(i=0;i<MATRIX_SIZE;i++)
	{
		for(j=0;j<MATRIX_SIZE;j++)
		{
			*(PtrDst+MATRIX_SIZE*j+i)=*(PtrSrc+MATRIX_SIZE*i+j);
		}
	}
}*/

void InverseByTranspose(GLfloat *PtrSrc,GLfloat *PtrDst)
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

void FillMatrixY(GLfloat ctheta,GLfloat stheta,GLfloat *Ptr)
{
	int i=0,j=0;
	for(i=0;i<MATRIX_SIZE;i++)
	{
		for(j=0;j<MATRIX_SIZE;j++)
		{
			if((i==0 && j==0) || (i==2&&j==2))
			{
				*(Ptr+MATRIX_SIZE*i+j)=ctheta;
			}else if((i==1&&j==1)||(i==3&&j==3))
			{
				*(Ptr+MATRIX_SIZE*i+j)=1;
			}else if(i==0&&j==2)
			{
				*(Ptr+MATRIX_SIZE*i+j)=stheta;
			}else if(i==2&&j==0)
			{
				*(Ptr+MATRIX_SIZE*i+j)=-stheta;
			}else
			{
				*(Ptr+MATRIX_SIZE*i+j)=0;
			}
		}
	}
}

void FillMatrixX(GLfloat ctheta,GLfloat stheta,GLfloat *Ptr)
{
	int i=0,j=0;
	for(i=0;i<MATRIX_SIZE;i++)
	{
		for(j=0;j<MATRIX_SIZE;j++)
		{
			if((i==0 && j==0) || (i==MATRIX_SIZE-1&&j==MATRIX_SIZE-1))
			{
				*(Ptr+MATRIX_SIZE*i+j)=1;
			}else if((i==1&&j==1)||(i==2&&j==2))
			{
				*(Ptr+MATRIX_SIZE*i+j)=ctheta;
			}else if(i==1&&j==2)
			{
				*(Ptr+MATRIX_SIZE*i+j)=-stheta;
			}else if(i==2&&j==1)
			{
				*(Ptr+MATRIX_SIZE*i+j)=stheta;
			}else
			{
				*(Ptr+MATRIX_SIZE*i+j)=0;
			}
		}
	}
}

void FillMatrixZ(GLfloat ctheta,GLfloat stheta,GLfloat *Ptr)
{
	int i=0,j=0;
	for(i=0;i<MATRIX_SIZE;i++)
	{
		for(j=0;j<MATRIX_SIZE;j++)
		{
			if((i==0 && j==0) || (i==1&&j==1))
			{
				*(Ptr+MATRIX_SIZE*i+j)=ctheta;
			}else if((i==2&&j==2)||(i==3&&j==3))
			{
				*(Ptr+MATRIX_SIZE*i+j)=1;
			}else if(i==0&&j==1)
			{
				*(Ptr+MATRIX_SIZE*i+j)=-stheta;
			}else if(i==1&&j==0)
			{
				*(Ptr+MATRIX_SIZE*i+j)=stheta;
			}else
			{
				*(Ptr+MATRIX_SIZE*i+j)=0;
			}
		}
	}
}

void Normalize(GLfloat *x,GLfloat *y,GLfloat *z)
{
	GLfloat tx,ty,tz;
	tx=*x,ty=*y,tz=*z;
	GLfloat d=sqrt(pow(tx,2)+pow(ty,2)+pow(tz,2));
	*x=tx/d;
	*y=ty/d;
	*z=tz/d;
}

