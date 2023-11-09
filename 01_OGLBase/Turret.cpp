#include "Turret.h"
#include "Player.h"

Turret::Turret()
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_shootDir = glm::vec3(1.0f, 0.0f, 0.0f);
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_shootDir, glm::vec3(0.0f, 1.0f, 0.0f)));

	HitBox hitbox = { m_position, {10.0f, 10.0f, 10.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/turret.obj"));
	m_mesh->initBuffers();

	m_collider.setVertices(m_mesh->GetVertices());

	m_texture.FromFile("assets/Weapons&Projectiles/turret_tex.png");

	m_lastShootTime = std::chrono::system_clock::now();

	m_coolDownTime = 0.25f;

	m_reference = nullptr;
	m_projectiles = nullptr;
}

Turret::Turret(glm::vec3 Pos, Player* ref, std::vector<std::unique_ptr<Projectile>>* proj)
{
	m_position = Pos;
	m_reference = ref;

	m_shootDir = glm::normalize(m_reference->GetPos() - m_position);

	HitBox hitbox = { m_position, {10.0f, 10.0f, 10.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/turret.obj"));
	m_mesh->initBuffers();

	m_collider.setVertices(m_mesh->GetVertices());

	m_texture.FromFile("assets/Weapons&Projectiles/turret_tex.png");

	m_lastShootTime = std::chrono::system_clock::now();

	m_coolDownTime = 0.5f;

	m_projectiles = proj;

	SetTransforms(glm::inverse(glm::lookAt(m_position, m_position + m_shootDir, glm::vec3(0.0f, 1.0f, 0.0f))));
}

bool Turret::Update(const float& delta)
{

	glm::vec3 diff_vec = (m_reference->GetPos() + m_reference->GetForwardVec() * 10.f) - m_position;
	m_shootDir = glm::normalize(diff_vec);

	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_shootDir, glm::vec3(0.0f, 1.0f, 0.0f)));

	if (glm::length(diff_vec) < 200.0f)
	{
		Shoot();
	}

	return false;
}

void Turret::Shoot()
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastShootTime;

	if (elapsed_seconds.count() >= m_coolDownTime)
	{
		m_projectiles->emplace_back(std::make_unique<Laser>(m_position, m_shootDir));

		m_lastShootTime = std::chrono::system_clock::now();
	}
}

void Turret::SetReference(Player* ref)
{
	m_reference = ref;
}