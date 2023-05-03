#define STB_IMAGE_IMPLEMENTATION
#define PI 3.141592

#include<iomanip>
#include <cmath>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <chrono>
#include <iostream>
#include "sphere.h"
#include "rectangle.h"
#include "shader.h"
#include "raw_model.h"
#include "terrain.h"
#include "physics_manager.h"

// global variables
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// declaring functions
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, unsigned int program);
void checkCompileErrors(unsigned int shader, std::string type);
int createSphere(float vertices[], int stackCount, int sectorCount);
void vertexToElement(float vertices[], int *vertex,  glm::vec3 vector);


class MainGameLoop {

	public:

	int start() {
		
		// initializing window
		// -----------------------------------
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

		#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		#endif

		GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
		if (window == NULL)
		{
			std::cout << "Failed to create GLFW window" << std::endl;
			glfwTerminate();
			return -1;
		}
		glfwMakeContextCurrent(window);
		glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

		// glad: load all OpenGL function pointers
		// ---------------------------------------
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			std::cout << "Failed to initialize GLAD" << std::endl;
			return -1;
		}
		
		Shader shaderProgram = Shader("vertex_shader.vs", "fragment_shader.fs");

		float sphereVertices[100000];
	       	int sphereVerticesNumber = createSphere(sphereVertices, 16, 16);	

		float rectangleVertices[] = {
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
		
		PhysicsManager physicsManager;
		Loader loader;
		Renderer renderer;

		RawModel sphereModel = loader.loadToVAO(sphereVertices, sphereVerticesNumber*3*sizeof(float));
		Editor pointer =  Editor(&shaderProgram, glm::vec3(0.0f, 0.0f, 0.0f), 3, window, &sphereModel);
			
		Terrain terrain = Terrain(&shaderProgram, 0, 0);		
		RawModel terrainModel = terrain.generateTerrain(loader);

		glEnable(GL_DEPTH_TEST);
		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);

		while (!glfwWindowShouldClose(window)) {
			// input
			processInput(window, shaderProgram.ID);
			renderer.prepare();
			glUseProgram(shaderProgram.ID);

			// rendering the camera	
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);

			view = glm::translate(view, glm::vec3(0.0f, -5.0f, -40.0f));
			view = glm::rotate(view, glm::radians(20.0f),glm::vec3(1.0f, 0.0f, 0.0f));
			projection = glm::perspective(glm::radians(45.0f), (800.0f/600.0f), 0.1f, 100.0f);

			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(glGetUniformLocation(shaderProgram.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
			
			//rendering objects
			pointer.generate();
			renderer.render(*pointer.model, GL_LINES);
			int pointerCommand = pointer.controls();
			if (pointerCommand != 0) {
				terrainModel = terrain.raiseVertices(&pointer, loader, pointerCommand);
			}

			terrain.generate();	// rendering terrain
			renderer.render(terrainModel, GL_TRIANGLES);

			// finishing frame
			glfwSwapBuffers(window);
			glfwPollEvents();	
		}
		
		return 0;
	}
};

int main() {
	
	// initializing the program
	MainGameLoop main;
	main.start();

}


void sphereCollisionResponse(Sphere *s1, Sphere *s2) {
	std::cout << "collision detected\n";
}


// specifies the viewport dimensions
// after clipping, primitives are put in viewport coordinates so that they can be rasterized
void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
	glViewport(0, 0, width, height);

}

void processInput(GLFWwindow *window, unsigned int program) {
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}	

// creates vertices for the sphere
int createSphere(float vertices[], int stackCount, int sectorCount) {
        int vertex = 0;

        glm::vec3 points[100][100];
        float x, y, z, xy;
        float sectorStep = 2 * PI / sectorCount;
        float stackStep = PI / stackCount;
        float sectorAngle, stackAngle;

        for (int i = 0; i <= stackCount; ++i) {
                stackAngle = PI / 2 - i * stackStep;

                xy = cos(stackAngle);
                z = sin(stackAngle);

                for (int j = 0; j <= sectorCount; ++j) {

                        sectorAngle = j * sectorStep;
                        x = xy * cos(sectorAngle);
                        y = xy * sin(sectorAngle);
                        points[i][j] = glm::vec3(x, y, z);
                }
        }

        for ( int i = 0; i < stackCount; ++i) {
                //std::cout << "\n";
                for ( int j = 0; j < sectorCount; ++j) {
                        vertexToElement(vertices, &vertex, points[i][j]);
                        vertexToElement(vertices, &vertex, points[i+1][j]);
                        vertexToElement(vertices, &vertex, points[i][j+1]);
                        vertexToElement(vertices, &vertex, points[i][j+1]);
                        vertexToElement(vertices, &vertex, points[i+1][j]);
                        vertexToElement(vertices, &vertex, points[i+1][j+1]);
                }
        }

        return vertex;
}

void vertexToElement(float vertices[], int *vertex,  glm::vec3 vector) {
        vertices[*vertex] = vector.x;
        vertices[*vertex+1] = vector.y;
        vertices[*vertex+2] = vector.z;
        *vertex += 3;
}

