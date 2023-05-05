#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raw_model.h"
#include "shader.h"



class GameObject {


	public:

	Shader *program;
	GLFWwindow *window;
	RawModel *rawModel;
	Renderer renderer;

	glm::vec3 color;
	glm::vec3 displacement;

	GameObject(Shader *program, GLFWwindow *window, Renderer renderer, glm::vec3 position, glm::vec3 color, RawModel *rawModel) {
		this->program = program;
                this->window = window;
		this->rawModel = rawModel;
		this->renderer = renderer;
		this->color = color;
                displacement = position;

	}
	

	//sets up uniforms for lighting, textures, etc...
	void render(glm::vec3 lightPosition, glm::vec3 lightColor, glm::vec3 viewPos) {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, displacement);

                glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniform3fv(glGetUniformLocation(program->ID, "objectColor"), 1, glm::value_ptr(color));
                glUniform3fv(glGetUniformLocation(program->ID, "lightColor"), 1, glm::value_ptr(lightColor));
                glUniform3fv(glGetUniformLocation(program->ID, "lightPos"), 1, glm::value_ptr(lightPosition));
                glUniform3fv(glGetUniformLocation(program->ID, "viewPos"), 1, glm::value_ptr(viewPos));

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
