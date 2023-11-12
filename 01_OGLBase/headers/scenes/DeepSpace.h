#pragma once

#include "Scene.h"
#include "../weapons/LaserGun.h"
#include <random>
#include "../weapons/Projectile.h"
#include "../Player.h"
#include "../weapons/Turret.h"
#include "../enemy/Enemy.h"

class DeepSpace : public Scene
{
private:
	void CreateMeteorField();
public:
	DeepSpace(std::vector<std::unique_ptr<Projectile>>*, Player*);

	void LoadScene() override;
	void DrawEntities(glm::mat4&, GameState&) override;
};