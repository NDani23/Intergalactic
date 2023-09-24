#include "Turret.h"

Turret::Turret()
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_shootDir = glm::vec3(1.0f, 0.0f, 0.0f);
	m_transforms = glm::lookAt(m_position, m_position + m_shootDir, glm::vec3(0.0f, 1.0f, 0.0f));

	HitBox hitbox = { m_position, {10.0f, 10.0f, 10.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/turret.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/turret_tex.png");

	m_lastShootTime = std::chrono::system_clock::now();

	m_coolDownTime = 0.25f;

	m_reference = nullptr;
	m_projectiles = nullptr;
}

Turret::Turret(glm::vec3 Pos, Entity* ref, std::vector<Projectile>* proj)
{
	m_position = Pos;
	m_reference = ref;

	m_shootDir = glm::normalize(ref->GetPos() - m_position);
	m_transforms = glm::lookAt(m_position, m_position + m_shootDir, glm::vec3(0.0f, 1.0f, 0.0f));

	HitBox hitbox = { m_position, {10.0f, 10.0f, 10.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/turret.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/turret_tex.png");

	m_lastShootTime = std::chrono::system_clock::now();

	m_coolDownTime = 0.25f;

	m_projectiles = proj;
}

void Turret::Update()
{
	glm::vec3 diff_vec = m_reference->GetPos() - m_position;
	m_shootDir = glm::normalize(diff_vec);

	m_transforms = glm::lookAt(m_position, m_position + m_shootDir, glm::vec3(0.0f, 1.0f, 0.0f));

	if (glm::length(diff_vec) < 200.0f)
	{
		Shoot();
	}
}

void Turret::Shoot()
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastShootTime;

	if (elapsed_seconds.count() >= m_coolDownTime)
	{
		Laser laser_proj(m_position, m_shootDir);
		m_projectiles->emplace_back(std::move(laser_proj));

		m_lastShootTime = std::chrono::system_clock::now();
	}
}

void Turret::SetReference(Entity* ref)
{
	m_reference = ref;
}