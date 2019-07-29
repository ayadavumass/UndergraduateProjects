#include "ScanLine.h"
#include<algorithm>
#include<iostream>
#include<math.h>

#define PART    2   //1 for c part, 2 for d part
//function declarations
void MakeActiveEdgeList(int ScanLine);
using namespace std;




//function declarations
bool cmpFunc (Info Obj1,Info Obj2) { return (Obj1.IntersectionX<Obj2.IntersectionX); }
bool cmpFunc_Depth (On_Off Obj1,On_Off Obj2) { return (Obj1.Depth<Obj2.Depth); }
void On_Off_Surface(vector<On_Off> *OnFlagCount,int SurfaceNum,int ObjNum);     //now works only for convex
void DepthCalcRoutine(vector<On_Off> *OnFlagCount,int PixX,int PixY);

void FindEdgesOfSurface(int ObjNum,int SurfNum,vector<Info>* Output);
void NonIterativeGoroud(vector<Info>* Edges,int PixX,int PixY);

void PrintEdgeList();

void ScanLineAlgo()
{
    int i=0,j=0,k=0;;
    for(j=0;j<winHeight;j++)   //each scan line
    {
        k=0;
        ActiveEdgeList.clear();
        MakeActiveEdgeList(j);
        if(j==200)
        {
            PrintEdgeList();
        }

        vector<On_Off> OnFlagCount;   // index of surface which is on
        OnFlagCount.clear();
        for(i=0;i<winWidth;i++)
        {
            if(k<ActiveEdgeList.size())
            {
                if(i==(int)ActiveEdgeList.at(k).IntersectionX)
                {
                    while(i==(int)ActiveEdgeList.at(k).IntersectionX)    //pathological case, when two edges have same intersection at scan line
                    {
                    vector<int>::iterator it1;
                    for ( it1=ActiveEdgeList.at(k).SurfaceVect.begin() ; it1 < ActiveEdgeList.at(k).SurfaceVect.end(); it1++ )
                    {
                       // OnFlagCount.push_back(*it1);
                        //SurfaceTable.at(*it1).flag=true;    //turning on the surface
                       On_Off_Surface(&OnFlagCount,(*it1),ActiveEdgeList.at(k).ObjNum);     //now works only for convex
                    }
                    DepthCalcRoutine(&OnFlagCount,i,j);
                    k++;
                    if(k>=ActiveEdgeList.size())
                        break;
                    }
                }
                else if(i<(int)ActiveEdgeList.at(k).IntersectionX)     // intermidiate between intersections
                {
                    DepthCalcRoutine(&OnFlagCount,i,j);
                }
            }
        }
    }
}


void MakeActiveEdgeList(int ScanLine)
{
    vector<Info>::iterator it;
    int i=0;
    for(i=0;i<NUMOBJECTS;i++)
    {
        for ( it=EdgeTable[i].begin() ; it < EdgeTable[i].end(); it++ )
        {
            GLdouble Y1,Y2,X1,X2;

            Y1=VertexTable[i].at((*it).InfoVect.at(0)).y;   // taking y coordinate of both edge points
            Y2=VertexTable[i].at((*it).InfoVect.at(1)).y;

            X1=VertexTable[i].at((*it).InfoVect.at(0)).x;   // taking x coordinate of both edge points
            X2=VertexTable[i].at((*it).InfoVect.at(1)).x;

            if(((Y1-ScanLine)*(Y2-ScanLine))<0)   // edge intersects scanline
            {
                GLdouble Scn=ScanLine;
                //GLdouble IntersectionX=(((Scn-Y1)*(X1-X2))/(Y1-Y2))+ X1;   //intersection X on scanline
                GLdouble IntersectionX=((Scn-Y1)*(*it).InverseSlope)+X1;
                (*it).IntersectionX=IntersectionX;
                ActiveEdgeList.push_back((*it));
            }else{

            }
        }
    }
    if(ActiveEdgeList.size()>0)
        sort (ActiveEdgeList.begin(), ActiveEdgeList.end(), cmpFunc);
}

void DepthCalcRoutine(vector<On_Off> *OnFlagCount,int PixX,int PixY)
{
    GLdouble A,B,C,D;
    if(OnFlagCount->size()>0)
    {
        vector<On_Off>::iterator it;
        for ( it=OnFlagCount->begin() ; it < OnFlagCount->end(); it++ ) // every edge of surface
        {
            A=SurfaceTable[(*it).ObjNum].at((*it).SurfNum).A;
            B=SurfaceTable[(*it).ObjNum].at((*it).SurfNum).B;
            C=SurfaceTable[(*it).ObjNum].at((*it).SurfNum).C;
            D=SurfaceTable[(*it).ObjNum].at((*it).SurfNum).D;
            (*it).Depth=(-A*PixX-B*PixY-D)/C;
        }
         sort (OnFlagCount->begin(), OnFlagCount->end(), cmpFunc_Depth);
         if(PART==1)
         {
         if(OnFlagCount->at(0).ObjNum==0)
         {
             FB[PixY][PixX][0]=1.0;
             FB[PixY][PixX][1]=0.0;
             FB[PixY][PixX][2]=0.0;

         }else if(OnFlagCount->at(0).ObjNum==1)
         {
             FB[PixY][PixX][0]=0.0;
             FB[PixY][PixX][1]=1.0;
             FB[PixY][PixX][2]=0.0;

         }
         }else if(PART==2)
         {
         vector<Info> CurrEdges;
         CurrEdges.clear();
         FindEdgesOfSurface(OnFlagCount->at(0).ObjNum,OnFlagCount->at(0).SurfNum,&CurrEdges);
         NonIterativeGoroud(&CurrEdges,PixX,PixY);
         }
    }
}

void On_Off_Surface(vector<On_Off> *OnFlagCount,int SurfaceNum,int ObjNum)     //now works only for convex
{
    vector<On_Off>::iterator it;
    bool found=false;
    for ( it=OnFlagCount->begin() ; it < OnFlagCount->end(); it++ ) // every edge of surface
    {
        if(((*it).SurfNum==SurfaceNum) && ((*it).ObjNum==ObjNum))
        {
            found=true;
            break;
        }
    }
    if(found)   //right edge has come, off the surface
    {
        OnFlagCount->erase(it);
        SurfaceTable[ObjNum].at(SurfaceNum).flag=false;    //turning on the surface
    }else{    //left edge is there on the surface
        On_Off Obj;
        Obj.SurfNum=SurfaceNum;
        Obj.ObjNum=ObjNum;
        OnFlagCount->push_back(Obj);
        SurfaceTable[ObjNum].at(SurfaceNum).flag=true;    //turning on the surface
        }
}

void PrintEdgeList()
{
    cout<<"Printing Active Edge List"<<endl;
    vector<Info>::iterator it1;
    for ( it1=ActiveEdgeList.begin() ; it1 < ActiveEdgeList.end(); it1++ )
        {
            cout<<"ObjNum "<<(*it1).ObjNum<<"Num "<<(*it1).Num<<"IntersectionX "<<(*it1).IntersectionX<<endl;

        }
}


void FindEdgesOfSurface(int ObjNum,int SurfNum,vector<Info>* Output)      //finds edges of ON  surface whic are intersecting scan line, for goroud
{
    vector<Info>::iterator it;
    for ( it=ActiveEdgeList.begin() ; it < ActiveEdgeList.end(); it++ )
    {
        if((*it).ObjNum==ObjNum)
        {
            vector<int>::iterator it1;
            for ( it1=(*it).SurfaceVect.begin() ; it1 < (*it).SurfaceVect.end(); it1++ )
            {
                if((*it1)==SurfNum)
                {
                    Output->push_back((*it));
                }
            }
        }
    }
}


void NonIterativeGoroud(vector<Info>* Edges,int PixX,int PixY)
{
    GLfloat GoroudColor1[3],GoroudColor2[3],GoroudColor3[3];
    GLfloat Color1[3],Color2[3],Color3[3],Color4[3];
    GLdouble Point1[3],Point2[3],Point3[3],Point4[3];
    GLdouble X1,X2;   //intersection poibnts od edges with scan line
    int ObjNum=Edges->at(0).ObjNum;
    GLdouble PixXD=PixX,PixYD=PixY;

    X1=Edges->at(0).IntersectionX;
    X2=Edges->at(1).IntersectionX;


    Color1[0]=VertexTable[ObjNum].at(Edges->at(0).InfoVect.at(0)).Color[0];
    Color1[1]=VertexTable[ObjNum].at(Edges->at(0).InfoVect.at(0)).Color[1];
    Color1[2]=VertexTable[ObjNum].at(Edges->at(0).InfoVect.at(0)).Color[2];

    Point1[0]=VertexTable[ObjNum].at(Edges->at(0).InfoVect.at(0)).x;
    Point1[1]=VertexTable[ObjNum].at(Edges->at(0).InfoVect.at(0)).y;
    Point1[2]=VertexTable[ObjNum].at(Edges->at(0).InfoVect.at(0)).z;



    Color2[0]=VertexTable[ObjNum].at(Edges->at(0).InfoVect.at(1)).Color[0];
    Color2[1]=VertexTable[ObjNum].at(Edges->at(0).InfoVect.at(1)).Color[1];
    Color2[2]=VertexTable[ObjNum].at(Edges->at(0).InfoVect.at(1)).Color[2];

    Point2[0]=VertexTable[ObjNum].at(Edges->at(0).InfoVect.at(1)).x;
    Point2[1]=VertexTable[ObjNum].at(Edges->at(0).InfoVect.at(1)).y;
    Point2[2]=VertexTable[ObjNum].at(Edges->at(0).InfoVect.at(1)).z;


    Color3[0]=VertexTable[ObjNum].at(Edges->at(1).InfoVect.at(0)).Color[0];
    Color3[1]=VertexTable[ObjNum].at(Edges->at(1).InfoVect.at(0)).Color[1];
    Color3[2]=VertexTable[ObjNum].at(Edges->at(1).InfoVect.at(0)).Color[2];

    Point3[0]=VertexTable[ObjNum].at(Edges->at(1).InfoVect.at(0)).x;
    Point3[1]=VertexTable[ObjNum].at(Edges->at(1).InfoVect.at(0)).y;
    Point3[2]=VertexTable[ObjNum].at(Edges->at(1).InfoVect.at(0)).z;

    Color4[0]=VertexTable[ObjNum].at(Edges->at(1).InfoVect.at(1)).Color[0];
    Color4[1]=VertexTable[ObjNum].at(Edges->at(1).InfoVect.at(1)).Color[1];
    Color4[2]=VertexTable[ObjNum].at(Edges->at(1).InfoVect.at(1)).Color[2];

    Point4[0]=VertexTable[ObjNum].at(Edges->at(1).InfoVect.at(1)).x;
    Point4[1]=VertexTable[ObjNum].at(Edges->at(1).InfoVect.at(1)).y;
    Point4[2]=VertexTable[ObjNum].at(Edges->at(1).InfoVect.at(1)).z;

    GoroudColor1[0]=fabs((PixYD-Point2[1])/(Point1[1]-Point2[1]))*Color1[0]+fabs((PixYD-Point1[1])/(Point1[1]-Point2[1]))*Color2[0];
    GoroudColor1[1]=fabs((PixYD-Point2[1])/(Point1[1]-Point2[1]))*Color1[1]+fabs((PixYD-Point1[1])/(Point1[1]-Point2[1]))*Color2[1];
    GoroudColor1[2]=fabs((PixYD-Point2[1])/(Point1[1]-Point2[1]))*Color1[2]+fabs((PixYD-Point1[1])/(Point1[1]-Point2[1]))*Color2[2];

    GoroudColor2[0]=fabs((PixYD-Point4[1])/(Point3[1]-Point4[1]))*Color3[0]+fabs((PixYD-Point3[1])/(Point3[1]-Point4[1]))*Color4[0];
    GoroudColor2[1]=fabs((PixYD-Point4[1])/(Point3[1]-Point4[1]))*Color3[1]+fabs((PixYD-Point3[1])/(Point3[1]-Point4[1]))*Color4[1];
    GoroudColor2[2]=fabs((PixYD-Point4[1])/(Point3[1]-Point4[1]))*Color3[2]+fabs((PixYD-Point3[1])/(Point3[1]-Point4[1]))*Color4[2];

     GoroudColor3[0]=fabs((PixXD-X2)/(X1-X2))*GoroudColor1[0]+fabs((PixXD-X1)/(X1-X2))*GoroudColor2[0];
     GoroudColor3[1]=fabs((PixXD-X2)/(X1-X2))*GoroudColor1[1]+fabs((PixXD-X1)/(X1-X2))*GoroudColor2[1];
     GoroudColor3[2]=fabs((PixXD-X2)/(X1-X2))*GoroudColor1[2]+fabs((PixXD-X1)/(X1-X2))*GoroudColor2[2];

     FB[PixY][PixX][0]=GoroudColor3[0];
     FB[PixY][PixX][1]=GoroudColor3[1];
     FB[PixY][PixX][2]=GoroudColor3[2];
}
