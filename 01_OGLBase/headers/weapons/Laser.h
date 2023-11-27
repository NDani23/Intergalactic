#pragma once

#include "Projectile.h"

#include "../../includes/ProgramObject.h"
#include "../../includes/BufferObject.h"
#include "../../includes/VertexArrayObject.h"

#include <GL/glew.h>

class Laser : public Projectile
{
private:
	static GLuint VaoID;

	static void InitGeometry();

	float m_travelDistance;
public:
	Laser(glm::vec3, glm::vec3);
	Laser(glm::vec3, glm::vec3, int);
	bool Update(const float&) override;
	bool CheckHit(Entity*) override;
	void DrawMesh(ProgramObject&, glm::mat4&) override;
};
