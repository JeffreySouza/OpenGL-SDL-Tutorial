CC = g++

COMPILER_FLAGS = -std=c++11 -w -O3 -DNDEBUG -Wall -Werror -Wextra -pedantic

LINKER_FLAGS = -lSDL2 -lGL -lGLEW -lSOIL #-lGLU

HELLO = hello/hello.cpp
SHADER = shader/shader.cpp
TEXTURE = texture/texture.cpp
TRANSFORMATION = transformation/transformation.cpp
COORDINATE = coordinate/coordinate.cpp
CAMERA = camera/camera.cpp

EXECUTABLE_HELLO = hello.out
EXECUTABLE_SHADER = shader.out
EXECUTABLE_TEXTURE = texture.out
EXECUTABLE_TRANSFORMATION = transformation.out
EXECUTABLE_COORDINATE = coordinate.out
EXECUTABLE_CAMERA = camera.out
EXECUTABLES = $(EXECUTABLE_HELLO) $(EXECUTABLE_SHADER) $(EXECUTABLE_TEXTURE) $(EXECUTABLE_TRANSFORMATION) $(EXECUTABLE_COORDINATE) $(EXECUTABLE_CAMERA)

all: hello-make shader-make texture-make transformation-make coordinate-make camera-make

hello-make: $(HELLO)
	$(CC) $(HELLO) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXECUTABLE_HELLO)

shader-make: $(SHADER)
	$(CC) $(SHADER) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXECUTABLE_SHADER)

texture-make: $(TEXTURE)
	$(CC) $(TEXTURE) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXECUTABLE_TEXTURE)

transformation-make: $(TRANSFORMATION)
	$(CC) $(TRANSFORMATION) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXECUTABLE_TRANSFORMATION)

coordinate-make: $(COORDINATE)
	$(CC) $(COORDINATE) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXECUTABLE_COORDINATE)

camera-make: $(CAMERA)
	$(CC) $(CAMERA) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXECUTABLE_CAMERA)

clean:
	rm *.o *.exe $(EXECUTABLES)

