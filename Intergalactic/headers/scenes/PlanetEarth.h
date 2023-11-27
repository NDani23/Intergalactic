#pragma once

#include <random>

#include "DesertFloor.h"
#include "Scene.h"
#include "headers/weapons/LaserGun.h"
#include "headers/weapons/Projectile.h"
#include "headers/Player.h"
#include "headers/weapons/Turret.h"
#include "headers/enemy/Enemy.h"
#include "headers/collision_detection/HitBox.h"

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
