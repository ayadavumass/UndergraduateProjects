#include "Bresenham.h"
#include <math.h>
#include<stdio.h>
void Bresenham_Line(GLdouble * Vertex1, GLdouble * Vertex2)
{

    int x0=Vertex1[0];
     int y0=Vertex1[1];

     int xEnd=Vertex2[0];
     int yEnd=Vertex2[1];

     bool acute=true;

     printf("Z Corrd %lf %lf %lf, %lf %lf %lf\n  ",Vertex1[0],Vertex1[1],Vertex1[2],Vertex2[0],Vertex2[1],Vertex2[2]);

     if(((yEnd-y0)/(xEnd-x0))<0)
     return;

     int dx=fabs(xEnd-x0), dy=fabs(yEnd-y0);
     //int dx=xEnd-x0, dy=yEnd-y0;

     int p=2*dy - dx;
     int twoDy=2*dy,twoDyMinusDx=2*(dy-dx);
     int x,y;

     /* Determne which end point to use as start position */
     if(x0>xEnd)
     {
         x=xEnd;
         y=yEnd;
         xEnd=x0;

     }else{
         x=x0;
         y=y0;
     }
     printf("Frame Buffer %lf %lf %lf\n",FB[x][y][0],FB[x][y][1],FB[x][y][2]);


     FB[y][x][0]=1.0;
     FB[y][x][1]=1.0;
     FB[y][x][2]=0.0;

     while(x<xEnd) {
         //printf("Here\n");
         x++;
         if(p<0)
            p+=twoDy;
            else{

                y++;

                p+=twoDyMinusDx;
            }
            FB[y][x][0]=1.0;
            FB[y][x][1]=1.0;
            FB[y][x][2]=0.0;

     }
}
