#include "Bullet.h"
#include "keys.h"
#include "Player.h"
#include <complex>
#include <bitset>

Bullet::Bullet(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID) : GameObject2D(initPosition, initOrientation, initSize, initTextureID) {


	std::complex<float> i = std::complex<float>(0.0f, 1.0f);
	auto c = exp(i * initOrientation);

	velocity = glm::vec2(c.real() * 4, c.imag() * 4);

}

void Bullet::update(double tDelta) {

	position = position + (velocity * (float)tDelta);

	
	
}
