#pragma once

#include "Map.h"
#include "DesertFloor.h"

class PlanetEarth : public Map
{
private:
	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec3 c;
	};

	DesertFloor m_Floor;
public:
	PlanetEarth(std::vector<std::unique_ptr<Projectile>>*, Player*);

	void LoadMap() override;
	void DrawEntities(glm::mat4&, GameState&) override;
};
