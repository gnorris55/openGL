#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raw_model.h"
#include "shader.h"

class GameObject {


	public:

	float G = -98.1; //dm/s1 
	float AIR_DENSITY = 26.45f; //kg/m^3 lb/dm^3
	float DYNAMIC_VISCOSITY = 0.0018;

	Shader *program;
	GLFWwindow *window;
	RawModel *rawModel;
	Renderer renderer;

	glm::vec3 color;
	glm::vec3 startingPosition;
	glm::vec3 displacement;
	glm::vec3 direction;
	float velocity = 0.0f;
	float mass = 0.10; //lb
	float rotation = 0.0f;

	GameObject(Shader *program, GLFWwindow *window, Renderer renderer, glm::vec3 position, glm::vec3 color, RawModel *rawModel) {
		this->program = program;
                this->window = window;
		this->rawModel = rawModel;
		this->renderer = renderer;
		this->color = color;
                startingPosition = position;
		displacement = startingPosition;

	}
	

	//sets up uniforms for lighting, textures, etc...
	void render(glm::vec3 lightPosition, glm::vec3 lightColor, glm::vec3 viewPos) {

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, startingPosition);

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

	glm::vec3 flightVec = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 velocityVector = glm::vec3(0.0f, 0.0f, 0.0f);

	float radius;
	float angle = 45.0f;
        float velocity = 20.0f;
	bool collision = false;
	float startTime = 0.0f;	

	SphereObject(float radius, Shader *program, GLFWwindow *window, Renderer renderer, glm::vec3 position, glm::vec3 color, RawModel *rawModel) : GameObject(program, window, renderer, position,  color, rawModel) {
		this->radius = radius;
		std::cout << radius << "\n";
        }
	
	void render(glm::vec3 lightPosition, glm::vec3 lightColor, glm::vec3 viewPos) {

		glm::mat4 model = glm::mat4(1.0f);
	
		model = glm::translate(model, displacement);
		model = glm::rotate(model, glm::radians(rotation),glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::translate(model, glm::vec3(0.0f, (float)radius, 0.0f));
		model = glm::scale(model, glm::vec3(radius, radius, radius));

                glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniform3fv(glGetUniformLocation(program->ID, "objectColor"), 1, glm::value_ptr(color));
                glUniform3fv(glGetUniformLocation(program->ID, "lightColor"), 1, glm::value_ptr(lightColor));
                glUniform3fv(glGetUniformLocation(program->ID, "lightPos"), 1, glm::value_ptr(lightPosition));
                glUniform3fv(glGetUniformLocation(program->ID, "viewPos"), 1, glm::value_ptr(viewPos));

		renderer.render(*rawModel, GL_TRIANGLES);
	}
	
	//TODO: add drag
	void projectileMotion(float angle, float initialVelocity) {	
		if (collision == false) {
			
			float flightTime = glfwGetTime() - startTime;
			float drag = glm::length(velocityVector)* mass;
			float zPosition = (-initialVelocity)*flightTime*cos(glm::radians(angle));
			float yPosition = abs(initialVelocity)*flightTime*sin(glm::radians(angle)) + (0.5)*mass*G*pow(flightTime, 2);
			flightVec = glm::vec3(0.0f, yPosition, zPosition);
			displacement = startingPosition + flightVec;
			setVelocityVector(angle, initialVelocity, drag, flightTime);
		}

	}


	void setTime() {
		startTime = glfwGetTime();
	}

	void setVelocityVector(float angle, float initialVelocity, float dragForce, int currTime) {
		// for drag
		//float z = -initialVelocity*cos(glm::radians(angle))*exp(-dragForce*currTime);
		//float y = ((-G/dragForce) + initialVelocity*sin(glm::radians(angle)))*exp(-dragForce*currTime) - (-G/dragForce);
		float yVelocity = abs(initialVelocity)*sin(glm::radians(angle)) + G*mass*currTime;
		float zVelocity = -initialVelocity*cos(glm::radians(angle));
		//std::cout << "y velocity: " << yVelocity << "z velocity" << zVelocity << "\n";
		velocityVector = glm::vec3(0.0f, yVelocity, zVelocity);	
	}
	
	void controls() {
                  const float speed = 0.15f;
                  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                          displacement.x -= speed;
		  }
                  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                          displacement.x += speed;
		  }
                  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                          displacement.z -= speed;
		  }
                  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                          displacement.z += speed;
		  }
          }





};

#endif
