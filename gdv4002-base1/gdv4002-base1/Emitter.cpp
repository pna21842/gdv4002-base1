#include "Emitter.h"
#include "Engine.h"
#include "Asteroid.h"


using namespace std;



Emitter::Emitter(glm::vec2 initPosition, glm::vec2 initSize, float emitTimeInterval) : GameObject2D(initPosition, 0.0f, initSize, 0) 
{

	this->emitTimeInterval = emitTimeInterval;
	emitCounter = emitTimeInterval;

	this->asteroidThrust = asteroidThrust;

	particleNumber = 0;


	// Obtain a seed for the random number engine
	random_device rd;

	// Standard mersenne_twister_engine seeded with rd() - mt19937 is a high-quality pseudo-random number generator
	gen = mt19937(rd());
	normDist = uniform_real_distribution<float>(-1.0f, 1.0f);
	massDist = uniform_real_distribution<float>(0.05f, 0.1f);
	scaleDist = uniform_real_distribution<float>(0.25f, 0.8f);
	thrustDistY = uniform_real_distribution<float>(-0.025f, 0.025f);
	thrustDistX = uniform_real_distribution<float>(-0.025f, 0.025f);

}

// override render but do nothing - we'll not render anything for the emitter 
void Emitter::render() 
{
}



void Emitter::update(double tDelta) 
{

	emitCounter += (float)tDelta;

	while (emitCounter >= emitTimeInterval) 
	{

		// decrease emitCounter by emitTimeInterval - don't set to 0 as this would ignore the case where multiple particles are needed.
		emitCounter -= emitTimeInterval;

		// Create new particle
		float x = position.x + normDist(gen) * size.x;
		float y = position.y + normDist(gen) * size.y;
		float scale = scaleDist(gen);
		float mass = massDist(gen);
		float rotationSpeed = glm::radians(normDist(gen) * 45.0f);
		float thrustY = thrustDistY(gen);
		float thrustX = thrustDistX(gen);
		
		GLuint asteroidTexture = loadTexture("Resources\\Textures\\myAsteroid1.png");

		Asteroid* a1 = new Asteroid(glm::vec2(x, y), 0.0f, glm::vec2(scale, scale), GLuint(asteroidTexture), mass, rotationSpeed, thrustY, thrustX);

		string key = string("Asteroid");

		if (particleNumber > 0) { // first name in collection must not be numbered if using this approach

			// add value so unique anyway - not using engine mechanism
			key += to_string(particleNumber);
		}

		particleNumber++;

		addObject(key.c_str(), a1);

	}
}



