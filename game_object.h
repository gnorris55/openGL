#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raw_model.h"
#include "shader.h"

class GameObject {


	public:

	float G = -9.81; 

	Shader *program;
	GLFWwindow *window;
	RawModel *rawModel;
	Renderer renderer;

	glm::vec3 color;
	glm::vec3 displacement;
	glm::vec3 direction;
	float velocity = 0.0f;
	float mass = 0.0f;
	float rotation = 0.0f;	

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

	void activateRigidBody(float mass, float initialVelocity) {
		this->mass = mass;
		this->velocity = initialVelocity;
	}

	void controls() {
                const float speed = 0.20f;
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                        displacement.x -= speed;
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                        displacement.x += speed;
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                        displacement.z -= speed;
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                        displacement.z += speed;
        }
};

class SphereObject: public GameObject {

	public:

	int radius;
	glm::vec3 rollVec = glm::vec3(5.0f, 0.0f, 0.0f);

	SphereObject(int radius, Shader *program, GLFWwindow *window, Renderer renderer, glm::vec3 position, glm::vec3 color, RawModel *rawModel) : GameObject(program, window, renderer, position,  color, rawModel) {
		this->radius = radius;
        }
	
	void render(glm::vec3 lightPosition, glm::vec3 lightColor, glm::vec3 viewPos) {

		glm::mat4 model = glm::mat4(1.0f);

		model = glm::translate(model, displacement);
		model = glm::rotate(model, glm::radians(rotation),glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, (float)radius, 0.0f));
		model = glm::rotate(model, glm::radians(rollVec.z),glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, glm::radians(rollVec.x),glm::vec3(0.0f, 0.0f, 1.0f));
		std::cout << rollVec.x << ", " << rollVec.z << "\n";

                glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniform3fv(glGetUniformLocation(program->ID, "objectColor"), 1, glm::value_ptr(color));
                glUniform3fv(glGetUniformLocation(program->ID, "lightColor"), 1, glm::value_ptr(lightColor));
                glUniform3fv(glGetUniformLocation(program->ID, "lightPos"), 1, glm::value_ptr(lightPosition));
                glUniform3fv(glGetUniformLocation(program->ID, "viewPos"), 1, glm::value_ptr(viewPos));

		renderer.render(*rawModel, GL_TRIANGLES);
	}
	
	void controls() {
                  const float speed = 0.15f;
		  const float rollSpeed = 10.0f;
                  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                          displacement.x -= speed;
			  rollVec.x -= rollSpeed;
			  rollVec.z = 0;
		  }
                  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                          displacement.x += speed;
                          rollVec.x += rollSpeed;
			  rollVec.z = 0;
		  }
                  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                          displacement.z -= speed;
                          rollVec.z -= rollSpeed;
			  rollVec.x = 0;
		  }
                  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                          displacement.z += speed;
                          rollVec.z += rollSpeed;
			  rollVec.x = 0;
		  }
		std::cout << rollVec.x << "\n";
          }





};

#endif
