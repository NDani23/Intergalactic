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

public:
	PlanetEarth(std::vector<std::unique_ptr<Projectile>>*, Player*);

	void LoadMap() override;
	void DrawEntities(glm::mat4&, GameState&) override;
};
