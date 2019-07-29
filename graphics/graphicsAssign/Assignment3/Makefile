all: PersScene LightingCube SquareGo

PersScene: PersScene.o CommonFunc.o	Perspective.o LightingModel.o PersSceneGUI.o Transform.o
		g++ -I/usr/include/GL PersScene.o CommonFunc.o	Perspective.o LightingModel.o PersSceneGUI.o Transform.o -o PersScene -lglui -lglut -lGLU -lGL


LightingCube: Lighting3DCube.o CommonFunc.o	Perspective.o LightingModel.o 3DLightingCubeGUI.o Transform.o
	g++ -I/usr/include/GL Lighting3DCube.o CommonFunc.o Perspective.o LightingModel.o 3DLightingCubeGUI.o Transform.o -o LightingCube -lglui -lglut -lGLU -lGL

SquareGo: SquareGo.o CommonFunc.o Perspective.o LightingModel.o SquareGoGUI.o Transform.o
		g++ -I/usr/include/GL SquareGo.o CommonFunc.o Perspective.o LightingModel.o SquareGoGUI.o Transform.o -o SquareGo -lglui -lglut -lGLU -lGL

PersScene.o: PersScene.cpp
		g++ -c PersScene.cpp

Transform.o: Transform.cpp
		g++ -c Transform.cpp

CommonFunc.o: CommonFunc.cpp
		 g++ -c CommonFunc.cpp 

Perspective.o: Perspective.cpp
		 g++ -c Perspective.cpp 

LightingModel.o: LightingModel.cpp
		 g++ -c LightingModel.cpp 

PersSceneGUI.o: PersSceneGUI.cpp
		 g++ -c PersSceneGUI.cpp

3DLightingCubeGUI.o: 3DLightingCubeGUI.cpp
		 g++ -c 3DLightingCubeGUI.cpp

Lighting3DCube.o: Lighting3DCube.cpp
		g++ -c Lighting3DCube.cpp

SquareGoGUI.o: SquareGoGUI.cpp
		 g++ -c SquareGoGUI.cpp

SquareGo.o: SquareGo.cpp
		g++ -c SquareGo.cpp

clean:
	rm -rf *o  PersScene LightingCube

run:
	./PersScene& ./LightingCube& ./SquareGo&
