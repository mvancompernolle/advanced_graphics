# Linux
CC=clang++
LIBS= -lSDL2 -lGLEW -lGL -lfreeimageplus -lgdal

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x -I/usr/include/gdal

OBJ= ShaderLoader.o Texture.o Terrain.o Shape.o 

all: ../bin/terrain

../bin/terrain: ../src/main.cpp $(OBJ)
	$(CC) $(CXXFLAGS) ../src/main.cpp -o ../bin/terrain $(OBJ) $(LIBS)

ShaderLoader.o: ../src/ShaderLoader.h ../src/ShaderLoader.cpp
	$(CC) $(CXXFLAGS) -c ../src/ShaderLoader.cpp

Terrain.o: ../src/Terrain.h ../src/Terrain.cpp
	$(CC) $(CXXFLAGS) -c ../src/Terrain.cpp

Texture.o: ../src/Texture.h ../src/Texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/Texture.cpp

Shape.o: ../src/Shape.h ../src/Shape.cpp
	$(CC) $(CXXFLAGS) -c ../src/Shape.cpp

clean:
	rm -f *.o ../bin/terrain $(RM)

