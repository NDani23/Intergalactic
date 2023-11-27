#include "../../headers/weapons/Turret.h"
#include "../../headers/Player.h"

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

	m_currentCoolDown = 0.25f;

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

	m_currentCoolDown = 0.25f;

	m_coolDownTime = 0.5f;

	m_projectiles = proj;

	SetTransforms(glm::inverse(glm::lookAt(m_position, m_position + m_shootDir, glm::vec3(0.0f, 1.0f, 0.0f))));
}

bool Turret::Update(const float& delta)
{

	glm::vec3 diff_vec = (m_reference->GetPos() + m_reference->GetForwardVec() * 20.f) - m_position;
	m_shootDir = glm::normalize(diff_vec);
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_shootDir, glm::vec3(0.0f, 1.0f, 0.0f)));

	if (m_currentCoolDown > 0.f) m_currentCoolDown = std::max(0.f, m_currentCoolDown - delta);

	if (glm::length(diff_vec) < 300.0f)
	{
		Shoot();
	}

	return false;
}

void Turret::Shoot()
{
	if (m_currentCoolDown <= 0.f)
	{
		m_projectiles->emplace_back(std::make_unique<Laser>(m_position + m_shootDir * 5.f, m_shootDir));

		m_currentCoolDown = m_coolDownTime;
	}
}

void Turret::SetReference(Player* ref)
{
	m_reference = ref;
}