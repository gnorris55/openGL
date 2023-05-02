#ifndef TERRAIN_H
#define TERRAIN_H

#include "raw_model.h"

class Terrain {
	
	public:

	//constant variables
	float SIZE = 10;
	int VERTEX_COUNT = 10;

	RawModel *model;
	float x;
	float z;

	Terrain(int gridX, int gridZ) {
		x = gridX * SIZE;
		z = gridX * SIZE;
	}

	RawModel generate(Loader loader) {
		int count = VERTEX_COUNT*VERTEX_COUNT;
		int posX, posY, posZ;
		int vertexPointer = 0;
		float vertices[count*18];
		glm::vec3 points[VERTEX_COUNT][VERTEX_COUNT];

		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				posX = j;
				posY = 0;
				posZ = i;
				points[i][j] = glm::vec3(posX, posY, posZ);
				std::cout << posX << "\n";
			}
		}

		int vertex = 0;
		for (int j = 0; j < VERTEX_COUNT; j++) {
			for (int i = 0; i < VERTEX_COUNT; i++) {
				vertexToElement(vertices, &vertex, points[i][j]);
				vertexToElement(vertices, &vertex, points[i+1][j]);
				vertexToElement(vertices, &vertex, points[i][j+1]);
				vertexToElement(vertices, &vertex, points[i][j+1]);
				vertexToElement(vertices, &vertex, points[i+1][j]);
				vertexToElement(vertices, &vertex, points[i+1][j+1]);

			}
		}

		return loader.loadToVAO(vertices, sizeof(vertices));
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
