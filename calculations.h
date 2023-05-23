#ifndef CALCULATIONS_H
#define CALCULATIONS_H

class Math {

	public:	

	void mapSquare(float vertices[], float normals[], int *vertex, int *normalsVertex, glm::vec3 points[], glm::vec3 normalDir) {
		//first triangle
		vertexToElement(vertices, vertex, points[0]);
		vertexToElement(vertices, vertex, points[1]);
		vertexToElement(vertices, vertex, points[2]);

		glm::vec3 normalVec = calculateNormals(points[0], points[1], points[2]);
		normalVec *= normalDir;
		vertexToElement(normals, normalsVertex, normalVec);
		vertexToElement(normals, normalsVertex, normalVec);
		vertexToElement(normals, normalsVertex, normalVec);


		//second triangle
		vertexToElement(vertices, vertex, points[1]);
		vertexToElement(vertices, vertex, points[3]);
		vertexToElement(vertices, vertex, points[2]);

		normalVec = calculateNormals(points[1], points[3], points[2]);
		normalVec *= normalDir;
		vertexToElement(normals, normalsVertex, normalVec);
		vertexToElement(normals, normalsVertex, normalVec);
		vertexToElement(normals, normalsVertex, normalVec);
	}

	void vertexToElement(float vertices[], int *vertex,  glm::vec3 vector) {
                vertices[*vertex] = vector.x;
                vertices[*vertex+1] = vector.y;
                vertices[*vertex+2] = vector.z;
                *vertex += 3;
        }

        glm::vec3 calculateNormals(glm::vec3 vectorA, glm::vec3 vectorB, glm::vec3 vectorC) {
                //(B-A)x(C-A)
                //
                return glm::cross(vectorB - vectorA, vectorC - vectorA);

        }

        float barryCentric(glm::vec3 p1, glm::vec3 p2, glm::vec3 p3, glm::vec2 pos) {
                float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
                float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
                float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
                float l3 = 1.0f - l1 - l2;
                return l1 * p1.y + l2 * p2.y + l3 * p3.y;
        }


};

#endif
