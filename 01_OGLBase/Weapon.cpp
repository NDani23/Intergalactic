#include "Weapon.h"

glm::vec3 Weapon::GetShootDir() const
{
	return m_shootDir;
}

ProjectileType::type Weapon::GetProjectileType()
{
	return m_projectileType;
}

float Weapon::GetCoolDownTime()
{
	return m_coolDownTime;
}

void Weapon::SetPosition(const glm::vec3 pos)
{
	m_position = pos;
}

void Weapon::SetShootDir(const glm::vec3 dir)
{
	m_shootDir = dir;
}