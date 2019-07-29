#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <vector>
#ifndef SCANLINE_H_INCLUDED
#define SCANLINE_H_INCLUDED
#define NUMOBJECTS  2
typedef struct Info{
    int Num;
    std::vector<int> InfoVect;     // stores edgepoints or surface edges
    std::vector<int> SurfaceVect;   //in case of edge belongs to whic surfaces
    bool flag;         //on off for surface  used only in SurfaceTable
    int ObjNum;         // for finding which object this edge belongs during active edge list formation
    GLdouble IntersectionX;  // intersection of edge with curr scan line
    GLdouble InverseSlope;       //in case of edge
    GLdouble A,B,C,D;             //plane equation parameters for surface
}Info;
typedef struct Point{
    int Num;
    GLdouble x;
    GLdouble y;
    GLdouble z;
    GLfloat Color[3];   //RGB
}Point;

typedef struct On_Off{
    int ObjNum;
    int SurfNum;
    GLdouble Depth;    // comparing depth while find visible surface
}On_Off;

extern std::vector<Point> VertexTable[NUMOBJECTS];
extern std::vector<Info> EdgeTable[NUMOBJECTS];
extern std::vector<Info> SurfaceTable[NUMOBJECTS];
extern const int winWidth,winHeight;
extern std::vector<Info> ActiveEdgeList;
extern GLfloat FB[500][500][3];
void ScanLineAlgo();


#endif // SCANLINE_H_INCLUDED
