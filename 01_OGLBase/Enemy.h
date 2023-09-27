#pragma once

#include "Weapon.h"
#include "Laser.h"

class Enemy : public Weapon
{
private:
	int m_health;
	float m_speed;

	Entity* m_target;
	std::vector<Projectile>* m_projectiles;

public:
	Enemy();
	Enemy(glm::vec3, Entity*, std::vector<Projectile>*);
	void Update(const float& delta) override;

};
