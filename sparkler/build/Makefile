# Linux
CC=clang++
LIBS= -lSDL2 -lGLEW -lGL -lfreeimageplus

# Compiler flags
CXXFLAGS= -g -Wall -std=c++0x

OBJ= Camera.o Texture.o RenderProgram.o SparklerUpdateProgram.o SparklerRenderProgram.o RandomTexture.o Sparkler.o

all: ../bin/sparkler

../bin/sparkler: ../src/main.cpp $(OBJ)
	$(CC) $(CXXFLAGS) ../src/main.cpp -o ../bin/sparkler $(OBJ) $(LIBS)

Camera.o: ../src/Camera.h ../src/Camera.cpp
	$(CC) $(CXXFLAGS) -c ../src/Camera.cpp

Texture.o: ../src/Texture.h ../src/Texture.cpp
	$(CC) $(CXXFLAGS) -c ../src/Texture.cpp

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
	rm -f *.o ../bin/Sparkler $(RM)

