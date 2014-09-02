
OS := $(shell uname)

ifeq ($(OS), Linux)
CC=clang++
LIBS= -lGLEW -lGLU -lGL -lassimp -I/usr/include/bullet -lBulletDynamics -lBulletSoftBody -lBulletCollision -lLinearMath -lfreeimageplus -lSDL2
CXXFLAGS= -g -Wall -std=c++0x
RM= 

else #Mac
CC=clang++
LIBS=  -framework OpenGL -framework -framework Cocoa -lGLEW -lassimp -I/usr/include/bullet -lBulletDynamics -lBulletSoftBody -lBulletCollision -lLinearMath -lfreeimageplus
CXXFLAGS= -g -Wall -std=c++11 -stdlib=libc++
RM= ../bin/Matrix.dSYM
endif
OBJ= ShaderLoader.o Mesh.o Texture.o Terrain.o

all: ../bin/terrain

../bin/terrain: ../src/main.cpp $(OBJ)
	$(CC) $(CXXFLAGS) ../src/main.cpp -o ../bin/terrain $(OBJ) $(LIBS)

ShaderLoader.o: ../src/ShaderLoader.h ../src/ShaderLoader.cpp
	$(CC) $(CXXFLAGS) -c ../src/ShaderLoader.cpp

Mesh.o: ../src/Mesh.h ../src/Mesh.cpp
	$(CC) $(CXXFLAGS) -c ../src/Mesh.cpp -I/usr/include/bullet 

Terrain.o: ../src/Terrain.h ../src/Terrain.cpp
	$(CC) $(CXXFLAGS) -c ../src/Terrain.cpp

Texture.o: ../src/Texture.h ../src/Texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/Texture.cpp

clean:
	rm -rf *.o ../bin/terrain $(RM)
