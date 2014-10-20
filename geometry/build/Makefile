# Linux
CC=clang++
LIBS= -lSDL2 -lGLEW -lGL -lfreeimageplus

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x

OBJ= ShaderLoader.o Texture.o Water.o Grass.o

all: ../bin/water

../bin/water: ../src/main.cpp $(OBJ)
	$(CC) $(CXXFLAGS) ../src/main.cpp -o ../bin/water $(OBJ) $(LIBS)

ShaderLoader.o: ../src/ShaderLoader.h ../src/ShaderLoader.cpp
	$(CC) $(CXXFLAGS) -c ../src/ShaderLoader.cpp

Texture.o: ../src/Texture.h ../src/Texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/Texture.cpp

Water.o: ../src/Water.h ../src/Water.cpp
	$(CC) $(CXXFLAGS) -c ../src/Water.cpp

Grass.o: ../src/Grass.h ../src/Grass.cpp
	$(CC) $(CXXFLAGS) -c ../src/Grass.cpp

clean:
	rm -f *.o ../bin/water $(RM)

