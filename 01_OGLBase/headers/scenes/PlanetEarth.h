#pragma once

#include "DesertFloor.h"
#include "Scene.h"
#include "../weapons/LaserGun.h"
#include <random>
#include "../weapons/Projectile.h"
#include "../Player.h"
#include "../weapons/Turret.h"
#include "../enemy/Enemy.h"
#include "../collision_detection/HitBox.h"

class PlanetEarth : public Scene
{
private:
	struct Vertex
	{
		glm::vec3 p;
		glm::vec3 n;
		glm::vec3 c;
	};
public:
	PlanetEarth(Player*);

	void LoadScene() override;
};
