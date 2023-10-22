#pragma once

#include "Map.h"
#include "LaserGun.h"
#include <random>
#include "Projectile.h"
#include "Player.h"
#include "Turret.h"
#include "Enemy.h"
#include "Mothership.h"

class PlanetEarth : public Map
{
private:
	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec3 c;
	};

	GLuint m_floorVaoID = 0;
	GLuint m_floorVboID = 0;
	GLuint m_floorIbID = 0;

	ProgramObject m_floorProgram;
	glm::vec3 GetFloorUV(float, float);
	void InitFloor();
public:
	PlanetEarth(std::vector<std::unique_ptr<Projectile>>*, Player*);

	void LoadMap() override;
	void DrawEntities(glm::mat4&, GameState&) override;
};
