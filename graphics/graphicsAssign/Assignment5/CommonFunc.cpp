#include "CommonFunc.h"
#include"math.h"
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


void CrossProduct(GLfloat *V1,GLfloat *V2,GLfloat *V3)       //evaluates V1xV2 stores in V3
{
    V3[0]=V1[1]*V2[2]-V1[2]*V2[1];
    V3[1]=V1[2]*V2[0]-V1[0]*V2[2];
    V3[2]=V1[0]*V2[1]-V1[1]*V2[0];
}

void DotProduct(GLfloat *V1,GLfloat *V2,GLfloat *Output)
{
    *Output=V1[0]*V2[0]+V1[1]*V2[1]+V1[2]*V2[2];
}

