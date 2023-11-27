#include "headers/enemy/Rocketer.h"
#include "headers/scenes/Scene.h"

std::unique_ptr<Mesh> Rocketer::m_static_mesh;
Texture2D Rocketer::m_static_tex;

int Rocketer::FirstInit()
{
	Rocketer::m_static_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Enemies/rocketer.obj"));
	Rocketer::m_static_mesh->initBuffers();
	Rocketer::m_static_tex = Texture2D("assets/Enemies/rocketer_tex.png");

	return 1;
}

Rocketer::Rocketer()
{
	static int onFirstCall = FirstInit();
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_forward_vec = glm::vec3(1.0f, 0.0f, 0.0f);
	m_shootDir = m_forward_vec;
	m_up_vec = glm::vec3(0.0f, 1.0f, 0.0f);

	SetTransforms(glm::inverse(glm::lookAt(m_position, m_position + m_forward_vec, glm::vec3(0.0f, 1.0f, 0.0f))));

	HitBox hitbox = { m_position, {8.0f, 2.5f, 10.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_health = 150;
	m_speed = 100;
	m_damage = 10;
	m_mobility = 9;
	m_shootAngle = 0.5f;
	m_shootDistance = 400.f;

	m_mesh = nullptr;

	m_coolDownTime = 10.f;
	m_currentCoolDown = 0.f;
}

Rocketer::Rocketer(glm::vec3 pos, Player* target, std::vector<std::unique_ptr<Projectile>>* projectiles, Scene* scene)
{
	static int onFirstCall = FirstInit();
	m_position = pos;
	m_target = target;
	m_projectiles = projectiles;
	m_Scene = scene;

	m_tailFire.Setfrequency(0.025f);
	m_tailFire.SetPartycleSystem(&m_Scene->GetParticleSystem());

	m_forward_vec = glm::normalize(glm::vec3(0, 0, 0) - m_position);
	m_shootDir = m_forward_vec;
	m_up_vec = glm::vec3(0.0f, 1.0f, 0.0f);
	
	SetTransforms(glm::inverse(glm::lookAt(m_position, m_position + m_forward_vec, glm::vec3(0.0f, 1.0f, 0.0f))));

	m_hitboxes.resize(1);
	m_hitboxes[0] = { m_position, {8.0f, 2.5f, 10.0f} };

	m_health = 150;
	m_speed = 100;
	m_damage = 10;
	m_mobility = 9;
	m_shootAngle = 0.5f;
	m_shootDistance = 400.f;

	m_coolDownTime = 10.f;

	m_currentCoolDown = 0.f;
}


void Rocketer::Shoot()
{
	if (m_currentCoolDown <= 0.f)
	{
		m_projectiles->emplace_back(std::make_unique<Rocket>(m_position + m_forward_vec * 10.f, m_target));

		m_currentCoolDown = m_coolDownTime;
	}
}

void Rocketer::DrawMesh(ProgramObject& program, glm::mat4& viewProj)
{
	program.SetTexture("texImage", 0, Rocketer::m_static_tex);
	program.SetUniform("MVP", viewProj * m_transforms);
	program.SetUniform("world", m_transforms);
	program.SetUniform("worldIT", glm::inverse(glm::transpose(m_transforms)));

	m_static_mesh->draw();
}

void Rocketer::UpdateDimensions()
{
	HitBox newHitBox = { m_position, {8.0f, 2.5f, 10.0f} };
	glm::vec3 cross_vec = glm::normalize(glm::cross(m_forward_vec, m_up_vec));

	newHitBox.dimensions.height = 2.5f + ((abs(m_up_vec.y) - 1) * (8.0f - 2.5f)) / -1;
	newHitBox.dimensions.height = std::max(2.5 + ((abs(m_forward_vec.y) - 0) * (10.0f - 2.5f)) / 1, (double)newHitBox.dimensions.height);

	newHitBox.dimensions.width = 8.0f + ((abs(cross_vec.x) - 1) * (2.5f - 8.0f)) / -1;
	newHitBox.dimensions.width = std::max(2.5 + ((abs(m_forward_vec.x)) * (10.0f - 2.5f)) / 1, (double)newHitBox.dimensions.width);

	newHitBox.dimensions.length = 2.5f + ((abs(m_forward_vec.z)) * (10.0f - 2.5f)) / 1;
	newHitBox.dimensions.length = std::max(2.5f + ((abs(cross_vec.z)) * (8.0f - 2.5)) / 1, (double)newHitBox.dimensions.length);

	m_hitboxes[0] = std::move(newHitBox);
}

std::unique_ptr<Mesh>& Rocketer::GetMesh()
{
	return m_static_mesh;
}