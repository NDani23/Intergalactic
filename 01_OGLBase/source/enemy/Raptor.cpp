#include "../../headers/enemy/Raptor.h"
#include "../../headers/scenes/Scene.h"

std::unique_ptr<Mesh> Raptor::m_static_mesh;
Texture2D Raptor::m_static_tex;

int Raptor::FirstInit()
{
	Raptor::m_static_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Enemies/raptor.obj"));
	Raptor::m_static_mesh->initBuffers();
	Raptor::m_static_tex = Texture2D("assets/Enemies/raptor_tex.png");

	return 1;
}

Raptor::Raptor()
{
	static int onFirstCall = FirstInit();
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_forward_vec = glm::vec3(1.0f, 0.0f, 0.0f);
	m_shootDir = m_forward_vec;
	m_up_vec = glm::vec3(0.0f, 1.0f, 0.0f);

	SetTransforms(glm::inverse(glm::lookAt(m_position, m_position + m_forward_vec, glm::vec3(0.0f, 1.0f, 0.0f))));

	HitBox hitbox = { m_position, {10.0f, 3.0f, 11.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_health = 200;
	m_speed = 200;
	m_damage = 20;
	m_mobility = 13;
	m_shootAngle = 0.3f;
	m_shootDistance = 300.f;

	m_coolDownTime = 0.20f;
	m_currentCoolDown = 0.f;
}

Raptor::Raptor(glm::vec3 pos, Player* target, std::vector<std::unique_ptr<Projectile>>* projectiles, Scene* scene)
{
	static int onFirstCall = FirstInit();
	m_position = pos;
	m_target = target;
	m_projectiles = projectiles;
	m_Scene = scene;

	ParticleProps particleProp;	
	particleProp.ColorEnd = { 200.f / 255.f, 200.f / 255.f, 200.f / 255.f, 1.f };
	particleProp.ColorBegin = { 254.f / 255.f, 109.f / 255.f, 41 / 255.f, 1.f };
	particleProp.SizeBegin = 0.8f;
	particleProp.SizeVariation = 0.3f;
	particleProp.SizeEnd = 0.0f;
	particleProp.LifeTime = 0.08f;
	particleProp.Velocity = { 0.0f, 0.0f, 0.0f };
	particleProp.VelocityVariation = { 1.5f, 1.5f, 1.5f };
	particleProp.Position = { 0.0f, 0.f, 0.0f };

	m_tailFire.SetParticleProp(particleProp);
	m_tailFire.Setfrequency(0.020f);
	m_tailFire.SetPartycleSystem(&m_Scene->GetParticleSystem());

	m_forward_vec = glm::normalize(glm::vec3(0, 0, 0) - m_position);;
	m_shootDir = m_forward_vec;
	m_up_vec = glm::vec3(0.0f, 1.0f, 0.0f);

	SetTransforms(glm::inverse(glm::lookAt(m_position, m_position + m_forward_vec, glm::vec3(0.0f, 1.0f, 0.0f))));

	m_hitboxes.resize(1);
	m_hitboxes[0] = { m_position, {10.0f, 3.0f, 11.0f} };

	m_health = 200;
	m_speed = 180;
	m_damage = 20;
	m_mobility = 13;
	m_shootAngle = 0.3f;
	m_shootDistance = 300.f;

	m_mesh = nullptr;

	m_coolDownTime = 0.2f;
	m_currentCoolDown = 0.f;
}


void Raptor::Shoot()
{
	if (m_currentCoolDown <= 0.f)
	{
		m_projectiles->emplace_back(std::make_unique<Laser>(m_position + m_forward_vec * 10.f, m_shootDir, m_damage));

		m_currentCoolDown = m_coolDownTime;
	}
}

void Raptor::DrawMesh(ProgramObject& program, glm::mat4& viewProj)
{
	program.SetTexture("texImage", 0, Raptor::m_static_tex);
	program.SetUniform("MVP", viewProj * m_transforms);
	program.SetUniform("world", m_transforms);
	program.SetUniform("worldIT", glm::inverse(glm::transpose(m_transforms)));

	m_static_mesh->draw();
}

void Raptor::UpdateDimensions()
{
	HitBox newHitBox = { m_position, {10.0f, 3.0f, 11.0f} };
	glm::vec3 cross_vec = glm::normalize(glm::cross(m_forward_vec, m_up_vec));

	newHitBox.dimensions.height = 3.0f + ((abs(m_up_vec.y) - 1) * (10.0f - 3.0f)) / -1;
	newHitBox.dimensions.height = std::max(2.5 + ((abs(m_forward_vec.y) - 0) * (11.0f - 3.0f)) / 1, (double)newHitBox.dimensions.height);

	newHitBox.dimensions.width = 10.0f + ((abs(cross_vec.x) - 1) * (3.0f - 10.0f)) / -1;
	newHitBox.dimensions.width = std::max(2.5 + ((abs(m_forward_vec.x)) * (11.0f - 3.0f)) / 1, (double)newHitBox.dimensions.width);

	newHitBox.dimensions.length = 3.0f + ((abs(m_forward_vec.z)) * (11.0f - 3.0f)) / 1;
	newHitBox.dimensions.length = std::max(3.0f + ((abs(cross_vec.z)) * (10.0f - 3.0)) / 1, (double)newHitBox.dimensions.length);

	m_hitboxes[0] = std::move(newHitBox);
}

std::unique_ptr<Mesh>& Raptor::GetMesh()
{
	return m_static_mesh;
}