#pragma once

#include "DesertFloor.h"
#include "Map.h"
#include "../weapons/LaserGun.h"
#include <random>
#include "../weapons/Projectile.h"
#include "../Player.h"
#include "../weapons/Turret.h"
#include "../enemy/Enemy.h"
#include "../collision_detection/HitBox.h"

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
	Floor* GetFloor() override;
};
