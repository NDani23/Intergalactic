#pragma once

#include "headers/Entity.h"


class Barrier : public Entity
{
private:
	ProgramObject m_transparentProgram;

public:
	Barrier();
	Barrier(glm::vec3);

	void SetPos(glm::vec3);
	void DrawMesh(ProgramObject&, glm::mat4&) override;
};
