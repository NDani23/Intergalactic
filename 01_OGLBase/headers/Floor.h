#pragma once

#include <glm/glm.hpp>
#include "Player.h"

#include "../includes/ProgramObject.h"


class Floor
{
protected:
	glm::vec2 m_Res;

	GLuint m_VaoID = 0;
	GLuint m_VboID = 0;
	GLuint m_IbID = 0;

	ProgramObject m_Program;

	virtual glm::vec3 GetUV(float, float);
public:
	Floor() {};
	virtual void DrawFloor(glm::mat4&, Player*) {};
	virtual bool DetectCollision(Entity&);

	virtual float GetZCoord(float, float);
	virtual glm::vec3 GetNorm(float, float);
};
