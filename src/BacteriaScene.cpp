#include "BacteriaScene.h"

#include <stdio.h>
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <GenerationUtilities.h>

BacteriaScene::BacteriaScene(){}

BacteriaScene::~BacteriaScene(){}


void BacteriaScene::init(){
	// Clear in case we are re-generating an already existing (and thus filled) scene.
	objects.clear();
	objectsBack.clear();
	
	// Foreground organisms: position in the [-1,1] square, a color close to white, and a small size.
	// Also pick a cap mesh among the four existing (see resources/meshes), and an inital time.
	for(int i = 0; i < 100; ++i){
		const glm::vec3 randomPos = glm::vec3(Random::Float(-1.0f, 1.0f),Random::Float(-1.0f, 1.0f), 0.0f);
		const glm::vec3 randomCol = glm::vec3(Random::Float(0.9f,1.0f),Random::Float(0.9f,1.0f),Random::Float(0.9f,1.0f));
		const glm::vec3 randomScale = glm::vec3(Random::Float(0.01f, 0.08f));
		const int randomId = Random::Int(0, 3);
		const float randomInitialTime = Random::Float(0.0f, 10000.0f);
		objects.emplace_back(randomId, randomPos, randomScale, randomCol, randomInitialTime);
	}
	
	// Background organisms: same idea, but fewer, bigger, less colored.
	for(int i = 0; i < 70; ++i){
		const glm::vec3 randomPos = glm::vec3(Random::Float(-1.0f, 1.0f),Random::Float(-1.0f, 1.0f), 0.0f);
		const glm::vec3 randomCol = glm::vec3(Random::Float(0.9f,0.92f),Random::Float(0.9f,0.92f),Random::Float(0.9f,0.92f));
		const glm::vec3 randomScale = glm::vec3(Random::Float(0.05f, 0.17f));
		const int randomId = Random::Int(0, 3);
		const float randomInitialTime = Random::Float(0.0f, 10000.0f);
		objectsBack.emplace_back(randomId, randomPos, randomScale, randomCol, randomInitialTime);
	}
	
	// Random background color and color inversion.
	backgroundColor = Random::Float(0.8f, 1.2f)*glm::vec3(Random::Float(0.95f, 1.05f),Random::Float(0.95f, 1.05f),Random::Float(0.95f, 1.05f));
	invertColors = Random::Float() > 0.75f;
}

void BacteriaScene::update(double timer, double elapsedTime){
	// Update foreground objects: make them advance a random amount along their current direction.
	// From time to time, pick a new direction.
	for(auto & obj : objects){
		// Update time.
		obj.update(elapsedTime);
		// Pick a new random unit direction 20% of the time.
		if(Random::Float() < 0.2f){
			const glm::vec3 randomDir = glm::vec3(Random::Float(-1.0f, 1.0f),Random::Float(-1.0f, 1.0f), 0.0f);
			obj.direction(glm::normalize(randomDir));
		}
		// Update object position by following the current direction a random amount.
		obj.position(obj.position() + Random::Float(0.002f, 0.007f)*obj.direction());
	}
	
	// Update background objects with the same logic, but they move faster but keep their direction longer.
	for(auto & obj : objectsBack){
		// Update time.
		obj.update(elapsedTime);
		// Pick a new random unit direction 5% of the time.
		if(Random::Float() < 0.05f){
			const glm::vec3 randomDir = glm::vec3(Random::Float(-1.0f, 1.0f),Random::Float(-1.0f, 1.0f), 0.0f);
			obj.direction(glm::normalize(randomDir));
		}
		// Update object position by following the current direction a random amount.
		obj.position(obj.position() + Random::Float(0.004f, 0.008f)*obj.direction());
	}
	
}

void BacteriaScene::clean() const {
	for(auto & object : objects){
		object.clean();
	}
	for(auto & object : objectsBack){
		object.clean();
	}
};

