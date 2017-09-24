#include <GLFW/glfw3.h>
#include <stdio.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>

#include "Camera.h"


Camera::Camera() : _keyboard(_eye, _center, _up, _right) {
	_verticalResolution = 720;
	
	reset();
}

Camera::~Camera(){}

void Camera::reset(){
	_eye = glm::vec3(0.0,0.0,1.0);
	_center = glm::vec3(0.0,0.0,0.0);
	_up = glm::vec3(0.0,1.0,0.0);
	_right = glm::vec3(1.0,0.0,0.0);
	_view = glm::lookAt(_eye, _center, _up);
	_keyboard.reset();
}

void Camera::update(double elapsedTime){

	_keyboard.update(elapsedTime);

	// Update the view matrix.
	_view = glm::lookAt(_eye, _center, _up);
}


void Camera::key(int key, bool flag){
	
	if (key == GLFW_KEY_W || key == GLFW_KEY_A
		|| key == GLFW_KEY_S || key == GLFW_KEY_D) {
		_keyboard.registerMove(key, flag);
	} else if(flag && key == GLFW_KEY_R) {
		reset();
	} 
}


void Camera::screen(int width, int height){
	_screenSize[0] = float(width > 0 ? width : 1);
	_screenSize[1] = float(height > 0 ? height : 1);
	// Same aspect ratio as the display resolution
	_renderSize = (float(_verticalResolution)/_screenSize[1]) * _screenSize;
	// Perspective projection.
	_projection =  glm::ortho(-1.0f*_renderSize[0] / _renderSize[1], 1.0f*_renderSize[0] / _renderSize[1], -1.0f, 1.0f, 0.01f, 10.0f);
}

void Camera::internalResolution(int height){
	// No need to update the screen size.
	_verticalResolution = height;
	// Same aspect ratio as the display resolution
	_renderSize = (float(_verticalResolution)/_screenSize[1]) * _screenSize;
	// Perspective projection.
	_projection =  glm::ortho(-_renderSize[0] / _renderSize[1], _renderSize[0] / _renderSize[1], -1.0f, 1.0f, 0.01f, 10.0f);
}




