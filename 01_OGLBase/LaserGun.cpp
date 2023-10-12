#include "LaserGun.h"
#include "PLayer.h"

LaserGun::LaserGun()
{
	m_parent = nullptr;
	m_position = glm::vec3(0,0,0);
	m_shootDir = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 0.25f;
	m_lastShootTime = std::chrono::system_clock::now();

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/laser_gun2.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/grey_tex.jpg");
}

LaserGun::LaserGun(glm::vec3 pos, glm::vec3 shootdir, Player* parent)
{
	m_parent = parent;

	m_position = pos;
	m_shootDir = shootdir;
	m_transforms = glm::translate(pos);
	m_coolDownTime = 0.25f;
	m_lastShootTime = std::chrono::system_clock::now();

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/laser_gun2.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/grey_tex.jpg");
}

void LaserGun::Shoot(std::vector<Projectile>& projectiles)
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastShootTime;

	if (elapsed_seconds.count() >= m_coolDownTime)
	{
		Laser laser_proj(m_position + m_parent->GetCrossVec(), m_shootDir);
		projectiles.emplace_back(std::move(laser_proj));

		Laser laser_proj2(m_position - m_parent->GetCrossVec(), m_shootDir);
		projectiles.emplace_back(std::move(laser_proj2));

		m_lastShootTime = std::chrono::system_clock::now();
	}
	
}

void LaserGun::Shoot(std::vector<Projectile>& projectiles, int damage)
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastShootTime;

	if (elapsed_seconds.count() >= m_coolDownTime)
	{
		Laser laser_proj(m_position + m_parent->GetCrossVec(), m_shootDir);
		projectiles.emplace_back(std::move(laser_proj));

		Laser laser_proj2(m_position - m_parent->GetCrossVec(), m_shootDir);
		projectiles.emplace_back(std::move(laser_proj2));

		m_lastShootTime = std::chrono::system_clock::now();
	}
}