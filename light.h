#ifndef LIGHT_H
#define LIGHT_H

#include "raw_model.h"
#include "shader.h"

class Light {

	public:

	Shader *program;
	RawModel *model;
	GLFWwindow *window;

	glm::vec3 displacement = glm::vec3(0.0f, 0.0f, 0.0f);

	Light(Shader *program, glm::vec3 displacement, RawModel *model, GLFWwindow *window) {
		this->program = program;
		this->model = model;
		this->window = window;
		this->displacement = displacement;
	}
	
	
	void generate() {
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		float rotation = glfwGetTime() * 50.0f;
		model = glm::translate(model, displacement);
		//model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 0.0f, 1.0f));
		glUniform4fv(glGetUniformLocation(program->ID, "color"), 1, glm::value_ptr(color));
                glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

	}
	void controls() {
		const float speed = 0.05f;
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                        displacement.y += speed;
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                        displacement.y -= speed;
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                        displacement.x -= speed;
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                        displacement.x += speed;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
                        displacement.z -= speed;
                if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
                        displacement.z += speed;

	}


};


#endif
