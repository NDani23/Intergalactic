#pragma once

#include "Floor.h"

class DesertFloor : public Floor
{
private:
	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec3 c;
	};

	glm::vec3 GetUV(float, float) override;
	glm::vec3 GetNorm(float, float) override;
	float GetZCoord(float, float) override;
public:
	DesertFloor();
	virtual void DrawFloor(glm::mat4&, Player*) override;
};
