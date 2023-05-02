#ifndef PHYSICS_MANAGER_H
#define PHYSICS_MANAGER_H

#include "sphere.h"
#include "rectangle.h"


class PhysicsManager {

	public:

	int checkSphereCollision(Sphere sphere1, Sphere sphere2) {
		// calculate distance from center of sphere 1 to distance of centre of sphere2
		float xDistance = sphere1.displacement.x - sphere2.displacement.x;
		float yDistance = sphere1.displacement.y - sphere2.displacement.y;
		float zDistance = sphere1.displacement.z - sphere2.displacement.z;

		float distance = sqrt(pow(xDistance, 2) + pow(yDistance, 2) + pow(zDistance, 2));
		//std::cout << distance << "\n";
		if (distance <= sphere1.radius + sphere2.radius)
			return 1;

		return 0;
	}

	int checkSphereOnRectangleCollision(Sphere sphere, Rectangle rectangle) {
		float xDistance = abs(sphere.displacement.x - rectangle.displacement.x);
		float yDistance = abs(sphere.displacement.y - rectangle.displacement.y);
		float zDistance = abs(sphere.displacement.z - rectangle.displacement.z);

		std::cout << xDistance << "\n";
		std::cout << yDistance << "\n";
		std::cout << zDistance << "\n";
		
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

	void sphereCollisionResponse(Sphere *s1, Sphere *s2) {
        	std::cout << "collision detected\n";
	}

};


#endif
