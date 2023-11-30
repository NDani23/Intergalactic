#include "headers/weapons/Rocket.h"

Rocket::Rocket()
{
	m_target = nullptr;
	m_particleSystem = nullptr;
	m_transforms = glm::mat4();
	m_position = glm::vec3(0.f, 0.f, 0.f);
	m_speed = 210;
	m_direction = glm::vec3(1.0f, 0.0f, 0.0f);
	m_damage = 100;
	m_travelDistance = 0.f;

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/rocket.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/rocket_tex.png");
}

Rocket::Rocket(glm::vec3 pos, Entity* target, ParticleSystem* partSystem)
{
	m_target = target;
	m_particleSystem = partSystem;
	m_position = pos;
	m_speed = 210;
	m_direction = glm::normalize(target->GetPos() - m_position);
	m_damage = 100;
	m_travelDistance = 0.f;
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_direction, glm::vec3(0.0f, 1.0f, 0.0f)));

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/rocket.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/rocket_tex.png");

	ParticleProps particleProp;
	particleProp.ColorEnd = { 200.f / 255.f, 200.f / 255.f, 200.f / 255.f, 1.f };
	particleProp.ColorBegin = { 254.f / 255.f, 109.f / 255.f, 41 / 255.f, 1.f };
	particleProp.SizeBegin = 0.4f;
	particleProp.SizeVariation = 0.1f;
	particleProp.SizeEnd = 0.0f;
	particleProp.LifeTime = 0.05f;
	particleProp.Velocity = { 0.0f, 0.0f, 0.0f };
	particleProp.VelocityVariation = { 3.f, 3.f, 3.f };
	particleProp.Position = { 0.0f, 0.f, 0.0f };

	m_TailFire.SetParticleProp(particleProp);
	m_TailFire.Setfrequency(0.020f);
	m_TailFire.SetPartycleSystem(m_particleSystem);
}

bool Rocket::Update(const float& delta)
{

	glm::vec3 temp_dir = glm::normalize(m_target->GetPos() - m_position);

	glm::vec3 cross_vec = temp_dir - m_direction;

	if (glm::length(cross_vec) * (delta * 150.f) < 0.015f)
	{
		m_direction = temp_dir;
	}
	else
	{
		m_direction = glm::normalize(m_direction + cross_vec * 0.015f * (delta * 150.f));
	}
	glm::vec3 newPos = m_position + m_direction * (delta * m_speed);
	m_travelDistance += glm::length(newPos - m_position);
	m_position = newPos;
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_direction, glm::vec3(0.0f, 1.0f, 0.0f)));

	if (m_particleSystem != nullptr)
	{
		m_TailFire.SetPosition(m_position);
		m_TailFire.Update(delta);
	}

	return m_travelDistance > 800.f;
}

bool Rocket::CheckHit(Entity* entity)
{
	return AAB::Collide(entity->GetHitboxes(), m_position);
}