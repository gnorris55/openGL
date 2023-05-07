#ifndef CAMERA_H
#define CAMERA_H

#include "shader.h"

class Camera {

	public:
	
	Shader *program;
	glm::vec3 displacement;
	float xAngle;
	float yAngle;
	float zAngle;

	Camera(glm::vec3 displacement, float xAngle = 0, float yAngle = 0, float zAngle = 0) {
		this->displacement = displacement;
		this->xAngle = xAngle;
		this->yAngle = yAngle;
		this->zAngle = zAngle;
	}

	void define(unsigned int programID) {
		glm::mat4 view = glm::mat4(1.0f);
                glm::mat4 projection = glm::mat4(1.0f);

                view = glm::translate(view, displacement);
                view = glm::rotate(view, glm::radians(xAngle),glm::vec3(1.0f, 0.0f, 0.0f));
                view = glm::rotate(view, glm::radians(yAngle),glm::vec3(0.0f, 1.0f, 0.0f));
                view = glm::rotate(view, glm::radians(zAngle),glm::vec3(0.0f, 0.0f, 1.0f));
                projection = glm::perspective(glm::radians(45.0f), (800.0f/600.0f), 0.1f, 100.0f);
		
		glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1, GL_FALSE, glm::value_ptr(view));
                glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
	}
	


};

#endif
