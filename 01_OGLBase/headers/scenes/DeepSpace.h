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
	DeepSpace(Player*);

	void LoadScene() override;
};