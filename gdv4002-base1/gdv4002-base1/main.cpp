#include "Engine.h"
#include "Keys.h"
#include "Player.h"
#include "Emitter.h"
#include <bitset>
#include <complex>
using namespace std;

// Function prototypes
void myUpdate(GLFWwindow* window, double tDelta);
void myRender(GLFWwindow* window);
void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);
void deleteBullet(GLFWwindow* window, double tDelta);

std::bitset<5> keys{ 0x0 };



void deleteBullet(GLFWwindow* window, double tDelta) {

	GameObjectCollection Bullet = getObjectCollection("Bullet");

	for (int i = 0; i < Bullet.objectCount; i++) {

		if (Bullet.objectArray[i]->position.y < -(getViewplaneHeight() / 2.0f)) {

			deleteObject(Bullet.objectArray[i]);
		}
		if (Bullet.objectArray[i]->position.x < -(getViewplaneHeight() / 2.0f)) {

			deleteObject(Bullet.objectArray[i]);
		}
		if (Bullet.objectArray[i]->position.y < (getViewplaneHeight() / 2.0f)) {

			deleteObject(Bullet.objectArray[i]);
		}
		if (Bullet.objectArray[i]->position.x < (getViewplaneHeight() / 2.0f)) {

			deleteObject(Bullet.objectArray[i]);
		}
	}
}



int main(void) {

	// Initialise the engine (create window, setup OpenGL backend)
	int initResult = engineInit("GDV4002 - Applied Maths for Games", 1024, 1024, 5.0f);

	// If the engine initialisation failed report error and exit
	if (initResult != 0) {

		printf("Cannot setup game window!!!\n");
		return initResult; // exit if setup failed

	}


	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_ALWAYS);

	
	//
	// Setup game scene objects here
	//


	GLuint backgroundTexture = loadTexture("Resources\\Textures\\MyBackground3.png");
	GameObject2D* Background = new GameObject2D(glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(5.0f, 5.0f), GLuint(backgroundTexture));
	addObject("Background", Background);

	GLuint playerTexture = loadTexture("Resources\\Textures\\MyShip2.png");
	Player* mainPlayer = new Player(glm::vec2(0.0f, 0.0f), glm::radians(90.0f), glm::vec2(0.625f, 0.635f), playerTexture, 1.50f);
	addObject("player", mainPlayer);


	Emitter* emitterTop = new Emitter(
		glm::vec2(0.0f, getViewplaneHeight() / 2.0f * 1.1f),
		glm::vec2(getViewplaneWidth() / 2.0f, 0.0f),
		0.625f);
	addObject("emitter", emitterTop);

	Emitter* emitterLeft = new Emitter(
		glm::vec2(-getViewplaneWidth() / 2.0f * 1.1, 0.0f),
		glm::vec2(0.0f, getViewplaneHeight() / 2.0f),
		0.625f);
	addObject("emitter", emitterLeft);

	Emitter* emitterRight = new Emitter(
		glm::vec2(getViewplaneWidth() / 2.0f * 1.1, 0.0f),
		glm::vec2(0.0f, getViewplaneHeight() / 2.0f),
		0.625f);
	addObject("emitter", emitterRight);

	Emitter* emitterBottom = new Emitter(
		glm::vec2(0.0f, -getViewplaneHeight() / 2.0f * 1.1f),
		glm::vec2(getViewplaneWidth() / 2.0f, 0.0f),
		0.625f);
	addObject("emitter", emitterBottom);


	setKeyboardHandler(myKeyboardHandler);

	listGameObjectKeys();
	listObjectCounts();

	setRenderFunction(myRender);


	// Enter main loop - this handles update and render calls
	engineMainLoop();

	// When we quit (close window for example), clean up engine resources
	engineShutdown();

	// return success :)
	return 0;
}


void myUpdate(GLFWwindow* window, double tDelta) {

}


void myRender(GLFWwindow* window) {

	GameObject2D* background = getObject("Background");
	if (background != nullptr)
		background->render();

	GameObject2D* Player = getObject("player");
	if (Player != nullptr)
		Player->render();

	GameObjectCollection asteroids = getObjectCollection("Asteroid");
	for (int i = 0; i < asteroids.objectCount; i++) {
		if (asteroids.objectArray[i] != nullptr)
			asteroids.objectArray[i]->render();
	}

	GameObjectCollection Bullets = getObjectCollection("bullet");
	for (int i = 0; i < Bullets.objectCount; i++){
		if (Bullets.objectArray[i] != nullptr)
			Bullets.objectArray[i]->render();
	}
}



void myKeyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	// Check if the key was just pressed
	if (action == GLFW_PRESS) {

		// now check which key was pressed...
		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			// If escape is pressed tell GLFW we want to close the window(and quit)
				glfwSetWindowShouldClose(window, true);
			break;
		case GLFW_KEY_W:
			keys[Key::W] = true;
			break;
		case GLFW_KEY_S:
			keys[Key::S] = true;
			break;
		case GLFW_KEY_A:
			keys[Key::A] = true;
			break;
		case GLFW_KEY_D:
			keys[Key::D] = true;
			break;
		case GLFW_KEY_SPACE:
			keys[Key::SPACE] = true;
			break;

		}
	}
	// If not pressed, check the key has just been released
	else if (action == GLFW_RELEASE) {

		switch (key)
		{
		case GLFW_KEY_W:
			keys[Key::W] = false;
			break;
		case GLFW_KEY_S:
			keys[Key::S] = false;
			break;
		case GLFW_KEY_A:
			keys[Key::A] = false;
			break;
		case GLFW_KEY_D:
			keys[Key::D] = false;
			break;
		case GLFW_KEY_SPACE:
			keys[Key::SPACE] = false;
			break;


		}
	}
}




