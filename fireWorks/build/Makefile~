# Linux
CC=g++
LIBS= -lSDL2 -lGLEW -lGL

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x

OBJ= ShaderLoader.o

all: ../bin/Matrix

../bin/Matrix: ../src/main.cpp ShaderLoader.o
	$(CC) $(CXXFLAGS) ../src/main.cpp -o ../bin/Matrix $(OBJ) $(LIBS)

ShaderLoader.o: ../src/ShaderLoader.h ../src/ShaderLoader.cpp
	$(CC) $(CXXFLAGS) -c ../src/ShaderLoader.cpp

clean:
	rm -f *.o ../bin/Matrix

