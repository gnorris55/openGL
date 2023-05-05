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
#include "terrain.h" //blocked until normals can be added
#include "physics_manager.h"
#include "light.h"
#include "game_object.h"
#include "camera.h"

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
		
		Shader programShader = Shader("vertex_shader.vs", "fragment_shader.fs");
		Shader lightingShader = Shader("lighting_vertex_shader.vs", "lighting_fragment_shader.fs");

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

		float cubeNormals[] {
			 0.0f,  0.0f, -1.0f,
			 0.0f,  0.0f, -1.0f,
			 0.0f,  0.0f, -1.0f,
			 0.0f,  0.0f, -1.0f,
			 0.0f,  0.0f, -1.0f,
			 0.0f,  0.0f, -1.0f,

			 0.0f,  0.0f,  1.0f,
			 0.0f,  0.0f,  1.0f,
			 0.0f,  0.0f,  1.0f,
			 0.0f,  0.0f,  1.0f,
			 0.0f,  0.0f,  1.0f,
			 0.0f,  0.0f,  1.0f,

			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,
			-1.0f,  0.0f,  0.0f,

			 1.0f,  0.0f,  0.0f,
			 1.0f,  0.0f,  0.0f,
			 1.0f,  0.0f,  0.0f,
			 1.0f,  0.0f,  0.0f,
			 1.0f,  0.0f,  0.0f,
			 1.0f,  0.0f,  0.0f,

			 0.0f, -1.0f,  0.0f,
			 0.0f, -1.0f,  0.0f,
			 0.0f, -1.0f,  0.0f,
			 0.0f, -1.0f,  0.0f,
			 0.0f, -1.0f,  0.0f,
			 0.0f, -1.0f,  0.0f,

			 0.0f,  1.0f,  0.0f,
			 0.0f,  1.0f,  0.0f,
			 0.0f,  1.0f,  0.0f,
			 0.0f,  1.0f,  0.0f,
			 0.0f,  1.0f,  0.0f,
			 0.0f,  1.0f,  0.0f
		};
		
		// creating instances of manager classes	
		PhysicsManager physicsManager;
		Loader loader;
		Renderer renderer;


		// creatign game objects and terrain
		RawModel cube = loader.loadToVAO(rectangleVertices, cubeNormals, sizeof(rectangleVertices));
		GameObject cubeObject = GameObject(&programShader, window, renderer, glm::vec3(5.0f, 5.0f, 0.0f), glm::vec3(0.5, 0.5, 0.5), &cube);
		Terrain terrain = Terrain(&programShader, renderer, 0, 0, glm::vec3(0.0f, 0.80f, 0.48f));
		RawModel terrainModel = terrain.generateTerrain(loader);		

		// creating camera and light(s)
		Light lightCube = Light(&lightingShader, renderer, glm::vec3(0.0f, 5.0f, 2.0f), glm::vec3(0.7f, 0.5f, 0.3f), &cube, window);
		Camera camera = Camera(glm::vec3(0.0f, -7.0f, -20.0f), 20.0f);
		
		glEnable(GL_DEPTH_TEST);

		while (!glfwWindowShouldClose(window)) {
			
			//preparing frame
			renderer.prepare();
			
			// input
			processInput(window, programShader.ID);
			lightCube.controls();
			

			//rendering light source
			glUseProgram(lightingShader.ID);
			camera.define(lightingShader.ID);	
			lightCube.render();
		
			// rendering gameObjects and terrain	
			glUseProgram(programShader.ID);
			camera.define(programShader.ID);

			terrain.render(&terrainModel, lightCube.displacement, lightCube.color, camera.displacement);	
			cubeObject.render(lightCube.displacement, lightCube.color, camera.displacement);
			
			// closing frame
			glfwSwapBuffers(window);
			glfwPollEvents();	
		}
		
		return 0;
	}
};

int main() {
	
	MainGameLoop main;
	main.start();

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
// TODO: put this in the sphere class
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

