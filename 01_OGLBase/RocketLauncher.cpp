#include "RocketLauncher.h"
#include "Player.h"

RocketLauncher::RocketLauncher()
{
	m_ID = 0;
	m_parent = nullptr;
	m_position = glm::vec3(0, 0, 0);
	m_shootDir = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 10.f;
	m_lastShootTime = std::chrono::system_clock::now();

	m_side = 0;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/launcher.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/grey_tex.jpg");
	m_projectileImage.FromFile("assets/Weapons&Projectiles/rocket.png");
}

RocketLauncher::RocketLauncher(Player* target, int side)
{
	m_ID = 0;
	m_parent = target;
	m_side = side;
	m_shootDir = m_parent->GetForwardVec();
	m_position = m_parent->GetPos() - (float)m_side*(m_parent->GetCrossVec() * 2.5f) - (m_parent->GetForwardVec() * 2.f) - (m_parent->GetUpVec() * 0.5f);
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_shootDir, m_parent->GetUpVec()));
	m_coolDownTime = 10.f;
	m_lastShootTime = std::chrono::system_clock::now();

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/launcher.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/grey_tex.jpg");
	m_projectileImage.FromFile("assets/Weapons&Projectiles/rocket.png");
}

void RocketLauncher::Shoot(std::vector<std::unique_ptr<Projectile>>& projectiles)
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastShootTime;

	if (elapsed_seconds.count() >= m_coolDownTime && m_parent->GetTarget() != nullptr)
	{
		projectiles.emplace_back(std::make_unique<Rocket>(m_position, m_parent->GetTarget()));

		m_lastShootTime = std::chrono::system_clock::now();
	}
}

void RocketLauncher::Update()
{

	m_shootDir = m_parent->GetForwardVec();
	m_position = m_parent->GetPos() - (float)m_side * (m_parent->GetCrossVec() * 2.5f) - (m_parent->GetForwardVec() * 2.f) - (m_parent->GetUpVec() * 0.5f);
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_shootDir, m_parent->GetUpVec()));
}

bool RocketLauncher::requireTarget()
{
	return true;
}