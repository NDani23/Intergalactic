#include "headers/enemy/Raptor.h"
#include "headers/scenes/Scene.h"

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

	m_hitboxes.resize(1);
	m_hitboxes[0] = { m_position, {7.0f, 2.0f, 8.0f} };
	m_baseDimensions = { 7.0f, 2.0f, 8.0f };

	m_health = 200;
	m_speed = 200;
	m_damage = 20;
	m_mobility = 13;
	m_shootAngle = 0.3f;
	m_shootDistance = 300.f;

	m_coolDownTime = 0.20f;
	m_currentCoolDown = 0.f;

	m_shootSound = Mix_LoadWAV("assets/sound/laser.mp3");

	if (m_shootSound == nullptr)
	{
		std::cout << "could not load audio file!" << std::endl;
	}
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
	m_hitboxes[0] = { m_position, {7.0f, 2.0f, 8.0f} };
	m_baseDimensions = { 7.0f, 2.0f, 8.0f };

	m_health = 200;
	m_speed = 180;
	m_damage = 20;
	m_mobility = 13;
	m_shootAngle = 0.3f;
	m_shootDistance = 300.f;

	m_mesh = nullptr;

	m_coolDownTime = 0.2f;
	m_currentCoolDown = 0.f;

	m_shootSound = Mix_LoadWAV("assets/sound/laser.mp3");

	if (m_shootSound == nullptr)
	{
		std::cout << "could not load audio file!" << std::endl;
	}
}


void Raptor::Shoot()
{
	if (m_currentCoolDown <= 0.f)
	{
		m_projectiles->emplace_back(std::make_unique<Laser>(m_position + m_forward_vec * 10.f, m_shootDir, m_damage));

		m_currentCoolDown = m_coolDownTime;

		float soundVolume = (1 - glm::smoothstep(0.f, 350.0f, glm::distance(m_target->GetPos(), m_position))) * 10;
		Mix_Volume(0, soundVolume);
		Mix_PlayChannel(0, m_shootSound, 0);
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

std::unique_ptr<Mesh>& Raptor::GetMesh()
{
	return m_static_mesh;
}