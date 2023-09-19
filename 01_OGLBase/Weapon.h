#pragma once

#include "Entity.h"
#include "ProjectileType.h"

class Weapon : public Entity
{
protected:
	glm::vec3 m_shootDir;
	ProjectileType::type m_projectileType;
	float m_coolDownTime;
	float m_coolDownCounter;

public:

	glm::vec3 GetShootDir() const;
	ProjectileType::type GetProjectileType();
	float GetCoolDownTime();
	float GetCoolDownCounter();
	void SetPosition(const glm::vec3);
	void SetShootDir(const glm::vec3);
};
