
OS := $(shell uname)

ifeq ($(OS), Linux)
CC=clang++
LIBS= -lglut -lGLEW -lGL -lassimp -lfreeimageplus
CXXFLAGS= -g -Wall -std=c++0x -DASSIMP_3
RM= 

else #Mac
CC=clang++
LIBS=  -framework OpenGL -framework GLUT -framework Cocoa -lGLEW -lassimp -lfreeimageplus
CXXFLAGS= -g -Wall -std=c++11 -stdlib=libc++
RM= ../bin/Matrix.dSYM
endif
OBJ= ShaderLoader.o ModelLoader.o

all: ../bin/Matrix

../bin/Matrix: ../src/main.cpp $(OBJ)
	$(CC) $(CXXFLAGS) ../src/main.cpp -o ../bin/Matrix $(OBJ) $(LIBS)

ShaderLoader.o: ../src/ShaderLoader.h ../src/ShaderLoader.cpp
	$(CC) $(CXXFLAGS) -c ../src/ShaderLoader.cpp

ModelLoader.o: ../src/ModelLoader.h ../src/ModelLoader.cpp
	$(CC) $(CXXFLAGS) -c ../src/ModelLoader.cpp
clean:
	rm -rf *.o ../bin/Matrix $(RM)
