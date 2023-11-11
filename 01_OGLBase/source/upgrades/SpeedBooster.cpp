#include "../../headers/upgrades/SpeedBooster.h"
#include "../../headers/Player.h"

SpeedBooster::SpeedBooster()
{
	m_ID = 0;
	m_parent = nullptr;
	m_position = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 20.f;
	m_durationTime = 5.f;
	m_currentCoolDown = 0.f;
	m_activeTime = 0.f;

	m_active = false;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Upgrades/booster.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Upgrades/booster_tex.png");
	m_Image.FromFile("assets/Upgrades/booster.png");
}

SpeedBooster::SpeedBooster(Player* parent)
{
	m_ID = 0;
	m_parent = parent;
	m_active = false;
	m_position = m_parent->GetPos();
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - parent->GetForwardVec(), m_parent->GetUpVec()));
	m_coolDownTime = 20.f;
	m_durationTime = 5.f;
	m_currentCoolDown = 0.f;
	m_activeTime = 0.f;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Upgrades/booster.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Upgrades/booster_tex.png");
	m_Image.FromFile("assets/Upgrades/booster.png");
}

void SpeedBooster::Update(const float delta)
{
	m_position = m_parent->GetPos();
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_parent->GetForwardVec(), m_parent->GetUpVec()));


	if (m_active)
	{
		m_activeTime -= delta;
	}
	else if (m_currentCoolDown > 0.f)
	{
		m_currentCoolDown = std::max(0.f, m_currentCoolDown - delta);
	}

	if (m_active && m_activeTime <= 0.f)
	{
		m_active = false;
		m_activeTime = 0.f;
		m_currentCoolDown = m_coolDownTime;
		if(m_parent->GetSpeed() > m_parent->GetMaxSpeed()) m_parent->setSpeed(m_parent->GetMaxSpeed());
	}
}

void SpeedBooster::Activate()
{
	if (m_currentCoolDown <= 0.f)
	{
		m_parent->setSpeed(m_parent->GetMaxSpeed() * 2.f);
		m_activeTime = m_durationTime;
		m_active = true;
		m_currentCoolDown = m_coolDownTime;
	}
}