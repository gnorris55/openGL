#ifndef TERRAIN_H
#define TERRAIN_H

#include "raw_model.h"
#include "shader.h"
#include "sphere.h"
#include "game_object.h"
#include "calculations.h"

template <int LENGTH>

class Terrain {

	public:

	//constant variables
	float SIZE = 128;
	int VERTEX_COUNT = LENGTH;
	int SQUARE_POINTS = 18;
	

	Shader *program;
	Math math;
	Renderer renderer;
	glm::vec3 color;
	glm::vec3 points[LENGTH][LENGTH];
	float heightMap[LENGTH][LENGTH];

	float worldX;
	float worldZ;
	
	// main functions
	
	Terrain(Shader *program, Renderer renderer, int gridX, int gridZ, glm::vec3 color) {
		this->program = program;
		this->renderer = renderer;
		this->color = color;
		worldX = gridX * SIZE - SIZE/2;
		worldZ = gridZ * SIZE - SIZE/2;
	}

	void render(RawModel *rawModel, glm::vec3 lightPosition, glm::vec3 lightColor, glm::vec3 viewPos) {
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(worldX, 0.0f, worldZ));

		glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
                glUniform3fv(glGetUniformLocation(program->ID, "objectColor"), 1, glm::value_ptr(color));
                glUniform3fv(glGetUniformLocation(program->ID, "lightColor"), 1, glm::value_ptr(lightColor));
                glUniform3fv(glGetUniformLocation(program->ID, "lightPos"), 1, glm::value_ptr(lightPosition));
                glUniform3fv(glGetUniformLocation(program->ID, "viewPos"), 1, glm::value_ptr(viewPos));

		renderer.render(*rawModel, GL_TRIANGLES);
	}

	RawModel generateTerrain(Loader loader) {
		int count = VERTEX_COUNT*VERTEX_COUNT*SQUARE_POINTS;
		float vertices[count];
		float normals[count];

		setPoints();
		mapVertices(vertices, normals);

		return loader.loadToVAO(vertices, normals, sizeof(vertices) - ((VERTEX_COUNT*sizeof(float)*SQUARE_POINTS)*2) + (sizeof(float)*SQUARE_POINTS));
	}

	void edit(Editor *editor, RawModel *terrainModel,  Loader loader) {
		
		// setting editor height depending on terrain
		float pointerHeight;
		getHeight(editor->displacement.x, editor->displacement.z, &pointerHeight);
                editor->displacement.y = pointerHeight;
		
		// altering terrain
		if (editor->mode == 1) {
			int command = editor->controls();
			*terrainModel = alterVertices(editor, loader, command);
		}
			
	}
	
	//assigns a height variable depending on the x and z coordinates and returns the normal of the triangle that those coordinates are in
	glm::vec3 getHeight(float x, float z, float *height) {	
		
		float posX = x - worldX;
                float posZ = z - worldZ;
		float gridSquareSize = SIZE / ((float)VERTEX_COUNT - 1);
		int gridX = (int)floor(posX/gridSquareSize);
		int gridZ = (int)floor(posZ/gridSquareSize);
		if (gridX >= VERTEX_COUNT - 1 || gridZ >= VERTEX_COUNT - 1 || gridX < 0 || gridZ < 0)
			return glm::vec3(0.0f, 0.0f, 0.0f);
		float xCoord = (fmodf(posX,gridSquareSize)/gridSquareSize);
		float zCoord = (fmodf(posZ,gridSquareSize)/gridSquareSize);
		if (xCoord <= (1-zCoord)) {
			*height = math.barryCentric(glm::vec3(0, heightMap[gridX][gridZ], 0), 
				     	glm::vec3(1, heightMap[gridX+1][gridZ], 0),
				     	glm::vec3(0, heightMap[gridX][gridZ+1], 1),
				     	glm::vec2(xCoord, zCoord));

			return math.calculateNormals(points[gridX][gridZ], points[gridX+1][gridZ], points[gridX][gridZ+1]);			
		} else {
			*height = math.barryCentric(glm::vec3(1, heightMap[gridX+1][gridZ], 0), 
				     	glm::vec3(1, heightMap[gridX+1][gridZ+1], 1),
				     	glm::vec3(0, heightMap[gridX][gridZ+1], 1),
				    	glm::vec2(xCoord, zCoord));

			return math.calculateNormals(points[gridX+1][gridZ], points[gridX+1][gridZ+1], points[gridX][gridZ]);			
		}

		
	}

	
	private:

	RawModel alterVertices(Editor *pointer, Loader loader, int type) {
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
					heightMap[i][j] += (type)*(0.10f * intensity);
				}

			}
		}
		return updateVertices(loader);
	

	}

	void setSphere(SphereObject *sphereObject) {
		float height;
		glm::vec3 normal = getHeight(sphereObject->displacement.x, sphereObject->displacement.z, &height);
		glm::vec3 sphereNormal = glm::normalize(sphereObject->displacement);

		float dotProduct = glm::dot(normal, sphereNormal);
		dotProduct *= (180/3.14);
		sphereObject->rotation = dotProduct;
		//TODO: make sure that ball is always above terrain mesh	

                sphereObject->displacement.y = height+1;

	}
	
	
	RawModel updateVertices(Loader loader) {
		float vertices[VERTEX_COUNT*VERTEX_COUNT*SQUARE_POINTS];
		float normals[VERTEX_COUNT*VERTEX_COUNT*SQUARE_POINTS];
		mapVertices(vertices, normals);

		return loader.loadToVAO(vertices, normals, sizeof(vertices) - ((VERTEX_COUNT*sizeof(float)*SQUARE_POINTS)*2) + (sizeof(float)*SQUARE_POINTS));
	}
			

	void printPoints() {
		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				glm::vec3 point = points[i][j];
				std::cout << "["<< point.x << ", " << point.y << ", " << point.z << "]\n";
			}
		}
	}

	// creates the beginning layout for the terrain raw model
	void setPoints() {
		float posX, posY, posZ;
		for (int i = 0; i < VERTEX_COUNT; i++) {
                        for (int j = 0; j < VERTEX_COUNT; j++) {
                                posX = (float)j/((float) VERTEX_COUNT - 1) * SIZE;
                                posY = 0;
                                posZ = (float)i/((float) VERTEX_COUNT - 1) * SIZE;
                                points[j][i] = glm::vec3(posX, posY, posZ);
                                heightMap[j][i] = posY;
                        }

                }

	}
	
	void mapVertices(float vertices[], float normals[]) {
		int vertex = 0;
		int normalsVertex = 0;
		for (int i = 0; i < VERTEX_COUNT; i++) {
			for (int j = 0; j < VERTEX_COUNT; j++) {
				if ((j != VERTEX_COUNT-1 && i != VERTEX_COUNT-1) || (j == VERTEX_COUNT-1 && i == VERTEX_COUNT-1)) {
			
					glm::vec3 inputPoints[] = {points[i][j], points[i][j+1], points[i+1][j], points[i+1][j+1]};
					math.mapSquare(vertices, normals, &vertex, &normalsVertex, inputPoints, glm::vec3(1.0f, 1.0f, 1.0f));	
					
				}

			}

		}
	}


};



#endif
