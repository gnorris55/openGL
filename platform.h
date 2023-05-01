#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Rectangle {

        public:
        Shader *program;
	unsigned int VAO;
        glm::vec3 position;
        float xScale;
        float zScale;
        float yScale;

        Rectangle(Shader *program, glm::vec3 position, float xScale, float yScale, float zScale) {
                this->program = program;
                this->position = position;
		this->position.y -= 0.5f;
                this->xScale = xScale;
                this->yScale = yScale;
                this->zScale = zScale;

        }

	void createRectangle() {
		float vertices[] = {
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

                unsigned int VBO;
                glGenVertexArrays(1, &VAO);
                glGenBuffers(1, &VBO);

                glBindVertexArray(VAO);
                glBindBuffer(GL_ARRAY_BUFFER, VBO);
                glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
                glEnableVertexAttribArray(0);
                glBindVertexArray(VAO);
	}

        glm::mat4 render() {

                glm::vec4 color = glm::vec4(0.22f, 0.91f, 0.19f, 1.0f);
                glUniform4fv(glGetUniformLocation(program->ID, "color"), 1, glm::value_ptr(color));

                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, position);
                model = glm::scale(model,glm::vec3(xScale, 0.5f, zScale));
                glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"),
                                        1, GL_FALSE, glm::value_ptr(model));
                glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(VAO);

                return model;
        }

};

