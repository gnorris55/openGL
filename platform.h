#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "raw_model.h"

class Rectangle {

        public:
        Shader *program;
	RawModel *model;
        glm::vec3 position;
        float xScale;
        float zScale;
        float yScale;

        Rectangle(Shader *program, glm::vec3 position, float xScale, float yScale, float zScale, RawModel *rectangleModel) {
                this->program = program;
                this->position = position;
		this->position.y -= 0.5f;
                this->xScale = xScale;
                this->yScale = yScale;
                this->zScale = zScale;
		model = rectangleModel;
	}

        void generate() {

                glm::vec4 color = glm::vec4(0.22f, 0.91f, 0.19f, 1.0f);
                glUniform4fv(glGetUniformLocation(program->ID, "color"), 1, glm::value_ptr(color));

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, position);
                model = glm::scale(model,glm::vec3(xScale, 0.5f, zScale));
                glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

        }

};

