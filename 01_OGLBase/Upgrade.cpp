#include "Upgrade.h"

Upgrade::Upgrade()
{
	m_parent = nullptr;
	m_position = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 0.0f;
	m_lastShootTime = std::chrono::system_clock::now();

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = nullptr;
}

float Upgrade::GetCoolDownTime()
{
	return m_coolDownTime;
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

void Upgrade::Update()
{
}

void Upgrade::Activate()
{
}