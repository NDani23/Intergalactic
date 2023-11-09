#pragma once

#include "Map.h"
#include "../LaserGun.h"
#include <random>
#include "../Projectile.h"
#include "../Player.h"
#include "../Turret.h"
#include "../Enemy.h"

class DeepSpace : public Map
{
private:
	void CreateMeteorField();
public:
	DeepSpace(std::vector<std::unique_ptr<Projectile>>*, Player*);

	void LoadMap() override;
	void DrawEntities(glm::mat4&, GameState&) override;
};