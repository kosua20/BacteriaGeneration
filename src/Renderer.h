#ifndef Renderer_h
#define Renderer_h
#include <gl3w/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

#include "helpers/GenerationUtilities.h"

#include "BacteriaScene.h"
#include "Framebuffer.h"
#include "Blur.h"
#include "camera/Camera.h"
#include "ScreenQuad.h"

class Renderer {

public:

	~Renderer();

	/// Init function
	Renderer(int width, int height);

	/// Draw function
	void draw();

	void physics(double elapsedTime);

	/// Clean function
	void clean() const;

	/// Handle screen resizing
	void resize(int width, int height, float scaleFactor);

	/// Handle keyboard inputs
	void keyPressed(int key, int action);
	
	
private:
	
	double _timer;

	Camera _camera;

	std::shared_ptr<Blur> _blurBuffer;
	std::shared_ptr<Framebuffer> _sceneFramebuffer;
	std::shared_ptr<Framebuffer> _backgroundFramebuffer;
	std::shared_ptr<Framebuffer> _postprocessFramebuffer;
	
	ScreenQuad _postprocessScreen;
	ScreenQuad _finalScreen;

	BacteriaScene _scene;

};

#endif
