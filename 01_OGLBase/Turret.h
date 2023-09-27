#pragma once

#include "Weapon.h"
#include "Laser.h"

class Turret : public Weapon
{
private:
	Entity* m_reference;
	std::vector<Projectile>* m_projectiles;
public:
	Turret();
	Turret(glm::vec3, Entity*, std::vector<Projectile>*);
	void Update(const float& delta) override;
	void Shoot();
	void SetReference(Entity*);
};
