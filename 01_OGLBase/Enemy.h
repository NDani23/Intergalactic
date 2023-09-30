#pragma once

#include "Weapon.h"
#include "Laser.h"

#include <algorithm>

class Enemy : public Weapon
{
private:
	int m_health;
	float m_speed;
	glm::vec3 m_up_vec;

	Entity* m_target;
	std::vector<Projectile>* m_projectiles;

public:
	Enemy();
	Enemy(glm::vec3, Entity*, std::vector<Projectile>*);
	void Update(const float& delta) override;

private:
	HitBox UpdateDimensions();

};
