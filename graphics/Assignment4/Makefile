all: PartA PartB PartC PartD PartA_Ori PartC_Ori PartD_Ori

PartA: Cube.o Clipping.o
	g++ -I/usr/include/GL Cube.o Clipping.o -o PartA -lglui -lglut -lGLU -lGL
Cube.o:	Cube.cpp
	g++ -c Cube.cpp

PartA_Ori: Cube_Ori.o Clipping.o
	g++ -I/usr/include/GL Cube_Ori.o Clipping.o -o PartA_Ori -lglui -lglut -lGLU -lGL
Cube_Ori.o:	Cube_Ori.cpp
	g++ -c Cube_Ori.cpp

Clipping.o:	Clipping.cpp
	g++ -c Clipping.cpp
PartB: BPart.o Bresenham.o 
	g++ -I/usr/include/GL BPart.o Bresenham.o -o PartB -lglui -lglut -lGLU -lGL
BPart.o: BPart.cpp
	g++ -c BPart.cpp
Bresenham.o: Bresenham.cpp
	g++ -c Bresenham.cpp
PartC: CPart.o ScanLine.o 
	g++ -I/usr/include/GL CPart.o ScanLine.o -o PartC -lglui -lglut -lGLU -lGL
CPart.o: CPart.cpp
	g++ -c CPart.cpp

PartC_Ori: CPart_Ori.o ScanLine.o 
	g++ -I/usr/include/GL CPart_Ori.o ScanLine.o -o PartC_Ori -lglui -lglut -lGLU -lGL
CPart_Ori.o: CPart_Ori.cpp
	g++ -c CPart_Ori.cpp

PartD: DPart.o ScanLine.o 
	g++ -I/usr/include/GL DPart.o ScanLine.o -o PartD -lglui -lglut -lGLU -lGL
DPart.o: DPart.cpp
	g++ -c DPart.cpp

PartD_Ori: DPart_Ori.o ScanLine.o 
	g++ -I/usr/include/GL DPart.o ScanLine.o -o PartD_Ori -lglui -lglut -lGLU -lGL
DPart_Ori.o: DPart_Ori.cpp
	g++ -c DPart_Ori.cpp

ScanLine.o: ScanLine.cpp
	g++ -c ScanLine.cpp
	
