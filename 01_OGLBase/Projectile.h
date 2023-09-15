#pragma once

#include "Entity.h"

class Projectile : Entity
{
protected:
	float m_speed;
	glm::vec3 m_direction;
	int m_damage;

public:
	float GetSpeed();
	glm::vec3 GetDirection();
	int GetDamage();
};