#pragma once

#include "Entity.h"
#include "ProjectileType.h"
#include <chrono>
#include <ctime>  

class Weapon : public Entity
{
protected:
	glm::vec3 m_shootDir;
	ProjectileType::type m_projectileType;
	float m_coolDownTime;
	std::chrono::time_point<std::chrono::system_clock> m_lastShootTime;

public:

	glm::vec3 GetShootDir() const;
	ProjectileType::type GetProjectileType();
	float GetCoolDownTime();
	void SetPosition(const glm::vec3);
	void SetShootDir(const glm::vec3);
};
