#include "Weapon.h"

glm::vec3 Weapon::GetShootDir() const
{
	return m_shootDir;
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


void Weapon::SetCooldown(float cooldown)
{
	m_coolDownTime = cooldown;
}

void Weapon::SetParent(Player* parent)
{
	m_parent = parent;
}

void Weapon::Shoot(std::vector<Projectile>&)
{
}

void Weapon::Shoot(std::vector<Projectile>&, int)
{
}