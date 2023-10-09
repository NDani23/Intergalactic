#pragma once

#include "map.cpp"
#include "LaserGun.h"
#include <random>
#include "Projectile.h"
#include "Player.h"
#include "Turret.h"
#include "Enemy.h"
#include "Mothership.h"

namespace Map1 {

	void InitMap(Map&, std::vector<Projectile>&, Player*);
	void ResetMap(Map&, std::vector<Projectile>&, Player*);

	static void CreateMeteorField(Map&);
}
