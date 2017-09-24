#include <stdio.h>
#include <iostream>
#include <vector>
#include <cstring> // For memcopy depending on the platform.


#include "Renderer.h"


Renderer::~Renderer(){}

Renderer::Renderer(int width, int height){

	// Initialize the timer.
	_timer = glfwGetTime();
	// Initialize random generator;
	Random::seed();
	// Setup projection matrix.
	_camera.screen(width, height);
	_camera.internalResolution(height);
	
	// Sizes.
	const int rWidth = (int)_camera.renderSize()[0];
	const int rHeight = (int)_camera.renderSize()[1];
	const int rHalfWidth = (int)(0.5f * rWidth);
	const int rHalfHeight = (int)(0.5f * rHeight);
	// Find the closest power of 2 size.
	const int renderPow2Size = std::pow(2,(int)floor(log2(_camera.renderSize()[0])));
	
	_sceneFramebuffer = std::make_shared<Framebuffer>(rWidth, rHeight, GL_RGBA, GL_UNSIGNED_BYTE, GL_RGBA8, GL_LINEAR,GL_CLAMP_TO_EDGE, false);
	// Background can be rendered at half-resolution with no alpha.
	_backgroundFramebuffer = std::make_shared<Framebuffer>(rHalfWidth, rHalfHeight, GL_RGB, GL_UNSIGNED_BYTE, GL_RGB8, GL_LINEAR,GL_CLAMP_TO_EDGE, false);
	// Once postprocessing is reached, no need for alpha anymore.
	_postprocessFramebuffer = std::make_shared<Framebuffer>(rWidth, rHeight, GL_RGB, GL_UNSIGNED_BYTE, GL_RGB8, GL_LINEAR,GL_CLAMP_TO_EDGE, false);
	_blurBuffer = std::make_shared<Blur>(renderPow2Size, renderPow2Size, 3);
	
	// Query the renderer identifier, and the supported OpenGL version.
//	const GLubyte* renderer = glGetString(GL_RENDERER);
//	const GLubyte* version = glGetString(GL_VERSION);
//	std::cout << "Renderer: " << renderer << std::endl;
//	std::cout << "OpenGL version supported: " << version << std::endl;
//	checkGLError();

	// GL options.
	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glBlendEquation (GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	checkGLError();

	// Init the scene and the full-screen quads.
	_scene.init();
	_postprocessScreen.init(_sceneFramebuffer->textureId(), "postprocess");
	_finalScreen.init(_postprocessFramebuffer->textureId(), "final");
	checkGLError();
	
}


void Renderer::draw() {
	
	// Compute the time elapsed since last frame
	double elapsed = glfwGetTime() - _timer;
	_timer = glfwGetTime();
	
	// Physics simulation
	physics(elapsed);
	
	
	// --- Background bacteria --------
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	_backgroundFramebuffer->bind();
	
	glViewport(0,0,_backgroundFramebuffer->width(),_backgroundFramebuffer->height());
	
	// Clear with light gray.
	glClearColor(0.92f, 0.94f, 0.98f,0.0f);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	// Draw background organisms.
	for(auto & object : _scene.objectsBack){
		object.draw(_camera.view(), _camera.projection());
	}
	
	_backgroundFramebuffer->unbind();
	glDisable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	
	
	// --- Background blurring --------
	_blurBuffer->process(_backgroundFramebuffer->textureId());

	
	// --- Scene pass -----------------
	_sceneFramebuffer->bind();
	glViewport(0,0,_sceneFramebuffer->width(), _sceneFramebuffer->height());
	 // No need to clear the color buffer, the background will cover it all.
	glClear(GL_DEPTH_BUFFER_BIT);
	
	// Draw the blurred background.
	_blurBuffer->draw();
	
	// Draw foreground bacteria.
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	for(auto & object : _scene.objects){
		object.draw(_camera.view(), _camera.projection());
	}
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);
	
	_sceneFramebuffer->unbind();
	
	
	// --- Postprocess pass -----------
	_postprocessFramebuffer->bind();
	glViewport(0,0,_postprocessFramebuffer->width(), _postprocessFramebuffer->height());
	// Setup global parameters.
	glUseProgram(_postprocessScreen.program()->id());
	glUniform3fv(_postprocessScreen.program()->uniform("globalColor"), 1, &(_scene.backgroundColor[0]));
	glUniform1i(_postprocessScreen.program()->uniform("invert"), _scene.invertColors);
	// Draw the result of the scene pass.
	_postprocessScreen.draw();
	_postprocessFramebuffer->unbind();
	
	
	// --- Final pass (AA) ------------
	glViewport(0, 0, GLsizei(_camera.screenSize()[0]), GLsizei(_camera.screenSize()[1]));
	_finalScreen.draw( 1.0f / _camera.renderSize() );
	
	// Update timer
	_timer = glfwGetTime();
	
}

void Renderer::physics(double elapsedTime){
	_camera.update(elapsedTime);
	_scene.update(_timer, elapsedTime);
}


void Renderer::clean() const {
	// Clean objects.
	_scene.clean();
	_finalScreen.clean();
	_postprocessScreen.clean();
	_blurBuffer->clean();
	_sceneFramebuffer->clean();
	_backgroundFramebuffer->clean();
	_postprocessFramebuffer->clean();
}


void Renderer::resize(int width, int height, float scaleFactor){
	//Update the size of the viewport.
	glViewport(0, 0, width, height);
	// Update the projection matrix.
	_camera.screen(width, height);
	_camera.internalResolution(height/scaleFactor);
	// Resize the framebuffers.
	_sceneFramebuffer->resize(_camera.renderSize());
	_backgroundFramebuffer->resize(_camera.renderSize()/2.0f);
	_postprocessFramebuffer->resize(_camera.renderSize());
}

void Renderer::keyPressed(int key, int action){
	if(action == GLFW_PRESS){
		_camera.key(key, true);
	} else if(action == GLFW_RELEASE) {
		if (key == GLFW_KEY_P) {
			// Reload shaders.
			Resources::manager().reload();
			return;
		} else if(key == GLFW_KEY_M){
			// Resample the scene.
			_scene.init();
		}
		_camera.key(key, false);
	}
}



