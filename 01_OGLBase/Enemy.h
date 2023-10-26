#pragma once

#include "Weapon.h"
#include "Laser.h"

#include <algorithm>

class Map;

class Enemy : public Weapon
{
protected:
	int m_health;
	float m_speed;
	glm::vec3 m_forward_vec;
	glm::vec3 m_up_vec;

	Entity* m_target;
	std::vector<std::unique_ptr<Projectile>>* m_projectiles;
	Map* m_Map;

public:
	Enemy();
	virtual void Shoot() {};
	bool Hit(int) override;
	void SetTexture();
	bool IsTargetable() override;

private:
	virtual HitBox UpdateDimensions();
};
