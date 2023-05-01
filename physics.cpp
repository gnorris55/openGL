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
#include "platform.h"
#include "shader.h"
#include "raw_model.h"

// global variables
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
static long lastFrameTime;
static float delta;
glm::vec3 movement = glm::vec3(0.0f, 0.0f,0.0f);

// declaring functions
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window, unsigned int program);
void checkCompileErrors(unsigned int shader, std::string type);
int checkSphereCollision(Sphere sphere1, Sphere sphere2);
void sphereCollisionResponse(Sphere *s1, Sphere *s2);
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
		
		
		Shader shaderProgram = Shader("vertex_shader.vs", "fragment_shader.fs");

		//TODO: create model loader, assign VAOs for scene such as sphere, and cube and store VAOs in class
		Loader loader;
		Renderer renderer;
		RawModel cube = loader.loadToVAO(rectangleVertices, sizeof(rectangleVertices));
		RawModel sphereModel = loader.loadToVAO(sphereVertices, sphereVerticesNumber*3*sizeof(float));
		Sphere sphere1 = Sphere(&shaderProgram, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), 2, window, &sphereModel); 
		Sphere sphere2 = Sphere(&shaderProgram, glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f), 1, window, &sphereModel); 
	
		while (!glfwWindowShouldClose(window)) {
			// input
			processInput(window, shaderProgram.ID);
			renderer.prepare();
			glUseProgram(shaderProgram.ID);


			// rendering the camera	
			const float radius = 10.0f;
			float camX = sin(glfwGetTime()) * radius;
			float camZ = cos(glfwGetTime()) * radius;
			glm::mat4 view = glm::mat4(1.0f);
			glm::mat4 model = glm::mat4(1.0f);
			glm::mat4 projection = glm::mat4(1.0f);

			//view = glm::lookAt(glm::vec3(camX, 0.0, camZ), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
			//view = glm::rotate(view, glm::radians(camX*25.0f), glm::vec3(1.0f, 1.0f, 1.0f));
			view = glm::translate(view, glm::vec3(0.0f, 0.0f, -20.0f));
			projection = glm::perspective(glm::radians(45.0f), (800.0f/600.0f), 0.1f, 100.0f);

			unsigned int viewLoc = glGetUniformLocation(shaderProgram.ID, "view");
			unsigned int projectionLoc = glGetUniformLocation(shaderProgram.ID, "projection");
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
			glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

			
			renderer.render(cube);
			sphere1.generate();	
			renderer.render(*sphere1.model);	
			
			sphere2.generate();	
			renderer.render(*sphere2.model);	

			//if (checkSphereCollision(sphere1, sphere2) == 1)
				//sphereCollisionResponse(&sphere1, &sphere2);

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

int checkSphereCollision(Sphere sphere1, Sphere sphere2) {
	// calculate distance from center of sphere 1 to distance of centre of sphere2
	float xDistance = sphere1.displacement.x - sphere2.displacement.x;
	float yDistance = sphere1.displacement.y - sphere2.displacement.y;
	float zDistance = sphere1.displacement.z - sphere2.displacement.z;

	float distance = sqrt(pow(xDistance, 2) + pow(yDistance, 2) + pow(zDistance, 2));
	std::cout << distance << "\n";
	if (distance <= sphere1.radius + sphere2.radius)
		return 1;
	
	return 0;
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
                //std::cout << stackAngle << "\n";

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

