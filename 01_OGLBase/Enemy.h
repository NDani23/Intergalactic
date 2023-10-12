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
	std::vector<std::shared_ptr<Entity>>* world_objects;

public:
	Enemy();
	Enemy(glm::vec3, Entity*, std::vector<Projectile>*, std::vector<std::shared_ptr<Entity>>*);
	bool Update(const float& delta) override;
	void Shoot();
	bool Hit(int) override;
	void SetTexture();
	bool IsTargetable() override;

private:
	HitBox UpdateDimensions();

};
