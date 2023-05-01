.phony all:

all: physics

physics: physics.cpp
	g++ physics.cpp glad.c -o out -lglfw -ldl -lGLU -lGL

