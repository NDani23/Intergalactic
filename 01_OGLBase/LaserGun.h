#pragma once

#include "Weapon.h"
#include "Laser.h"

class Player;

class LaserGun : public Weapon
{
private:
public:
	LaserGun();
	LaserGun(glm::vec3, glm::vec3, Player*);
	void Shoot(std::vector<Projectile>&) override;
	void Shoot(std::vector<Projectile>&, int) override;

};
