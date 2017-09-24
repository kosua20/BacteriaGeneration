#include <stdio.h>
#include <iostream>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>


#include "Bacterium.h"


Bacterium::~Bacterium() {}

Bacterium::Bacterium(const int capId, const glm::vec3 & pos, const glm::vec3 & sca, const glm::vec3 & col, const float time) {
	
	_program = Resources::manager().getProgram("bacterium");
	_mesh = Resources::manager().getMesh("cap" + std::to_string(capId));
	_position = pos;
	_direction = glm::vec3(0.0f);
	_scale = sca;
	_time = time;
	_color = col;
	
	// Each bacterium has its own unique ID.
	static int id = 0;
	_id = id;
	++id;
	
	checkGLError();
}


void Bacterium::draw(const glm::mat4& view, const glm::mat4& projection) const {

	// Generate model matrix from position and scale.
	glm::mat4 model = glm::scale(glm::translate(glm::mat4(1.0f), _position), _scale);
	
	// Combine the three matrices.
	glm::mat4 MVP = projection * view * model;

	// Select the program (and shaders).
	glUseProgram(_program->id());

	// Upload the uniforms.
	glUniformMatrix4fv(_program->uniform("mvp"), 1, GL_FALSE, &MVP[0][0]);
	glUniform1i(_program->uniform("id"), _id);
	glUniform1f(_program->uniform("time"), _time);
	glUniform3f(_program->uniform("color"), _color[0], _color[1], _color[2]);
	
	// Select the geometry.
	glBindVertexArray(_mesh.vId);
	// Draw!
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _mesh.eId);
	glDrawElements(GL_TRIANGLES, _mesh.count, GL_UNSIGNED_INT, (void*)0);

	glBindVertexArray(0);
	glUseProgram(0);
	
}


void Bacterium::clean() const {
	glDeleteVertexArrays(1, &_mesh.vId);
	glDeleteProgram(_program->id());
}

