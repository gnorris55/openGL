#ifndef TERRAIN_H
#define TERRAIN_H

#include "raw_model.h"
#include "shader.h"
#include "sphere.h"

class Terrain {
	
	public:

	//constant variables
	float SIZE = 128;
	int VERTEX_COUNT = 128;
	int SQUARE_POINTS = 18;

	Shader *program;
	glm::vec3 points[128][128];
	float heightMap[128][128];

	float x;
	float z;

	Terrain(Shader *shader,int gridX, int gridZ) {
		x = gridX * SIZE;
		z = gridX * SIZE;
		program = shader;

	}

	void generate() {
		glm::vec4 color = glm::vec4(0.22f, 0.91f, 0.19f, 1.0f);
		glm::mat4 model = glm::mat4(1.0f);
                float rotation = glfwGetTime() * 50.0f;

		model = glm::translate(model, glm::vec3(-SIZE/2, 0.0f, -SIZE/2));

                glUniform4fv(glGetUniformLocation(program->ID, "color"), 1, glm::value_ptr(color));
		glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	
	}

	RawModel generateTerrain(Loader loader) {
		int count = VERTEX_COUNT*VERTEX_COUNT;
		int posX, posY, posZ;
		int vertexPointer = 0;
		int squarePoints = 18;
		float vertices[count*SQUARE_POINTS];

		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				posX = (float)j/((float) VERTEX_COUNT - 1) * SIZE;
				posY = 0;
				posZ = (float)i/((float) VERTEX_COUNT - 1) * SIZE;
				points[i][j] = glm::vec3(posX, posY, posZ);
				heightMap[i][j] = posY;
			}
			
		}
		mapVertices(vertices);

		return loader.loadToVAO(vertices, sizeof(vertices) - ((VERTEX_COUNT*sizeof(float)*SQUARE_POINTS)*2) + (sizeof(float)*SQUARE_POINTS));
	}

	RawModel raiseVertices(Editor *pointer, Loader loader) {
		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				glm::vec3 point = points[i][j];
				float posX = point.x - (float)VERTEX_COUNT/2;
				float posZ = point.z - (float)VERTEX_COUNT/2;
				if (posX < pointer->displacement.x + pointer->radius && posX > pointer->displacement.x - pointer->radius &&
				    posZ < pointer->displacement.z + pointer->radius && posZ > pointer->displacement.z - pointer->radius) {
				
					points[i][j].y += 0.10f;
					std::cout << pointer->displacement.x << ", " << pointer->displacement.z << "\n";
					std::cout << posX << ", " <<  posZ << "\n";
				}

			}
		}
		return updateVertices(loader);
	

	}
	
	RawModel updateVertices(Loader loader) {
		float vertices[VERTEX_COUNT*VERTEX_COUNT*SQUARE_POINTS];
		mapVertices(vertices);

		return loader.loadToVAO(vertices, sizeof(vertices) - ((VERTEX_COUNT*sizeof(float)*SQUARE_POINTS)*2) + (sizeof(float)*SQUARE_POINTS));
	}
			
	void mapVertices(float vertices[]) {
		int vertex = 0;
		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				if ((j != VERTEX_COUNT-1 && i != VERTEX_COUNT-1) || (j == VERTEX_COUNT-1 && i == VERTEX_COUNT-1)) {
					vertexToElement(vertices, &vertex, points[i][j]);
					vertexToElement(vertices, &vertex, points[i][j+1]);
					vertexToElement(vertices, &vertex, points[i+1][j]);
					vertexToElement(vertices, &vertex, points[i][j+1]);
					vertexToElement(vertices, &vertex, points[i+1][j+1]);
					vertexToElement(vertices, &vertex, points[i+1][j]);
				}

			}

		}
	}

	void printPoints() {
		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				glm::vec3 point = points[i][j];
				std::cout << "["<< point.x << ", " << point.y << ", " << point.z << "]\n";
			}
		}
	}

	private:

	void vertexToElement(float vertices[], int *vertex,  glm::vec3 vector) {
		vertices[*vertex] = vector.x;
		vertices[*vertex+1] = vector.y;
		vertices[*vertex+2] = vector.z;
		*vertex += 3;
	}

};


#endif
