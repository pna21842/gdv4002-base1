#include "Player.h"
#include "Keys.h"
#include "Engine.h"
#include "Bullet.h"
#include <complex>
#include <bitset>


extern std::bitset<5> keys;
extern glm::vec2 gravity;

Player::Player(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID, float mass) : GameObject2D(initPosition, initOrientation, initSize, initTextureID) {

	this->mass = mass;
	velocity = glm::vec2(0.0f, 0.0f); // default to 0 velocity
}

float emitTimeInterval = 0.15f;
float emitCounter = 0.15f;

void Player::update(double tDelta) {

	glm::vec2 F = glm::vec2(0.0f, 0.0f);

	const float thrust = 3.5f;
	const float dragCoefficient = 1.75f; // tweak to taste
	const float playerRotationSpeed = glm::radians(175.0f);

	std::complex<float> i = std::complex<float>(0.0f, 1.0f);
		auto c = exp(i * orientation);

	emitCounter += (float)tDelta;

	// 1. accumulate forces
	if (keys.test(Key::W) == true) {

		F += glm::vec2(c.real(), c.imag()) * thrust;
	}
	if (keys.test(Key::S) == true) {

		F += glm::vec2(-c.real(), -c.imag()) * thrust;
	}
	if (keys.test(Key::A) == true) {

		orientation += playerRotationSpeed * (float)tDelta;
	}
	if (keys.test(Key::D) == true) {

		orientation += -playerRotationSpeed * (float)tDelta;
	}
	if (keys.test(Key::SPACE) == true) {

		if (emitCounter >= emitTimeInterval)
		{	
			emitCounter = 0;

			GLuint bulletTexture = loadTexture("Resources\\Textures\\myBullet2.png");
			Bullet* shot = new Bullet(position, orientation, glm::vec2(0.1f, 0.1f), bulletTexture);
			addObject("bullet", shot);
		}
	}
	else {
		emitCounter = 0.25;
	}

	glm::vec2 drag = -velocity * dragCoefficient;
	F += drag;

	//F += gravity;


	// add impulse force

	//down - inverse teleport
	if (position.y < -getViewplaneHeight() / 2.0f) {

		position.y = getViewplaneHeight() / 2.0;
	}

	//up - inverse teleport
	if (position.y > getViewplaneHeight() / 2.0f) {		

		position.y = -getViewplaneHeight() / 2.0;
		}
		
	//left - bounce
	if (position.x < -getViewplaneWidth() / 2.0f) {

		position.x = getViewplaneHeight() / 2.0;
	}

	//right - inverse teleport
	if (position.x > getViewplaneWidth() / 2.0f) {

		position.x = -getViewplaneHeight() / 2.0;
	}


	// 2. calculate acceleration.  If f=ma, a = f/m
	glm::vec2 a = F * (1.0f / mass);
	

	// 3. integate to get new velocity
	velocity = velocity + (a * (float)tDelta);
	
	

	// 4. integrate to get new position
	position = position + (velocity * (float)tDelta);
	




}
