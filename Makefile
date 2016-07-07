CC = g++

COMPILER_FLAGS = -std=c++11 -w -O3 -DNDEBUG -Wall -Werror -Wextra -pedantic

LINKER_FLAGS = -lSDL2 -lGL -lGLEW #-lGLU

HELLO = hello.cpp
SHADER = shader.cpp

EXECUTABLE_HELLO = hello
EXECUTABLE_SHADER = shader
EXECUTABLES = $(EXECUTABLE_HELLO) $(EXECUTABLE_SHADER)

all: hello shader

hello: $(HELLO)
	$(CC) $(HELLO) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXECUTABLE_HELLO)

shader: $(SHADER)
	$(CC) $(SHADER) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXECUTABLE_SHADER)

clean:
	rm *.o *.exe $(EXECUTABLES)

