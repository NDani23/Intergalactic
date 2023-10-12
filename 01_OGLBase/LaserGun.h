#pragma once

#include "Weapon.h"
#include "Laser.h"

class Player;

class LaserGun : public Weapon
{
private:
	Player* m_parent;
public:
	LaserGun();
	LaserGun(glm::vec3, glm::vec3, Player*);
	void Shoot(std::vector<Projectile>&);
	void Shoot(std::vector<Projectile>&, int);
	void SetCooldown(float);
	void SetParent(Player*);

};
