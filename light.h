#ifndef LIGHT_H
#define LIGHT_H

#include "raw_model.h"
#include "shader.h"

class Light {

	public:

	Shader *program;
	RawModel *rawModel;
	GLFWwindow *window;
	Renderer renderer;

	glm::vec3 displacement = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 color;

	Light(Shader *program, Renderer renderer, glm::vec3 displacement, glm::vec3 color, RawModel *model, GLFWwindow *window) {
		this->program = program;
		this->rawModel = model;
		this->window = window;
		this->displacement = displacement;
		this->color = color;
		this->renderer = renderer;
	}
	
	
	void render() {
		
		glm::mat4 model = glm::mat4(1.0f);
		
		model = glm::translate(model, displacement);
		glUniform4fv(glGetUniformLocation(program->ID, "color"), 1, glm::value_ptr(color));
                glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		renderer.render(*rawModel, GL_TRIANGLES);

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
