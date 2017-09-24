#ifndef Keyboard_h
#define Keyboard_h

#include <glm/glm.hpp>

class Keyboard {

public:

	Keyboard(glm::vec3 & eye, glm::vec3 & center, glm::vec3 & up, glm::vec3 & right);

	~Keyboard();

	/// Reset the keyboard camera parameters.
	void reset();

	/// Update the values of the 4 view-frame vectors.
	void update(double elapsedTime);

	/// Register a pressed or release movement key.
	void registerMove(int direction, bool flag);



private:
	glm::vec3 & _eye;
	glm::vec3 & _center;
	glm::vec3 & _up;
	glm::vec3 & _right;

	bool _keys[7];

	float _speed;
	float _angularSpeed;
	
	glm::vec2 _previousPosition;
	glm::vec2 _deltaPosition;

};

#endif
