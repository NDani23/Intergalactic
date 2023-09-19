#include "LaserGun.h"

LaserGun::LaserGun()
{
	m_position = glm::vec3(0,0,0);
	m_shootDir = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_projectileType = ProjectileType::laser;
	m_coolDownTime = 0.5f;
	m_coolDownCounter = 0.0f;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/laser_gun.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/grey_tex.jpg");
}

LaserGun::LaserGun(glm::vec3 pos, glm::vec3 shootdir)
{
	m_position = pos;
	m_shootDir = shootdir;
	m_transforms = glm::translate(pos);
	m_projectileType = ProjectileType::laser;
	m_coolDownTime = 0.5f;
	m_coolDownCounter = 0.0f;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/laser_gun.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/grey_tex.jpg");
}

void LaserGun::Shoot(std::vector<Projectile>& projectiles)
{
	/*if (m_coolDownCounter <= 0.0f)
	{
		Laser laser_proj(m_position, m_shootDir);
		projectiles.emplace_back(std::move(laser_proj));

		m_coolDownCounter = m_coolDownTime;
	}
	else
	{
		m_coolDownCounter -= 0.1f;
	}*/
	

	Laser laser_proj(m_position, m_shootDir);
	projectiles.emplace_back(std::move(laser_proj));
}