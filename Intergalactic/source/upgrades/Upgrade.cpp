#include "headers/upgrades/Upgrade.h"

Upgrade::Upgrade()
{
	m_parent = nullptr;
	m_ID = -1;
	m_position = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 0.0f;
	m_currentCoolDown = 0.0f;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = nullptr;
}

float Upgrade::GetCoolDownTime()
{
	return m_coolDownTime;
}

float Upgrade::GetCurrentCooldown()
{
	return m_currentCoolDown;
}

Player* Upgrade::GetParent()
{
	return m_parent;
}

void Upgrade::SetPosition(const glm::vec3& pos)
{
	m_position = pos;
}

void Upgrade::SetCooldown(float cooldown)
{
	m_coolDownTime = cooldown;
}

void Upgrade::SetParent(Player* parent)
{
	m_parent = parent;
}

Texture2D& Upgrade::GetImage()
{
	return m_Image;
}

int Upgrade::ID()
{
	return m_ID;
}