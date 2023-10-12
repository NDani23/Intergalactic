#pragma once

#include "Weapon.h"
#include "Laser.h"

class RocketLauncher : public Weapon
{
private:

public:
	RocketLauncher();
	RocketLauncher(Player*);
	void Shoot(std::vector<Projectile>&) override;
};