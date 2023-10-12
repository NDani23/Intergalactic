#include "RocketLauncher.h"
#include "PLayer.h"

RocketLauncher::RocketLauncher()
{
	m_parent = nullptr;
	m_position = glm::vec3(0, 0, 0);
	m_shootDir = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 5.f;
	m_lastShootTime = std::chrono::system_clock::now();

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/launcher.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/grey_tex.jpg");
	m_projectileImage.FromFile("assets/rocket.png");
}

RocketLauncher::RocketLauncher(Player* target)
{
	m_parent = target;

	m_shootDir = m_parent->GetForwardVec();
	m_position = m_parent->GetPos() + (m_parent->GetCrossVec() * 2.5f) - (m_parent->GetForwardVec() * 2.f) - (m_parent->GetUpVec() * 0.5f);
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_shootDir, m_parent->GetUpVec()));
	m_coolDownTime = 5.f;
	m_lastShootTime = std::chrono::system_clock::now();

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/launcher.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/grey_tex.jpg");
	m_projectileImage.FromFile("assets/rocket.png");
}

void RocketLauncher::Shoot(std::vector<Projectile>& projectiles)
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastShootTime;

	if (elapsed_seconds.count() >= m_coolDownTime)
	{

		Laser laser_proj2(m_position, m_shootDir);
		projectiles.emplace_back(std::move(laser_proj2));

		m_lastShootTime = std::chrono::system_clock::now();
	}
}

void RocketLauncher::Update()
{
	m_shootDir = m_parent->GetForwardVec();
	m_position = m_parent->GetPos() + (m_parent->GetCrossVec() * 2.5f) - (m_parent->GetForwardVec() * 2.f) - (m_parent->GetUpVec() * 0.5f);
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_shootDir, m_parent->GetUpVec()));
}

bool RocketLauncher::requireTarget()
{
	return true;
}