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
	Renderer renderer;
	glm::vec3 color;
	glm::vec3 points[128][128];
	float heightMap[128][128];

	float x;
	float z;

	Terrain(Shader *program, Renderer renderer, int gridX, int gridZ, glm::vec3 color) {
		this->program = program;
		this->renderer = renderer;
		this->color = color;
		x = gridX * SIZE;
		z = gridX * SIZE;	
	}

	void render(RawModel *rawModel, glm::vec3 lightPosition, glm::vec3 lightColor, glm::vec3 viewPos) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-SIZE/2, 0.0f, -SIZE/2));

		glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniform3fv(glGetUniformLocation(program->ID, "objectColor"), 1, glm::value_ptr(color));
                glUniform3fv(glGetUniformLocation(program->ID, "lightColor"), 1, glm::value_ptr(lightColor));
                glUniform3fv(glGetUniformLocation(program->ID, "lightPos"), 1, glm::value_ptr(lightPosition));
                glUniform3fv(glGetUniformLocation(program->ID, "viewPos"), 1, glm::value_ptr(viewPos));

		renderer.render(*rawModel, GL_TRIANGLES);
	}

	RawModel generateTerrain(Loader loader) {
		int count = VERTEX_COUNT*VERTEX_COUNT;
		int posX, posY, posZ;
		int vertexPointer = 0;
		int squarePoints = 18;
		float vertices[count*SQUARE_POINTS];
		float normals[count*SQUARE_POINTS];

		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				posX = (float)j/((float) VERTEX_COUNT - 1) * SIZE;
				posY = 0;
				posZ = (float)i/((float) VERTEX_COUNT - 1) * SIZE;
				points[i][j] = glm::vec3(posX, posY, posZ);
				heightMap[i][j] = posY;
			}
			
		}
		mapVertices(vertices, normals);

		return loader.loadToVAO(vertices, normals, sizeof(vertices) - ((VERTEX_COUNT*sizeof(float)*SQUARE_POINTS)*2) + (sizeof(float)*SQUARE_POINTS));
	}

	RawModel raiseVertices(Editor *pointer, Loader loader, int type) {
		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				glm::vec3 point = points[i][j];
				float posX = point.x - (float)VERTEX_COUNT/2;
				float posZ = point.z - (float)VERTEX_COUNT/2;
				float xDistance = abs(pointer->displacement.x - posX);
				float zDistance = abs(pointer->displacement.z - posZ);
				float totalDistance = sqrt(pow(xDistance, 2) + pow(zDistance,2));
				
				if (totalDistance < pointer->radius) {
					
					float intensity = (pointer->radius-totalDistance)/pointer->radius;
					
					points[i][j].y += (type)*(0.10f * intensity);
					//std::cout << posX << ", " <<  posZ << "\n";
				}

			}
		}
		return updateVertices(loader);
	

	}
	
	RawModel updateVertices(Loader loader) {
		float vertices[VERTEX_COUNT*VERTEX_COUNT*SQUARE_POINTS];
		float normals[VERTEX_COUNT*VERTEX_COUNT*SQUARE_POINTS];
		mapVertices(vertices, normals);

		return loader.loadToVAO(vertices, normals, sizeof(vertices) - ((VERTEX_COUNT*sizeof(float)*SQUARE_POINTS)*2) + (sizeof(float)*SQUARE_POINTS));
	}
			
	void mapVertices(float vertices[], float normals[]) {
		int vertex = 0;
		int normalsVertex = 0;
		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				if ((j != VERTEX_COUNT-1 && i != VERTEX_COUNT-1) || (j == VERTEX_COUNT-1 && i == VERTEX_COUNT-1)) {

					//first triangle
					vertexToElement(vertices, &vertex, points[i][j]);
					vertexToElement(vertices, &vertex, points[i][j+1]);
					vertexToElement(vertices, &vertex, points[i+1][j]);
					
					glm::vec3 normalVec = calculateNormals(points[i][j], points[i][j+1], points[i+1][j]);
					glm::vec3 scalar = glm::vec3(-1.0f, -1.0f, -1.0f);
					normalVec *= scalar;
					vertexToElement(normals, &normalsVertex, normalVec);
					vertexToElement(normals, &normalsVertex, normalVec);
					vertexToElement(normals, &normalsVertex, normalVec);


					//second triangle
					vertexToElement(vertices, &vertex, points[i][j+1]);
					vertexToElement(vertices, &vertex, points[i+1][j+1]);
					vertexToElement(vertices, &vertex, points[i+1][j]);
					
					normalVec = calculateNormals(points[i][j+1], points[i+1][j+1], points[i+1][j]);
					normalVec *= scalar;
					vertexToElement(normals, &normalsVertex, normalVec);
					vertexToElement(normals, &normalsVertex, normalVec);
					vertexToElement(normals, &normalsVertex, normalVec);
					
					
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

	glm::vec3 calculateNormals(glm::vec3 vectorA, glm::vec3 vectorB, glm::vec3 vectorC) {
		//(B-A)x(C-A)
		return glm::cross(vectorB - vectorA, vectorC - vectorA);

	}

};


#endif
