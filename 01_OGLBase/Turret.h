#pragma once

#include "Weapon.h"
#include "Laser.h"

class Turret : public Weapon
{
private:
	Entity* m_reference;
	std::vector<std::unique_ptr<Projectile>>* m_projectiles;
public:
	Turret();
	Turret(glm::vec3, Entity*, std::vector<std::unique_ptr<Projectile>>*);
	virtual bool Update(const float& delta) override;
	void Shoot();
	void SetReference(Entity*);
};
