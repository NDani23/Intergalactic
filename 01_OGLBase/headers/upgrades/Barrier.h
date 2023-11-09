#pragma once

#include "../Entity.h"


class Barrier : public Entity
{
private:
	ProgramObject m_transparentProgram;

public:
	Barrier();
	Barrier(glm::vec3);

	void SetPos(glm::vec3);
	bool CanCollide() override;
	void DrawMesh(ProgramObject&, glm::mat4&) override;
};
