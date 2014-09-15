# Linux
CC=clang++
LIBS= -lSDL2 -lGLEW -lGL -lfreeimageplus -lgdal -lCEGUIOpenGLRenderer-0 -lCEGUIBase-0

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x -I/usr/include/gdal -I/usr/local/include/cegui-0/ -O2

OBJ= ShaderLoader.o Texture.o Terrain.o

all: ../bin/terrain

../bin/terrain: ../src/main.cpp $(OBJ)
	$(CC) $(CXXFLAGS) ../src/main.cpp -o ../bin/terrain $(OBJ) $(LIBS)

ShaderLoader.o: ../src/ShaderLoader.h ../src/ShaderLoader.cpp
	$(CC) $(CXXFLAGS) -c ../src/ShaderLoader.cpp

Terrain.o: ../src/Terrain.h ../src/Terrain.cpp
	$(CC) $(CXXFLAGS) -c ../src/Terrain.cpp

Texture.o: ../src/Texture.h ../src/Texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/Texture.cpp

clean:
	rm -f *.o ../bin/terrain $(RM)

