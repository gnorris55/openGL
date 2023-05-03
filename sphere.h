#ifndef SPHERE_H
#define SPHERE_H


#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <math.h>
#include <glm/gtx/string_cast.hpp>
#include "shader.h"
#include "raw_model.h"

class Sphere {
	public:

	Shader *program;
	glm::vec3 displacement;
	GLFWwindow *window;
	RawModel *model;

	int radius;
	float gravity = -9.8;
	float velocity = 0.05f;
	float startTime = 0.0f;

	Sphere(Shader *program, glm::vec3 position, int radius, GLFWwindow *window, RawModel *sphereModel) {
		this->program = program;
		this->window = window;
		this->radius = radius;
		displacement = position;
		model = sphereModel;

	}

	void generate() {
		glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
                glm::mat4 model = glm::mat4(1.0f);
                float rotation = glfwGetTime() * 50.0f;
                model = glm::translate(model, displacement);

                //model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 0.0f, 1.0f));
                model = glm::scale(model, glm::vec3(radius, radius, radius));

                glUniform4fv(glGetUniformLocation(program->ID, "color"), 1, glm::value_ptr(color));
                glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	}
	
	void controls() {
                const float speed = 0.05f;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			displacement.y += velocity;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			displacement.y -= velocity;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			displacement.x -= velocity;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			displacement.x += velocity;
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
			displacement.z -= velocity;
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
			displacement.z += velocity;
        }
};


class Editor: public Sphere {
	using Sphere::Sphere;
	public:
 	int controls() {
                const float speed = 0.05f;
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
                        displacement.z -= velocity;
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
                        displacement.z += velocity;
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
                        displacement.x -= velocity;
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
                        displacement.x += velocity;
                if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
			return 1;
                if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			return -1;

		return 0;	
        }

	
};















/*
class Pointer {

	public:
		Shader *program;
		glm::vec3 startingPosition  = glm::vec3(0.0f, 0.0f, 0.0f);
		int numVertices;
		float length = -5.0f;
		float rotation = 0.0f;
		float shotAngle = 0.0f;
		float gravity = 9.8f;
		unsigned int VAO;
			
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
			glBindVertexArray(VAO);
			glEnableVertexAttribArray(1);	
			glDrawArrays(GL_TRIANGLES, 0, numVertices);
		}
};
class GolfBall: public Sphere {
	using Sphere::Sphere;
	public:

	Pointer pointer;
	float initialVelocity = 20.0f;
	float landingSpot = 0.0f;
	int shotTaken = 0;
	int shotLanded = 0;
	unsigned int VAOSphere, VAOPointer;

	void assignVertices() {

		float pointerVertices[] = {
		    -0.5f, -0.5f, -0.5f,
		     0.5f, -0.5f, -0.5f,  
		     0.5f,  0.5f, -0.5f,
		     0.5f,  0.5f, -0.5f, 
		    -0.5f,  0.5f, -0.5f, 
		    -0.5f, -0.5f, -0.5f, 

		    -0.5f, -0.5f,  0.5f,
		     0.5f, -0.5f,  0.5f, 
		     0.5f,  0.5f,  0.5f, 
		     0.5f,  0.5f,  0.5f, 
		    -0.5f,  0.5f,  0.5f,
		    -0.5f, -0.5f,  0.5f, 

		    -0.5f,  0.5f,  0.5f,
		    -0.5f,  0.5f, -0.5f, 
		    -0.5f, -0.5f, -0.5f, 
		    -0.5f, -0.5f, -0.5f,
		    -0.5f, -0.5f,  0.5f, 
		    -0.5f,  0.5f,  0.5f,

		     0.5f,  0.5f,  0.5f,
		     0.5f,  0.5f, -0.5f,
		     0.5f, -0.5f, -0.5f, 
		     0.5f, -0.5f, -0.5f, 
		     0.5f, -0.5f,  0.5f, 
		     0.5f,  0.5f,  0.5f, 

		    -0.5f, -0.5f, -0.5f,
		     0.5f, -0.5f, -0.5f,
		     0.5f, -0.5f,  0.5f,
		     0.5f, -0.5f,  0.5f,
		    -0.5f, -0.5f,  0.5f,  
		    -0.5f, -0.5f, -0.5f, 

		    -0.5f,  0.5f, -0.5f,
		     0.5f,  0.5f, -0.5f, 
		     0.5f,  0.5f,  0.5f,
		     0.5f,  0.5f,  0.5f,
		    -0.5f,  0.5f,  0.5f,
		    -0.5f,  0.5f, -0.5f
		};

		float sphereVertices[100000];
		numVertices = createSphere(sphereVertices);
		
		unsigned int VBOSphere, VBOPointer, VAO[2];
		glGenVertexArrays(2, VAO);

		VAOSphere = VAO[0];
		VAOPointer = VAO[1];

		std::cout << VAOSphere << VAOPointer << "\n";
		

		glGenBuffers(1, &VBOSphere);
		glGenBuffers(1, &VBOPointer);


		glBindVertexArray(VAOSphere);	
		glBindBuffer(GL_ARRAY_BUFFER, VBOSphere);
		glBufferData(GL_ARRAY_BUFFER, sizeof(sphereVertices), sphereVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(VAOSphere);	
		
		glBindVertexArray(VAOPointer);	
		glBindBuffer(GL_ARRAY_BUFFER, VBOPointer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(pointerVertices), pointerVertices, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
		glEnableVertexAttribArray(0);
		glBindVertexArray(VAOPointer);	


	}
	
	glm::mat4 render() {
			
	        glm::vec4 color = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		glm::mat4 model = glm::mat4(1.0f);
		float rotation = glfwGetTime() * 50.0f;
		model = glm::translate(model, displacement);	
		
		if (shotTaken == 1 && shotLanded == 0) {
			
			float radianAngle = -(pointer.shotAngle*3.14)/180;
			float flightTime = glfwGetTime() - startTime;
			float currZ = -initialVelocity*flightTime*cos(radianAngle);
			float currY = initialVelocity*flightTime*sin(radianAngle) + 0.5*gravity*pow(flightTime, 2);
			
			//std::cout << "flight time: " << flightTime << ", y position: " << currY << ", x position: " << currX << "\n";
			model = glm::rotate(model, glm::radians(pointer.rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, glm::vec3(0.0f, currY, currZ));
			if (currY <= 0) {
				shotLanded = 1;
				movement = glm::vec3(0.0f, currY, currZ);
				std::cout << "current z pos: " << currZ << "\n";
				float xVelocity = initialVelocity*cos(radianAngle);
				float yVelocity = initialVelocity*sin(radianAngle) + gravity*flightTime;
				std::cout << "x velocity: " << xVelocity << ", y velocity: " << yVelocity << "\n";
			}

		} else if (shotLanded == 1) {
			model = glm::rotate(model, glm::radians(pointer.rotation), glm::vec3(0.0f, 1.0f, 0.0f));
			model = glm::translate(model, movement);
		}	
		else {
			pointer.render();
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
		rotation = glfwGetTime()*50.0f;
		model = glm::rotate(model, glm::radians(rotation), glm::vec3(1.0f, 0.0f, 1.0f));

		glUniform4fv(glGetUniformLocation(program->ID, "color"), 1, glm::value_ptr(color));
		glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

		// rendering the cube

		glBindVertexArray(VAOSphere);
		glDrawArrays(GL_LINES, 0, numVertices);
		glBindVertexArray(VAOSphere);
	
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
	
	
	void initializePointer() {
		pointer.program = program;
		pointer.startingPosition = displacement;
		pointer.startingPosition.y -= radius;
		pointer.startingPosition.z += radius;
		pointer.numVertices = numVertices;
		pointer.VAO = VAOPointer;
	}

};

*/

#endif
