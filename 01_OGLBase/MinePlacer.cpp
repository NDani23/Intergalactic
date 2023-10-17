#include "MinePlacer.h"
#include "PLayer.h"

MinePlacer::MinePlacer()
{
	m_parent = nullptr;
	m_position = glm::vec3(0, 0, 0);
	m_shootDir = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 10.f;
	m_lastShootTime = std::chrono::system_clock::now();

	m_side = 0;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/mine_placer.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/grey_tex.jpg");
	m_projectileImage.FromFile("assets/mine.png");
}

MinePlacer::MinePlacer(Player* target, int side)
{
	m_parent = target;
	m_side = side;
	m_shootDir = glm::vec3(0, 0, 0);
	m_position = m_parent->GetPos() - (float)m_side * (m_parent->GetCrossVec() * 2.5f) - (m_parent->GetForwardVec() * 2.f) - (m_parent->GetUpVec() * 0.2f);
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_parent->GetForwardVec(), m_parent->GetUpVec()));
	m_coolDownTime = 10.f;
	m_lastShootTime = std::chrono::system_clock::now();

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/mine_placer.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/grey_tex.jpg");
	m_projectileImage.FromFile("assets/mine.png");
}

void MinePlacer::Shoot(std::vector<std::unique_ptr<Projectile>>& projectiles)
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastShootTime;

	if (elapsed_seconds.count() >= m_coolDownTime)
	{
		projectiles.emplace_back(std::make_unique<Laser>(m_position, m_parent->GetForwardVec()));

		m_lastShootTime = std::chrono::system_clock::now();
	}
}

void MinePlacer::Update()
{
	m_position = m_parent->GetPos() - (float)m_side * (m_parent->GetCrossVec() * 2.5f) - (m_parent->GetForwardVec() * 2.f) - (m_parent->GetUpVec() * 0.2f);
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_parent->GetForwardVec(), m_parent->GetUpVec()));
}