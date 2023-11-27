#include "headers/upgrades/Shield.h"
#include "headers/Player.h"
#include "headers/scenes/Scene.h"

Shield::Shield()
{
	m_ID = 1;
	m_parent = nullptr;
	m_position = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 30.f;
	m_currentCoolDown = 0.f;
	m_durationTime = 10.f;
	m_activeTime = 0.f;

	m_barrier = std::make_shared<Barrier>(m_position);

	m_active = false;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Upgrades/shield.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Upgrades/shield_tex.png");
	m_Image.FromFile("assets/Upgrades/shield.png");

}

Shield::Shield(Player* parent)
{
	m_ID = 1;
	m_parent = parent;
	m_active = false;
	m_position = m_parent->GetPos();
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - parent->GetForwardVec(), m_parent->GetUpVec()));
	m_coolDownTime = 30.f;
	m_currentCoolDown = 0.f;
	m_durationTime = 10.f;
	m_activeTime = 0.f;

	m_barrier = std::make_shared<Barrier>(m_position);

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Upgrades/shield.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Upgrades/shield_tex.png");
	m_Image.FromFile("assets/Upgrades/shield.png");
}

void Shield::Update(const float delta)
{
	m_position = m_parent->GetPos();
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_parent->GetForwardVec(), m_parent->GetUpVec()));

	if (m_active)
	{
		m_barrier->SetPos(m_position);
		m_activeTime -= delta;
	}
	else if (m_currentCoolDown > 0.f)
	{
		m_currentCoolDown = std::max(0.f, m_currentCoolDown - delta);
	}

	if (m_active && m_activeTime <= 0.f)
	{
		m_parent->setImmortal(false);

		m_active = false;
		m_activeTime = 0.f;
		m_currentCoolDown = m_coolDownTime;
	}
}

void Shield::Activate()
{
	if (m_currentCoolDown <= 0.f)
	{
		//Put barrier around player
		m_parent->setImmortal(true);
		m_activeTime = m_durationTime;
		m_active = true;
		m_currentCoolDown = m_coolDownTime;
	}
}

void Shield::DrawMesh(ProgramObject& program, glm::mat4& viewProj)
{
	program.SetTexture("texImage", 0, m_texture);
	program.SetUniform("MVP", viewProj * m_transforms);
	program.SetUniform("world", m_transforms);
	program.SetUniform("worldIT", glm::inverse(glm::transpose(m_transforms)));

	m_mesh->draw();

	if (m_active) m_barrier->DrawMesh(program, viewProj);
}

