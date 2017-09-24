#ifndef Bacteria_h
#define Bacteria_h
#include <gl3w/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "helpers/ResourcesManager.h"



class Bacterium {

public:

	
	Bacterium(const int capId, const glm::vec3 & pos, const glm::vec3 & sca, const glm::vec3 & col, const float time);

	~Bacterium();

	/// Update function
	void update(const float delta){ _time += delta; };
	
	/// Draw function
	void draw(const glm::mat4& view, const glm::mat4& projection) const;
	
	/// Clean function
	void clean() const;

	/// Getters/setters
	glm::vec3 position(){ return _position; };
	void position(const glm::vec3 & pos){ _position = pos; };
	
	glm::vec3 direction(){ return _direction; };
	void direction(const glm::vec3 & dir){ _direction = dir; };
	
	glm::vec3 scale(){ return _scale; };
	void scale(const glm::vec3 & sca){  _scale = sca; };
	

private:
	
	std::shared_ptr<ProgramInfos> _program;
	MeshInfos _mesh;
	
	std::vector<TextureInfos> _textures;
	
	glm::vec3 _position;
	glm::vec3 _scale;
	glm::vec3 _color;
	glm::vec3 _direction;
	float _time;
	int _id;

};

#endif
