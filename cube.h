#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include "shader.h"
#include <glm/gtx/string_cast.hpp>

class Pointer {

	
	public:
		Shader *program;
		glm::vec3 startingPosition  = glm::vec3(0.0f, 0.0f, 0.0f);
		float length = -5.0f;
		float rotation = 0.0f;
		float shotAngle = 0.0f;
		float gravity = 9.8f;
		
		void render() {
			
			//getting uniform attributes
			glm::vec4 color = glm::vec4(1.0f, 0.5f, 0.2f, 1.0f);
			glm::mat4 model = glm::mat4(1.0f);
			
			// setting position
			model = glm::translate(model, startingPosition);
			model = glm::rotate(model, glm::radians(-shotAngle), glm::vec3(1.0f, 0.0f, 0.0f));	
			model = glm::rotate(model, glm::radians(rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0, 0.0, (0.5*length)));
			model = glm::scale(model, glm::vec3(0.2,0.1, length));
			//std::cout << glm::to_string(model) << std::endl;

			//setting uniform variable
			glUniform4fv(glGetUniformLocation(program->ID, "color"), 1, glm::value_ptr(color));
			glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));	

			// rendering the cube
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
};



class Cube {
	public:

	Shader *program;
	Pointer pointer;
	glm::vec3 startingPosition;
	glm::vec3 movement = glm::vec3(0.0f, 0.0f, 0.0f);
	GLFWwindow *window;

	int vertices;
	int size;
	float gravity = -9.8;
	float landingSpot = 0.0f;
	int shotTaken = 0;
	int shotLanded = 0;
	float initialVelocity = 20.0f;
	float startTime = 0.0f;

	Cube(Shader *program, glm::vec3 position, int size, int vertices, GLFWwindow *window) {
		this->program = program;
		this->size = size;
		this->window = window;
		this->vertices = vertices;
		startingPosition = position;
		initializePointer();
	}

	glm::mat4 render() {
			
	        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, startingPosition);
		if (shotTaken == 1 && shotLanded == 0) {
			//TODO trajectory of the cube as a function of time
			float radianAngle = -(pointer.shotAngle*3.14)/180;
			float flightTime = glfwGetTime() - startTime;
			float currZ = -initialVelocity*flightTime*cos(radianAngle);
			float currY = initialVelocity*flightTime*sin(radianAngle) + 0.5*gravity*pow(flightTime, 2);
			
			model = glm::rotate(model, glm::radians(pointer.rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, currY, currZ));
			if (currY <= 0) {
				shotLanded = 1;
				movement = glm::vec3(0.0f, currY, currZ);
				float xVelocity = initialVelocity*cos(radianAngle);
				float yVelocity = initialVelocity*sin(radianAngle) + gravity*flightTime;
				std::cout << "x velocity: " << xVelocity << ", y velocity: " << yVelocity << "\n";
			}

		} else if (shotLanded == 1) {
			model = glm::rotate(model, glm::radians(pointer.rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, movement);
		}	
		else {
			//pointer.render();
		}

		
		// moves the ball on the z axis depent on a trajectory
		if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS && shotTaken == 0) {
			float radianAngle = -(pointer.shotAngle*3.14)/180;
			startTime = glfwGetTime();
			shotTaken = 1;	
			std::cout << "shot angle on y-axis " <<  pointer.rotation << "\n";
			std::cout << "shot angle on z-axis: " <<  pointer.shotAngle << "\n";
			landingSpot = -pow(initialVelocity,2)*sin(2*radianAngle)/gravity;
			std::cout << "predicted landing position: " << landingSpot <<"\n";
		}

                	
		glUniform4fv(glGetUniformLocation(program->ID, "color"), 1, glm::value_ptr(color));
		glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// rendering the cube
		glDrawArrays(GL_TRIANGLES, 0, vertices);
	
		// return position for collision detection
		return model;

	}

	void controls() {
                const float speed = 0.05f;
		if (shotTaken == 0) {
			if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS && pointer.shotAngle > -88)
				pointer.shotAngle -= speed*15.0f;
			if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS && pointer.shotAngle < -5)
				pointer.shotAngle += speed*15.0f;
			if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS && pointer.rotation < 45)
				pointer.rotation += speed*15.0f;
			if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS && pointer.rotation > -45)
				pointer.rotation -= speed*15.0f;
		}	
        }

	private:
	
	void initializePointer() {
		pointer.program = program;
		pointer.startingPosition = startingPosition;
		pointer.startingPosition.y -= size/2;
		pointer.startingPosition.z += size/2;

	}
};
