#include "headers/weapons/Weapon.h"

Weapon::Weapon()
{
	m_ID = -1;
	m_active = false;
	m_parent = nullptr;
	m_position = glm::vec3(0, 0, 0);
	m_shootDir = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 0.0f;
	m_currentCoolDown = 0.f;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = nullptr;
}


glm::vec3 Weapon::GetShootDir() const
{
	return m_shootDir;
}

bool Weapon::IsActive()
{
	return m_active;
}

float Weapon::GetCoolDownTime()
{
	return m_coolDownTime;
}

float Weapon::GetCurrentCooldown()
{
	return m_currentCoolDown;
}

void Weapon::SetActive(bool active)
{
	m_active = active;
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

void Weapon::Shoot(std::vector<std::unique_ptr<Projectile>>&)
{
}

void Weapon::Shoot(std::vector<std::unique_ptr<Projectile>>&, int)
{
}

Texture2D& Weapon::GetProjectileImage()
{
	return m_projectileImage;
}

void Weapon::Update(const float)
{

}

bool Weapon::requireTarget()
{
	return false;
}

int Weapon::ID()
{
	return m_ID;
}