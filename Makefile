CC = g++

COMPILER_FLAGS = -std=c++11 -w -O3 -DNDEBUG -Wall -Werror -Wextra -pedantic

LINKER_FLAGS = -lSDL2 -lGL -lGLEW #-lGLU

HELLO = hello.cpp

EXECUTABLE_HELLO = out
EXECUTABLES = $(EXECUTABLE_HELLO)

all: hello

hello: $(HELLO)
	$(CC) $(HELLO) $(COMPILER_FLAGS) $(LINKER_FLAGS) -o $(EXECUTABLE_HELLO)

clean:
	rm *.o *.exe $(EXECUTABLES)

