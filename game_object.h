#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raw_model.h"
#include "shader.h"
#include "calculations.h"

class GameObject {

	public:

	float G = 9.81*2; //dm/s1 
	float AIR_DENSITY = 2.645f; //kg/m^3 lb/dm^3
	float DYNAMIC_VISCOSITY = 0.000018;

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

			float drag = 0.1;
			float z = -((initialVelocity*cos(glm::radians(angle)) + G)/drag) * (1 - exp(-drag*flightTime));
			float y = ((drag * initialVelocity*sin(glm::radians(angle)) + G)/pow(drag, 2)) * (1 - exp(-drag*flightTime)) - G*(flightTime/drag);
			flightVec = glm::vec3(0.0f, y, z);
			displacement = startingPosition + flightVec;
			setVelocityVector(angle, initialVelocity, drag, flightTime);
		}

	}


	void setTime() {
		startTime = glfwGetTime();
	}

	void setVelocityVector(float angle, float initialVelocity, float dragForce, int currTime) {
		// for drag
	
		float z = -initialVelocity*cos(glm::radians(angle))*exp(-dragForce*currTime);
		float y = ((G/dragForce) + initialVelocity*sin(glm::radians(angle)))*exp(-dragForce*currTime) - (G/dragForce);
		float yVelocity = abs(initialVelocity)*sin(glm::radians(angle)) - G*currTime;
		float zVelocity = -initialVelocity*cos(glm::radians(angle));
		//std::cout << "1) y velocity: " << yVelocity << " z velocity: " << zVelocity << "\n";
		std::cout << "2) y velocity: " << y << " z velocity: " << z << "\n";
		velocityVector = glm::vec3(0.0f, y, z);	
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


class Aimer {

	public:

	Renderer renderer;
	RawModel rawModel;
	Math math;
	Loader loader;

	int controls(GLFWwindow *window) {
		if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) { return 1; }       
		if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) { return -1; }       
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) { return 2; }       
		
		return 0;
	}

	RawModel generateModel(Loader loader, float angle, float velocity, float drag, float G) {
		
		int numVertices = 10;
		float vertices[numVertices*18];
		float normals[numVertices*18];
		return setVertices(numVertices, vertices, normals, angle, velocity, drag, G);
		

	}
	
	RawModel setVertices(int numVertices, float vertices[], float normals[], float angle, float velocity, float drag, float G) {


		int vertex = 0; 
		int normalsVertex = 0;
		glm::vec3 points[numVertices][2];
		for (int i = 0; i < numVertices; i++) {
			float timeDistance = (float)i/(float)numVertices * 0.75;
			float x = (float)(numVertices - i) / (float)(numVertices);
			//std::cout << timeDistance << "\n";
			//std::cout << x << "\n";
			float z = -((velocity*cos(glm::radians(angle)) + G)/drag) * (1 - exp(-drag* timeDistance));
                	float y = ((drag * velocity*sin(glm::radians(angle)) + G)/pow(drag, 2)) * (1 - exp(-drag*timeDistance)) - G*(timeDistance/drag);
			
			points[i][0] = glm::vec3(0.0, y, z);
			points[i][1] = glm::vec3(x, y, z);
		}
		
		//TODO: connect triangles and set normals for the aimer mesh
		for (int i = 0; i < numVertices; i++) {
			for ( int j = 0; j < 1; ++j) {
                        	glm::vec3 inputPoints[] = {points[i][j], points[i][j+1], points[i+1][j], points[i+1][j+1]};
                        	math.mapSquare(vertices, normals, &vertex, &normalsVertex, inputPoints, glm::vec3(-1.0f, -1.0f, -1.0f));
			}
                }


		//for (int i = 0; i < numVertices*18; i++) {
			//std::cout << vertices[i] << "\n";
		//}

		return loader.loadToVAO(vertices, normals, normalsVertex*sizeof(float) - 18*sizeof(float));

	}
	
	void render(glm::vec3 displacement, Shader *program, RawModel *rawModel) {
                
		//TODO: compatability for shader;
		glm::mat4 model = glm::mat4(1.0f);
		glm::vec3 color = glm::vec3 (1.0f, 0.388f, 0.20f);
                model = glm::translate(model, displacement);

                glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniform3fv(glGetUniformLocation(program->ID, "color"), 1, glm::value_ptr(color));
		
                renderer.render(*rawModel, GL_TRIANGLES);

	}

	//private:

};

class GolfBall: public SphereObject {
	
	public:
	
	float shotAngle = 0;
	float shotVelocity = 0;
	float drag = 0.1;
	bool collision = true;

	Loader loader;
	Aimer *aimer;

 	GolfBall(float radius, Shader *program, GLFWwindow *window, Renderer renderer, glm::vec3 position, glm::vec3 color, RawModel *rawModel, float shotAngle, float shotVelocity, Loader loader) : SphereObject(radius, program, window, renderer, position,  color, rawModel) {
		this->shotAngle = shotAngle;
		this->shotVelocity = shotVelocity;
		this->loader = loader;

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
		//renderAimer();			
 
	}

	void renderAimer(Shader *aimerProgram) {
		if (collision == true) {
			RawModel aimerRawModel = aimer->generateModel(loader, shotAngle, shotVelocity, drag, G);
			glm::vec3 aimerPos = displacement + glm::vec3(-radius, radius, -radius);
			aimer->render(aimerPos, aimerProgram, &aimerRawModel);
		}  

	}
	
	void projectileMotion() {	
		if (collision == false) {
			
			float flightTime = glfwGetTime() - startTime;

			float z = -((shotVelocity*cos(glm::radians(shotAngle)) + G)/drag) * (1 - exp(-drag*flightTime));
			float y = ((drag * shotVelocity*sin(glm::radians(shotAngle)) + G)/pow(drag, 2)) * (1 - exp(-drag*flightTime)) - G*(flightTime/drag);
			flightVec = glm::vec3(0.0f, y, z);
			displacement = startingPosition + flightVec;
			setVelocityVector(drag, flightTime);
		}

	}
	
	void setVelocityVector(float dragForce, int currTime) {
		// for drag
	
		float z = -shotVelocity*cos(glm::radians(shotAngle))*exp(-dragForce*currTime);
		float y = ((G/dragForce) + shotVelocity*sin(glm::radians(shotAngle)))*exp(-dragForce*currTime) - (G/dragForce);
		//float yVelocity = abs(initialVelocity)*sin(glm::radians(angle)) - G*currTime;
		//float zVelocity = -initialVelocity*cos(glm::radians(angle));
		//std::cout << "1) y velocity: " << yVelocity << " z velocity: " << zVelocity << "\n";
		//std::cout << "2) y velocity: " << y << " z velocity: " << z << "\n";
		velocityVector = glm::vec3(0.0f, y, z);	
	}

	void controls() {
		
		int aimerCommand =  aimer->controls(window);
		//std::cout << aimerCommand << "\n";
                const float objectSpeed = 0.15f;
		const float aimSpeed = 1.0f;
                if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
                        startingPosition.x -= objectSpeed;
                }
                if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
                        startingPosition.x += objectSpeed;
                }
                if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
                        startingPosition.z -= objectSpeed;
                }
                if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
                      	startingPosition.z += objectSpeed;
                }
		if (aimerCommand == 1 && shotAngle < 180.0f) {
			shotAngle += aimSpeed;
		      	//aimer.alterModel(shotAngle, shotVelocity)'
		}
		if (aimerCommand == -1 && shotAngle > 5.0f) {	
			shotAngle -= aimSpeed; 
		      	//aimer.alterModel(shotAngle, shotVelocity)'
		} 
		if (aimerCommand == 2) {
			setTime();
			collision = false;
		}
		//std::cout << shotAngle << "\n";
		displacement = startingPosition;
          }

};
#endif
