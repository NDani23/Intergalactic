#include "headers/upgrades/SpeedBooster.h"
#include "headers/Player.h"
#include "headers/scenes/Scene.h"

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

	ParticleProps particleProp;
	particleProp.ColorEnd = { 200.f / 255.f, 200.f / 255.f, 200.f / 255.f, 1.f };
	particleProp.ColorBegin = { 254.f / 255.f, 109.f / 255.f, 41 / 255.f, 1.f };
	particleProp.SizeBegin = 0.4f;
	particleProp.SizeVariation = 0.1f;
	particleProp.SizeEnd = 0.0f;
	particleProp.LifeTime = 0.04f;
	particleProp.Velocity = { 0.0f, 0.0f, 0.0f };
	particleProp.VelocityVariation = { 3.f, 3.f, 3.f };
	particleProp.Position = { 0.0f, 0.f, 0.0f };

	m_TailFireLeft.SetParticleProp(particleProp);
	m_TailFireLeft.Setfrequency(0.025f);
	m_TailFireLeft.SetPartycleSystem(&m_parent->GetMapPtr()->GetParticleSystem());

	m_TailFireRight.SetParticleProp(particleProp);
	m_TailFireRight.Setfrequency(0.025f);
	m_TailFireRight.SetPartycleSystem(&m_parent->GetMapPtr()->GetParticleSystem());

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

		m_TailFireLeft.SetPosition(m_position + m_parent->GetCrossVec() * 4.2f - m_parent->GetForwardVec() * 5.f);
		m_TailFireLeft.Update(delta);

		m_TailFireRight.SetPosition(m_position - m_parent->GetCrossVec() * 4.2f - m_parent->GetForwardVec() * 5.f);
		m_TailFireRight.Update(delta);
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
		Mix_Volume(3, 5);
	}
}

void SpeedBooster::Activate()
{
	if (m_currentCoolDown <= 0.f)
	{
		m_TailFireLeft.SetPartycleSystem(&m_parent->GetMapPtr()->GetParticleSystem());
		m_TailFireRight.SetPartycleSystem(&m_parent->GetMapPtr()->GetParticleSystem());

		m_parent->setSpeed(m_parent->GetMaxSpeed() * 2.f);
		m_activeTime = m_durationTime;
		m_active = true;
		m_currentCoolDown = m_coolDownTime;

		Mix_Volume(3, 10);
	}
}