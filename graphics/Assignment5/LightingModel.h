#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
#ifndef LIGHTINGMODEL_H_INCLUDED
#define LIGHTINGMODEL_H_INCLUDED
void LightCalculations(GLfloat * Vertex,GLfloat * AmbCoeff,GLfloat *AmbIntensity,GLfloat * DiffCoeff,GLfloat *DiffIntensity,GLfloat *NormalVect,GLfloat *LightPos,GLfloat *SpecCoeff,GLfloat *SpecIntensity,GLfloat *ViewerVect,GLfloat Shine,GLfloat *Output);
void AmbientLightCalc(GLfloat * Coeff,GLfloat *Intensity,GLfloat *Output);
void DiffuseLightCalc(GLfloat *Vertex,GLfloat * Coeff,GLfloat *Intensity,GLfloat *NormalVect,GLfloat *LightPos,GLfloat *Output);
void SpecularLightCalc(GLfloat *Vertex,GLfloat *Coeff,GLfloat *Intensity,GLfloat *ViewerVect,GLfloat *LightPos,GLfloat *NormalVect,GLfloat Shine,GLfloat *Output);

#endif // LIGHTINGMODEL_H_INCLUDED
