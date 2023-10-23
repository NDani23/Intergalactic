#pragma once

#include "Map.h"

class PlanetEarth : public Map
{
private:
	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec3 c;
	};

	GLuint m_FloorVaoID = 0;
	GLuint m_FloorVboID = 0;
	GLuint m_FloorIbID = 0;

	ProgramObject m_floorProgram;
	glm::vec3 GetFloorUV(float, float);
	glm::vec3 GetFloorNorm(float, float);
	void DrawFloor(glm::mat4&);
	void InitFloor();
	float GetZCoord(float, float);
public:
	PlanetEarth(std::vector<std::unique_ptr<Projectile>>*, Player*);

	void LoadMap() override;
	void DrawEntities(glm::mat4&, GameState&) override;
};
