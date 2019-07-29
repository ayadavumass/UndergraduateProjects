#include"Clipping.h"
#include<stdio.h>

#define MAXPTS  6

void Suther_Hodg2D(GLfloat * TriangVertex1,GLfloat * TriangVertex2,GLfloat * TriangVertex3,GLfloat *Output,GLint *NumVertices)    //in anticlockwise order
{
    GLfloat Temp[MAXPTS][4];

    Temp[0][0]=TriangVertex1[0];
    Temp[0][1]=TriangVertex1[1];
    Temp[0][2]=TriangVertex1[2];
    Temp[0][3]=TriangVertex1[3];


    Temp[1][0]=TriangVertex2[0];
    Temp[1][1]=TriangVertex2[1];
    Temp[1][2]=TriangVertex2[2];
    Temp[1][3]=TriangVertex2[3];


    Temp[2][0]=TriangVertex3[0];
    Temp[2][1]=TriangVertex3[1];
    Temp[2][2]=TriangVertex3[2];
    Temp[2][3]=TriangVertex3[3];

     GLfloat TempNew[MAXPTS][4];

    int Num=3,i=0,j=0;
    int NumNew=0;

    while(j<6)    // 6 clippers   j=0 z=1; j=1 z=-1;j=2 y=1; j=3 y=-1; j=4 x=1; j=5 x=-1;
    {
        NumNew=0;
        for(i=0;i<Num;i++)
        {
            GLint Ret;
            Inside_Outside(j,Temp[i%Num],Temp[(i+1)%Num],&Ret);
            printf("Ret %d\n",Ret);

            switch(Ret)
            {
                case 1:
                {
                    GLfloat IP[4];
                    IntersectionPoint(j,Temp[i%Num],Temp[(i+1)%Num],IP);

                    TempNew[NumNew][0]=IP[0];
                    TempNew[NumNew][1]=IP[1];
                    TempNew[NumNew][2]=IP[2];
                    TempNew[NumNew][3]=IP[3];
                    NumNew++;

                }
                break;
                case 2:
                {
                    TempNew[NumNew][0]=Temp[(i+1)%Num][0];
                    TempNew[NumNew][1]=Temp[(i+1)%Num][1];
                    TempNew[NumNew][2]=Temp[(i+1)%Num][2];
                    TempNew[NumNew][3]=Temp[(i+1)%Num][3];
                    NumNew++;


                }
                break;
                case 3:
                {
                    GLfloat IP[4];
                    IntersectionPoint(j,Temp[i%Num],Temp[(i+1)%Num],IP);

                    TempNew[NumNew][0]=IP[0];
                    TempNew[NumNew][1]=IP[1];
                    TempNew[NumNew][2]=IP[2];
                    TempNew[NumNew][3]=IP[3];
                    NumNew++;


                    TempNew[NumNew][0]=Temp[(i+1)%Num][0];
                    TempNew[NumNew][1]=Temp[(i+1)%Num][1];
                    TempNew[NumNew][2]=Temp[(i+1)%Num][2];
                    TempNew[NumNew][3]=Temp[(i+1)%Num][3];
                    NumNew++;


                }
                break;
                case 4:
                {
                }
                break;
            }


        }
        printf("PRINT %d\n",NumNew);

        Num=NumNew;
        *NumVertices=NumNew;
        int p=0;
        for(i=0;i<Num;i++)
        {
            Temp[i][0]=TempNew[i][0];
            Temp[i][1]=TempNew[i][1];
            Temp[i][2]=TempNew[i][2];
            Temp[i][3]=TempNew[i][3];


            Output[p++]=TempNew[i][0];
            Output[p++]=TempNew[i][1];
            Output[p++]=TempNew[i][2];
            Output[p++]=TempNew[i][3];


        }
        //break;
        j++;
    }

}


void Inside_Outside(GLint j,GLfloat *Vertex1,GLfloat *Vertex2,GLint *Ret)    //true for inside, false for outside, Ret denotes case number
{
    GLfloat x1=Vertex1[0]/Vertex1[3];
    GLfloat y1=Vertex1[1]/Vertex1[3];
    GLfloat z1=Vertex1[2]/Vertex1[3];

    GLfloat x2=Vertex2[0]/Vertex2[3];
    GLfloat y2=Vertex2[1]/Vertex2[3];
    GLfloat z2=Vertex2[2]/Vertex2[3];

    switch(j)
    {
        case 0:   // clipper z=1;
        {
            if((z1-1)<0)       // inside
            {
                if((z2-1)<0)   //inside
                {
                    *Ret=2;   //case 2

                }else{
                    *Ret=1;
                }
            }else{
                if((z2-1)<0)   //inside
                {
                     *Ret=3;

                }else{
                    *Ret=4;
                }
            }
        }
        break;
        case 1:   //z=-1
        {
            if((z1+1)>0)       // inside
            {
                if((z2+1)>0)   //inside
                {
                    *Ret=2;   //case 2

                }else{
                    *Ret=1;
                }
            }else{
                if((z2+1)>0)   //inside
                {
                     *Ret=3;

                }else{
                    *Ret=4;
                }
            }
        }
        break;
        case 2:   //case y=1
          {
            if((y1-1)<0)       // inside
            {
                if((y2-1)<0)   //inside
                {
                    *Ret=2;   //case 2

                }else{
                    *Ret=1;
                }
            }else{
                if((y2-1)<0)   //inside
                {
                     *Ret=3;

                }else{
                    *Ret=4;
                }
            }
        }
        break;
        case 3:   //case y=-1
        {
            if((y1+1)>0)       // inside
            {
                if((y2+1)>0)   //inside
                {
                    *Ret=2;   //case 2

                }else{
                    *Ret=1;
                }
            }else{
                if((y2+1)>0)   //inside
                {
                     *Ret=3;

                }else{
                    *Ret=4;
                }
            }
        }
        break;
        case 4:  //case x=1
             {
            if((x1-1)<0)       // inside
            {
                if((x2-1)<0)   //inside
                {
                    *Ret=2;   //case 2

                }else{
                    *Ret=1;
                }
            }else{
                if((x2-1)<0)   //inside
                {
                     *Ret=3;

                }else{
                    *Ret=4;
                }
            }
        }
        break;
        case 5:     //case x=-1
        {
            if((x1+1)>0)       // inside
            {
                if((x2+1)>0)   //inside
                {
                    *Ret=2;   //case 2

                }else{
                    *Ret=1;
                }
            }else{
                if((x2+1)>0)   //inside
                {
                     *Ret=3;

                }else{
                    *Ret=4;
                }
            }
        }
        break;
    }
}

bool IntersectionPoint(GLint j,GLfloat * Vertex1,GLfloat *Vertex2,GLfloat *IP)
{
    GLfloat u;
    switch(j)
    {
        case 0:      //case z=1
        {
            u=(Vertex1[2]-Vertex1[3])/((Vertex1[2]-Vertex1[3])-(Vertex2[2]-Vertex2[3]));
            if(((Vertex1[2]-Vertex1[3])-(Vertex2[2]-Vertex2[3]))==0)
            {
                printf("0000000000 %lf %lf %lf %lf %lf %lf %lf %lf \n",Vertex1[0],Vertex1[1],Vertex1[2],Vertex1[3],Vertex2[0],Vertex2[1],Vertex2[2],Vertex2[3]);
                return false;
            }
        }
        break;
        case 1:  //case z=-1
        {
             u=(Vertex1[2]+Vertex1[3])/((Vertex1[2]+Vertex1[3])-(Vertex2[2]+Vertex2[3]));

             if(((Vertex1[2]+Vertex1[3])-(Vertex2[2]+Vertex2[3]))==0)
            {
                printf("00000000001\n");
                return false;
            }
        }
        break;
        case 2:  //case y=1
        {
             u=(Vertex1[1]-Vertex1[3])/((Vertex1[1]-Vertex1[3])-(Vertex2[1]-Vertex2[3]));

             if(((Vertex1[1]-Vertex1[3])-(Vertex2[1]-Vertex2[3]))==0)
            {
                printf("00000000002\n");
                return false;
            }
        }
        break;
        case 3:
        {
              u=(Vertex1[1]+Vertex1[3])/((Vertex1[1]+Vertex1[3])-(Vertex2[1]+Vertex2[3]));


             if(((Vertex1[1]+Vertex1[3])-(Vertex2[1]+Vertex2[3]))==0)
            {
                printf("00000000003\n");
                return false;
            }
        }
        break;
        case 4:   //case x=1
        {
             u=(Vertex1[0]-Vertex1[3])/((Vertex1[0]-Vertex1[3])-(Vertex2[0]-Vertex2[3]));

             if(((Vertex1[0]-Vertex1[3])-(Vertex2[0]-Vertex2[3]))==0)
            {
                printf("00000000004\n");
                return false;
            }
        }
        break;
        case 5:
        {
              u=(Vertex1[0]+Vertex1[3])/((Vertex1[0]+Vertex1[3])-(Vertex2[0]+Vertex2[3]));


             if(((Vertex1[0]+Vertex1[3])-(Vertex2[0]+Vertex2[3]))==0)
            {
                printf("00000000005\n");
                return false;
            }
        }
        break;
    }
    IP[0]=Vertex1[0]+(Vertex2[0]-Vertex1[0])*u;
    IP[1]=Vertex1[1]+(Vertex2[1]-Vertex1[1])*u;
    IP[2]=Vertex1[2]+(Vertex2[2]-Vertex1[2])*u;
    IP[3]=Vertex1[3]+(Vertex2[3]-Vertex1[3])*u;
    return true;
}
