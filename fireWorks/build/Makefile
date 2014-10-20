# Linux
CC=clang++
LIBS= -lSDL2 -lGLEW -lGL -lfreeimageplus -lgdal

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x -I/usr/include/gdal

OBJ= ShaderLoader.o Texture.o Terrain.o Shape.o Grass.o Camera.o RenderProgram.o SparklerUpdateProgram.o SparklerRenderProgram.o RandomTexture.o Sparkler.o

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

Grass.o: ../src/Grass.h ../src/Grass.cpp
	$(CC) $(CXXFLAGS) -c ../src/Grass.cpp

Camera.o: ../src/Camera.h ../src/Camera.cpp
	$(CC) $(CXXFLAGS) -c ../src/Camera.cpp

RenderProgram.o: ../src/RenderProgram.h ../src/RenderProgram.cpp
	$(CC) $(CXXFLAGS) -c ../src/RenderProgram.cpp

SparklerUpdateProgram.o: ../src/SparklerUpdateProgram.h ../src/SparklerUpdateProgram.cpp
	$(CC) $(CXXFLAGS) -c ../src/SparklerUpdateProgram.cpp

SparklerRenderProgram.o: ../src/SparklerRenderProgram.h ../src/SparklerRenderProgram.cpp
	$(CC) $(CXXFLAGS) -c ../src/SparklerRenderProgram.cpp

RandomTexture.o: ../src/RandomTexture.h ../src/RandomTexture.cpp
	$(CC) $(CXXFLAGS) -c ../src/RandomTexture.cpp


Sparkler.o: ../src/Sparkler.h ../src/Sparkler.cpp
	$(CC) $(CXXFLAGS) -c ../src/Sparkler.cpp

clean:
	rm -f *.o ../bin/terrain $(RM)

