#pragma once

#include "Weapon.h"
#include "Laser.h"

class Player;

class LaserGun : public Weapon
{
private:
public:
	LaserGun();
	LaserGun(Player*);
	void Shoot(std::vector<std::unique_ptr<Projectile>>&) override;
	void Shoot(std::vector<std::unique_ptr<Projectile>>&, int) override;
	void Update(const float) override;
};
