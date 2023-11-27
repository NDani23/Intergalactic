#pragma once

#include "headers/Entity.h"

class Projectile : public Entity
{
protected:
	float m_speed;
	glm::vec3 m_direction;
	int m_damage;

public:
	float GetSpeed();
	glm::vec3 GetDirection();
	int GetDamage();
	void SetPosition(const glm::vec3&);
	virtual bool CheckHit(Entity*);

	bool operator==(const Projectile&) const;
};