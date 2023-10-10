#pragma once

#include "Weapon.h"
#include "Laser.h"

class LaserGun : public Weapon
{
public:
	LaserGun();
	LaserGun(glm::vec3, glm::vec3);
	void Shoot(std::vector<Projectile>&);
	void Shoot(std::vector<Projectile>&, int);
	void SetCooldown(float);

};
