#include "Shield.h"
#include "Player.h"
#include "Map.h"

Shield::Shield()
{
	m_ID = 1;
	m_parent = nullptr;
	m_position = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 30.f;
	m_durationTime = 5.f;
	m_lastActiveTime = std::chrono::system_clock::now();

	m_barrier = std::make_shared<Barrier>(m_position);

	m_active = false;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/shield.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/shield_tex.png");
	m_Image.FromFile("assets/shield.png");

}

Shield::Shield(Player* parent)
{
	m_ID = 1;
	m_parent = parent;
	m_active = false;
	m_position = m_parent->GetPos();
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - parent->GetForwardVec(), m_parent->GetUpVec()));
	m_coolDownTime = 30.f;
	m_durationTime = 5.f;
	m_lastActiveTime = std::chrono::system_clock::now();

	m_barrier = std::make_shared<Barrier>(m_position);

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/shield.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/shield_tex.png");
	m_Image.FromFile("assets/shield.png");
}

void Shield::Update()
{
	m_position = m_parent->GetPos();
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_parent->GetForwardVec(), m_parent->GetUpVec()));

	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastActiveTime;

	if (m_active)
	{
		m_barrier->SetPos(m_position);
	}

	if (m_active && elapsed_seconds.count() >= m_durationTime)
	{
		auto position = std::find(m_parent->GetMapPtr()->GetEntities().begin(), m_parent->GetMapPtr()->GetEntities().end(), m_barrier);
		if (position != m_parent->GetMapPtr()->GetEntities().end())
			m_parent->GetMapPtr()->GetEntities().erase(position);

		m_active = false;
		if (m_parent->GetSpeed() > m_parent->GetMaxSpeed()) m_parent->setSpeed(m_parent->GetMaxSpeed());
	}
}

void Shield::Activate()
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastActiveTime;

	if (elapsed_seconds.count() >= m_coolDownTime)
	{
		//Put barrier around player
		m_parent->GetMapPtr()->AddEntity(m_barrier);
		m_lastActiveTime = std::chrono::system_clock::now();
		m_active = true;
	}
}

