#pragma once

// Model Asteroids particle

#include "GameObject2D.h"

class Asteroid : public GameObject2D {

private:

	// We'll model physics properties for (linear) movement - that is movement without rotation
	float		mass;
	glm::vec2	velocity;
	float thrustY;
	float thrustX;
	
	


	// ... but we also want the Asteroid to rotate as they fall, so we'll change the orientation attribute but without the physics.  We already inherit the orientation attribute from GameObject2D, so we just need to add the 'change in orientation per second' here

	float		angleChangePerSecond;


public:

	Asteroid(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID, float mass, float angleChangePerSecond, float thrustY, float thrustX);

	void update(double tDelta) override;

	

};


