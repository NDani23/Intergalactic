#include "../headers/LaserGun.h"
#include "../headers/Player.h"

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

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/laser_gun2.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/grey_tex.jpg");
	m_projectileImage.FromFile("assets/Weapons&Projectiles/laser.png");
}

LaserGun::LaserGun(Player* parent)
{
	m_parent = parent;

	m_position = m_parent->GetPos();
	m_shootDir = m_parent->GetForwardVec();
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 0.25f;
	m_lastShootTime = std::chrono::system_clock::now();

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/laser_gun2.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/grey_tex.jpg");
	m_projectileImage.FromFile("assets/Weapons&Projectiles/laser.png");
}

void LaserGun::Shoot(std::vector<std::unique_ptr<Projectile>>& projectiles)
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastShootTime;

	if (elapsed_seconds.count() >= m_coolDownTime)
	{
		projectiles.emplace_back(std::make_unique<Laser>(m_position + m_parent->GetCrossVec(), m_shootDir));
		projectiles.emplace_back(std::make_unique<Laser>(m_position - m_parent->GetCrossVec(), m_shootDir));

		m_lastShootTime = std::chrono::system_clock::now();
	}
	
}

void LaserGun::Shoot(std::vector<std::unique_ptr<Projectile>>& projectiles, int damage)
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastShootTime;

	if (elapsed_seconds.count() >= m_coolDownTime)
	{
		projectiles.emplace_back(std::make_unique<Laser>(m_position + m_parent->GetCrossVec(), m_shootDir, damage));

		projectiles.emplace_back(std::make_unique<Laser>(m_position - m_parent->GetCrossVec(), m_shootDir, damage));

		m_lastShootTime = std::chrono::system_clock::now();
	}
}

void LaserGun::Update()
{
	m_shootDir = m_parent->GetForwardVec();
	m_position = m_parent->GetPos();
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_shootDir, m_parent->GetUpVec()));
}