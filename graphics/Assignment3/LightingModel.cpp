#include"LightingModel.h"
#include<math.h>
#include"CommonFunc.h"
void LightCalculations(GLfloat * Vertex,GLfloat * AmbCoeff,GLfloat *AmbIntensity,GLfloat * DiffCoeff,GLfloat *DiffIntensity,GLfloat *NormalVect,GLfloat *LightPos,GLfloat *SpecCoeff,GLfloat *SpecIntensity,GLfloat *ViewerVect,GLfloat Shine,GLfloat *Output)
{
    GLfloat AmbCon[3],DiffCon[3],SpecCon[3];
    if(LightPos[3]==1.0)         // for local light source
    {
        AmbientLightCalc(AmbCoeff,AmbIntensity,AmbCon);
        DiffuseLightCalc(Vertex,DiffCoeff,DiffIntensity,NormalVect,LightPos,DiffCon);
        SpecularLightCalc(Vertex,SpecCoeff,SpecIntensity,ViewerVect,LightPos,NormalVect,Shine,SpecCon);
        Output[0]=AmbCon[0]+DiffCon[0]+SpecCon[0];
        Output[1]=AmbCon[1]+DiffCon[1]+SpecCon[1];
        Output[2]=AmbCon[2]+DiffCon[2]+SpecCon[2];
    }
}

void AmbientLightCalc(GLfloat * Coeff,GLfloat *Intensity,GLfloat *Output)
{
    Output[0]=Coeff[0]*Intensity[0];
    Output[1]=Coeff[1]*Intensity[1];
    Output[2]=Coeff[2]*Intensity[2];
}

void DiffuseLightCalc(GLfloat *Vertex,GLfloat * Coeff,GLfloat *Intensity,GLfloat *NormalVect,GLfloat *LightPos,GLfloat *Output)
{
    GLfloat LightVect[]={LightPos[0]-Vertex[0],LightPos[1]-Vertex[1],LightPos[2]-Vertex[2]};
     Normalize(LightVect);
     GLfloat Cos;
    DotProduct(NormalVect,LightVect,&Cos);

    Output[0]=Coeff[0]*Intensity[0]*Cos;
    Output[1]=Coeff[1]*Intensity[1]*Cos;
    Output[2]=Coeff[2]*Intensity[2]*Cos;

    if(Cos<=0)
    {
         Output[0]=0;
        Output[1]=0;
        Output[2]=0;
    }
}

void SpecularLightCalc(GLfloat *Vertex,GLfloat *Coeff,GLfloat *Intensity,GLfloat *ViewerVect,GLfloat *LightPos,GLfloat *NormalVect,GLfloat Shine,GLfloat *Output)
{
     GLfloat LightVect[]={LightPos[0]-Vertex[0],LightPos[1]-Vertex[1],LightPos[2]-Vertex[2]};
      Normalize(LightVect);
      GLfloat Cos;
    DotProduct(NormalVect,LightVect,&Cos);
    GLfloat R[]={2*Cos*NormalVect[0]-LightVect[0],2*Cos*NormalVect[1]-LightVect[1],2*Cos*NormalVect[2]-LightVect[2]};
    Normalize(R);

    GLfloat VR;
    DotProduct(ViewerVect,R,&VR);

    Output[0]=Coeff[0]*Intensity[0]*pow(VR,Shine);
    Output[1]=Coeff[1]*Intensity[1]*pow(VR,Shine);
    Output[2]=Coeff[2]*Intensity[2]*pow(VR,Shine);
    if(Cos<=0||VR<=0)
    {
         Output[0]=0;
        Output[1]=0;
        Output[2]=0;
    }
}
