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
#include "light.h"
#include "game_object.h"
#include "camera.h"

// global variables
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// declaring functions
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
RawModel createSphere(float vertices[], float normals[], int stackCount, int sectorCount, Loader loader);
glm::vec3 calculateNormals(glm::vec3 vectorA, glm::vec3 vectorB, glm::vec3 vectorC);
void processInput(GLFWwindow *window, unsigned int program);
void checkCompileErrors(unsigned int shader, std::string type);
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
		
		unsigned int stackCount = 16;
		unsigned int sectorCount = 16;
		float sphereVertices[(stackCount*sectorCount)*18];
		float sphereNormals[(stackCount*sectorCount)*18];

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
		RawModel sphere = createSphere(sphereVertices, sphereNormals, stackCount, sectorCount, loader);
		
		SphereObject sphereObject = SphereObject(0.43f, &programShader, window, renderer, glm::vec3(5.0f, 10.0f, 35.0f), glm::vec3(1.0, 1.0, 1.0), &sphere);
		Terrain<128> terrain = Terrain<128>(&programShader, renderer, 0, 0, glm::vec3(0.0f, 0.80f, 0.48f));
		RawModel terrainModel = terrain.generateTerrain(loader);
		Editor pointer = Editor(&programShader, glm::vec3(0.0f,5.0f,0.0f), 1, window, &cube);

		// creating camera and light(s)
		Light lightCube = Light(&lightingShader, renderer, glm::vec3(-2.0f, 40.0f, 0.0f), glm::vec3(0.9f, 0.7f, 0.7f), &cube, window);
		Camera camera = Camera(glm::vec3(-15.0f, -5.0f, -60.0f), 5.0f, 45.0f);
		glEnable(GL_DEPTH_TEST);
		//glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		
		
		float terrainHeight;
		float angle = 45.0f;
		float velocity = 20.0f;
		sphereObject.collision=true;
		while (!glfwWindowShouldClose(window)) {
			
			//preparing frame
			renderer.prepare();
			
			// input
			processInput(window, programShader.ID);

			//rendering light source
			glUseProgram(lightingShader.ID);
			camera.define(lightingShader.ID);	
			lightCube.render();
				
			// rendering gameObjects and terrain	
			glUseProgram(programShader.ID);
			camera.define(programShader.ID);

			int type = pointer.controls();
			pointer.generate();
			renderer.render(cube, GL_LINES);

			//TODO: implement in physics manager ---
			if (type == 1) {
				terrainModel = terrain.raiseVertices(&pointer, loader, 1);
			} else if (type == -1) {
				sphereObject.collision = false;
				sphereObject.setTime();
			}
			if (sphereObject.collision == false) {
				sphereObject.projectileMotion(angle, velocity);
				physicsManager.sphereTerrainCollisionDetection(&sphereObject, 
						       terrain.getHeight(sphereObject.displacement.x, sphereObject.displacement.z, &terrainHeight), 
						       terrainHeight, &angle, &velocity);

			//----
			}

			sphereObject.render(lightCube.displacement, lightCube.color, camera.displacement);
			terrain.render(&terrainModel, lightCube.displacement, lightCube.color, camera.displacement);	
			
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
RawModel createSphere(float vertices[], float normals[], int stackCount, int sectorCount, Loader loader) {
        int vertex = 0;
	int normalsVertex = 0;
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
                for ( int j = 0; j < sectorCount; ++j) {
                        vertexToElement(vertices, &vertex, points[i][j]);
                        vertexToElement(vertices, &vertex, points[i+1][j]);
                        vertexToElement(vertices, &vertex, points[i][j+1]);

			glm::vec3 normalVec = calculateNormals(points[i][j], points[i][j+1], points[i+1][j]);
                        glm::vec3 scalar = glm::vec3(-1.0f, -1.0f, -1.0f);
                        normalVec *= scalar;
                        vertexToElement(normals, &normalsVertex, normalVec);
                        vertexToElement(normals, &normalsVertex, normalVec);
                        vertexToElement(normals, &normalsVertex, normalVec);

                        vertexToElement(vertices, &vertex, points[i][j+1]);
                        vertexToElement(vertices, &vertex, points[i+1][j]);
                        vertexToElement(vertices, &vertex, points[i+1][j+1]);

			normalVec = calculateNormals(points[i][j+1], points[i+1][j+1], points[i+1][j]);
                        normalVec *= scalar;
                        vertexToElement(normals, &normalsVertex, normalVec);
                        vertexToElement(normals, &normalsVertex, normalVec);
                        vertexToElement(normals, &normalsVertex, normalVec);

                }
        }

        return loader.loadToVAO(vertices, normals, normalsVertex*sizeof(float));
}

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

