#include "Weapon.h"


Weapon::Weapon()
{
	m_parent = nullptr;
	m_position = glm::vec3(0, 0, 0);
	m_shootDir = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 0.0f;
	m_lastShootTime = std::chrono::system_clock::now();

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = nullptr;
}


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

Texture2D& Weapon::GetProjectileImage()
{
	return m_projectileImage;
}

void Weapon::Update()
{

}