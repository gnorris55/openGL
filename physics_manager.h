#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "rectangle.h"
#include "game_object.h"
#include "terrain.h"
#include <algorithm>


class PhysicsManager {

	public:

	void terrainSphereManager(SphereObject *sphere, Terrain<128> *terrain, float angle, float velocity) {
		// if collision is not detected, move sphere and check for collision
		if (sphere->collision == false) {
			sphere->projectileMotion(angle, velocity);

			//height and normal of the current terrain triangle under sphere
			float terrainHeight = 0;
			glm::vec3 terrainNormal = terrain->getHeight(sphere->displacement.x, sphere->displacement.z, &terrainHeight);

                	if (sphereTerrainCollisionDetection(sphere, terrainNormal, terrainHeight) == 1)
				sphereTerrainCollisionResponse(sphere, terrainNormal, terrainHeight, &angle, &velocity);
		}
	}





	private:
	
	//TODO:
	// SPHERE TERRAIN PHYSICS============================

	int sphereTerrainCollisionDetection(SphereObject *sphere, glm::vec3 triangleNormal, float triangleHeight) {
		
		if ((sphere->displacement.y - triangleHeight) <= (sphere->radius) && (glfwGetTime()-sphere->startTime) > 0.1f) {
			sphere->collision = true;
			return 1;
		}
		return 0;
	}

	void sphereTerrainCollisionResponse(SphereObject *sphere, glm::vec3 normalVector, float triangleHeight, float *angle, float *velocity) {
				
		//TODO: make it work
		glm::vec3 velocityVector = sphere->velocityVector;
		float dir = 1;
		std::cout << normalVector.x << ", " << normalVector.y << ", " << normalVector.y << "\n";
		if (((normalVector.x != 0) && velocityVector.z < 0) || velocityVector.z > 0) {
			std::cout << "alrighty\n";
			dir = -1;
		}
		if (velocityVector.y > 0) {
			normalVector *= glm::vec3(-1.0f, -1.0f, -1.0f);
		}

		if (glm::length(velocityVector) > 1.0f) {

			float dot = glm::dot(glm::normalize(velocityVector), glm::normalize(normalVector));
			if (dot > 1) {dot = 0.99;}
			if (dot < -1) {dot = -0.99f;}
			std::cout << "dot product: " << dot << "\n";
			float angleOfCollision = acos(dot)*180/3.14;
			std::cout << "angle: " << 90-angleOfCollision << "\n";
			//printCollisionStats(velocityVector, sphere->displacement, normalVector, triangleHeight, angleOfCollision);
			sphere->startingPosition += sphere->flightVec;
			sphere->setTime();
			sphere->flightVec = glm::vec3(0.0f, 0.0f, 0.0f);
			*angle = 90 - angleOfCollision;

			if (glm::length(velocityVector) > 18) {
				*velocity = 15.0f*0.72*dir;
			}	else {
				*velocity = glm::length(velocityVector)*0.6*dir;
			}
			std::cout << "output velocity: " << *velocity << " output angle: " << *angle << "\n";
			sphere->collision = false;
		}

	}

	//============================================================================



	int sphereCollisionDetection(SphereObject sphere1, SphereObject sphere2) {
		// calculate distance from center of sphere 1 to distance of centre of sphere2
		float xDistance = sphere1.displacement.x - sphere2.displacement.x;
		float yDistance = sphere1.displacement.y - sphere2.displacement.y;
		float zDistance = sphere1.displacement.z - sphere2.displacement.z;

		float distance = sqrt(pow(xDistance, 2) + pow(yDistance, 2) + pow(zDistance, 2));
		if (distance <= sphere1.radius + sphere2.radius)
			return 1;

		return 0;
	}
	
	int sphereRectangleCollisionDetection(Sphere sphere, Rectangle rectangle) {
		float xDistance = abs(sphere.displacement.x - rectangle.displacement.x);
		float yDistance = abs(sphere.displacement.y - rectangle.displacement.y);
		float zDistance = abs(sphere.displacement.z - rectangle.displacement.z);

		if (xDistance >= ((rectangle.xScale/2) + sphere.radius) ||
		    yDistance >= ((rectangle.yScale/2) + sphere.radius) ||
		    zDistance >= ((rectangle.zScale/2) + sphere.radius))
			return 0;

		if (xDistance < (rectangle.xScale/2) ||
		    yDistance < (rectangle.yScale/2) ||
		    zDistance < (rectangle.zScale/2)) 
		   	return 1;

		float cornerDistance_sq = (pow((xDistance - rectangle.xScale/2), 2) +
					   pow((yDistance - rectangle.yScale/2), 2) +
					   pow((zDistance - rectangle.zScale/2), 2));

		if (cornerDistance_sq < pow(sphere.radius, 2)) {
			std::cout << "corner collision\n";
			return 1;
		} else
			return 0;
	}
	

};


#endif
