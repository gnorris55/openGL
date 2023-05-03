#ifndef LIGHT_H
#define LIGHT_H

#include "raw_model.h"
#include "shader.h"

class Light {

	public:
	Shader *program;
	RawModel *model;
	GLFWwindow *window;

	Light(Shader *program, RawModel *model, GLFWwindow *window) {
		this->program = program;
		this->model = model;
		std::cout << "light has been made\n";
	}
	
	/*
	void generate() {

	}

	void controls() {

	}
	*/


};


#endif
