#pragma once

#include "Weapon.h"
#include "Updatable.h"
#include "Laser.h"

class Turret : public Weapon, public Updatable
{
private:
	Entity* m_reference;
	std::vector<Projectile>* m_projectiles;
public:
	Turret();
	Turret(glm::vec3, Entity*, std::vector<Projectile>*);
	void Update();
	void Shoot();
	void SetReference(Entity*);
};
