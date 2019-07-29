all: assignment2

assignment2: assignment2.o assignscene.o
		g++ -I/usr/include/GL assignment2.o assignscene.o -o assignment2 -lglui -lglut -lGLU -lGL

assignment2.o: assignment2.cpp
		g++ -c assignment2.cpp



assignscene.o: assignscene.cpp
		 g++ -c assignscene.cpp 

clean:
	rm -rf *o  assignment2

run:
	./assignment2&
