#pragma once

#include <random>

#include "Scene.h"
#include "headers/weapons/LaserGun.h"
#include "headers/weapons/Projectile.h"
#include "headers/Player.h"
#include "headers/weapons/Turret.h"
#include "headers/enemy/Enemy.h"

class DeepSpace : public Scene
{
private:
	void CreateMeteorField();
public:
	DeepSpace(Player*);

	void LoadScene() override;
};