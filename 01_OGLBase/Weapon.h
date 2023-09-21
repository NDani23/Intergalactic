#pragma once

#include "Entity.h"
#include <chrono> 

class Weapon : public Entity
{
protected:
	glm::vec3 m_shootDir;
	float m_coolDownTime;
	std::chrono::time_point<std::chrono::system_clock> m_lastShootTime;

public:

	glm::vec3 GetShootDir() const;
	float GetCoolDownTime();
	void SetPosition(const glm::vec3);
	void SetShootDir(const glm::vec3);
};
