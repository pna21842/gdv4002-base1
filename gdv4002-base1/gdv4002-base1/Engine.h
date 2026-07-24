#pragma once

// Engine.h ver 1.4

#include "GraphicsCore.h"

#include "GUMemory.h"
#include "GUObject.h"
#include "GUClock.h"
#include "GameObject2D.h"
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include <iostream>
#include <string>
#include <map>
#include <unordered_map>
#include <bitset>
#include <glm/glm.hpp>
#include "texture_loader.h"

// Define function pointers for render and update functions
typedef void (*RenderFn)(GLFWwindow* window);
typedef void (*UpdateFn)(GLFWwindow* window, double tDelta);
typedef void (*ResizeFn)(GLFWwindow* window, float viewplaneWidth, float viewplaneHeight);


// Define new type to store collections of game objects
struct GameObjectCollection {

	int				objectCount;
	GameObject2D**  objectArray;

	// Default constructor - setup empty collection (0, nullptr)
	GameObjectCollection() {

		objectCount = 0;
		objectArray = nullptr;
	}

	// Constructor with specific size for the array - zero out (null) all game object pointers in the array
	GameObjectCollection(int n) {

		objectCount = n;
		objectArray = (GameObject2D**)calloc(n, sizeof(GameObject2D*));
	}

	// Copy constructor - copy internal contents
	GameObjectCollection(const GameObjectCollection& col) {

		if (col.objectCount == 0) {

			objectCount = 0;
			objectArray = nullptr;
		}
		else {

			objectCount = col.objectCount;
			objectArray = (GameObject2D**)malloc(objectCount * sizeof(GameObject2D*));
			memcpy_s(objectArray, objectCount * sizeof(GameObject2D*), col.objectArray, objectCount * sizeof(GameObject2D*));
		}
	}

	// Destructor to free up resources when collection goes out of scope- need to do this otherwise we get a memory leak :/
	~GameObjectCollection() {

		if (objectArray)
			free(objectArray);
	}
};



//
// Top level engine functionality - setup, main loop and shutdown
//
int engineInit(const char* windowTitle, int initWidth = 1920, int initHeight = 1080, float initViewplaneWidth = 5.0f, bool enableVSync = true);
void engineMainLoop();
void engineShutdown();

//
// Event registration
//
void setKeyboardHandler(GLFWkeyfun newKeyboardHandler);
void setRenderFunction(RenderFn fn); 
void setUpdateFunction(UpdateFn fn, bool overrideUpdate = true); // if overrideUpdate = true, fn completely replaces the update function in the engine (behaviour of previous version).  If false then fn is called *after* the default update.
void setResizeFunction(ResizeFn fn);

//
// Update / Query engine state
//

// Load a new texture image and return the OpenGL texture ID.  Store the texture internally in the engine for later use.
GLuint loadTexture(const char* texturePath, TextureProperties texProperties = TextureProperties());

// Provide properties to add a new game object.  This takes relevant properties and creates a new GameObject2D object in the game scene.
GameObject2D* addObject(
	const char* name,
	glm::vec2 initPosition = glm::vec2(0.0f, 0.0f),
	float initOrientation = 0.0f,
	glm::vec2 initSize = glm::vec2(1.0f, 1.0f),
	const char* texturePath = nullptr,
	TextureProperties texProperties = TextureProperties());

// Add an already existing object to the scene.  This overload of addObject can be used to provide subclasses of GameObject2D since addObject above creates GameObject2D instances directly.
GameObject2D* addObject(const char* name, GameObject2D* newObject);

// getObject returns the (singular) object with the *exact* key match
GameObject2D* getObject(const char* key);
GameObjectCollection getObjectCollection(const char* key);

// Delete the game object with key 'key' and return true if successful, false otherwise.  This matches key *exactly*, it does not do a partial match
bool deleteObject(const char* key);

// Delete the game object pointed to by objectPtr and return true if successful, false otherwise.  It is assumed 1 instance if each objectPtr exists in the object list maintained by the engine.  If this is not the case then the first instance of the pointer only is deleted.
bool deleteObject(GameObject2D* objectPtr);

// Delete any object where the key partially matches 'key'.  Unlike deleteObject, this can be used to remove groups of like-named objects.  The function returns 0 if no objects matched and nothing was deleted, otherwise it returns the number of elements removed.
int deleteMatchingObjects(const char* key);


void showAxisLines();
void hideAxisLines();
bool axisLinesVisible();
void setBackgroundColour(glm::vec4& newColour);
void setViewplaneWidth(float newWidth);
float getViewplaneWidth();
float getViewplaneHeight();

glm::vec4 getBackgroundColour();
void setBackgroundColour(glm::vec4 newColour);

int getObjectCounts(std::string key);

//
// Test functions
//
void listObjectCounts();
void listGameObjectKeys();