#pragma once

#include "GameObject2D.h"

class Bullet : public GameObject2D {

private:
	glm::vec2 velocity;
	

public:

	Bullet(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID);
	void update(double tDelta) override;






};