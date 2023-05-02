#ifndef RAW_MODEL_H
#define RAW_MODEL_H

#include "shader.h"

class RawModel {

	public: 
		
	unsigned int vaoID;
	unsigned int vertexCount;

	RawModel(int vaoID, int vertexCount) {
		this->vaoID = vaoID;
		this->vertexCount = vertexCount;
	}


};

class Loader {
	
	public:
	
	RawModel loadToVAO(float vertices[], int numVertices) {
		unsigned int VAO = createVAO();	
		storeDataInAttributeList(0, numVertices, vertices);
		return RawModel(VAO, numVertices/3*sizeof(float)); 

	}

	private:

	int createVAO() {
		unsigned int VAO;
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);
		return VAO;
	}

	void storeDataInAttributeList(int attributeNumber, int numVertices, float vertices[]) {
		unsigned int VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, numVertices, vertices, GL_STATIC_DRAW);
                glVertexAttribPointer(attributeNumber, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
                glEnableVertexAttribArray(0);
	}
};


class Renderer {
	
	public:
	
	void prepare() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
             	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
                glClear(GL_COLOR_BUFFER_BIT);
	}

	void render(RawModel model, GLenum type) {
		glBindVertexArray(model.vaoID);
		glDrawArrays(type, 0, model.vertexCount);
		//glDrawArrays(type, 0, 9*6*3);
		glBindVertexArray(model.vaoID);
	}
};

#endif

