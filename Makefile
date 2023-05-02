.phony all:

all: main_game_loop

main_game_loop: main_game_loop.cpp
	g++ main_game_loop.cpp glad.c -o game -lglfw -ldl -lGLU -lGL

