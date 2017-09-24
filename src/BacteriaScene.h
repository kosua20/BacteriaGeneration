#ifndef BacteriaScene_h
#define BacteriaScene_h

#include <gl3w/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>

#include "../Bacterium.h"

class BacteriaScene  {
	
public:
	
	~BacteriaScene();
	
	BacteriaScene();
	
	/// Init function
	void init();
	
	void update(double timer, double elapsedTime);
	
	/// Clean function
	void clean() const;
	
	std::vector<Bacterium> objects;
	std::vector<Bacterium> objectsBack;
	glm::vec3 backgroundColor;
	bool invertColors;
	
};

#endif
