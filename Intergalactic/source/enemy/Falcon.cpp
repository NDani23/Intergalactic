#include "headers/enemy/Falcon.h"
#include "headers/scenes/Scene.h"

std::unique_ptr<Mesh> Falcon::m_static_mesh;
Texture2D Falcon::m_static_tex;

int Falcon::FirstInit()
{
	Falcon::m_static_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Enemies/enemy_ship.obj"));
	Falcon::m_static_mesh->initBuffers();
	Falcon::m_static_tex = Texture2D("assets/Enemies/enemy_tex.png");

	return 1;
}

Falcon::Falcon()
{
	static int onFirstCall = FirstInit();
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_forward_vec = glm::vec3(1.0f, 0.0f, 0.0f);
	m_shootDir = m_forward_vec;
	m_up_vec = glm::vec3(0.0f, 1.0f, 0.0f);

	SetTransforms(glm::inverse(glm::lookAt(m_position, m_position + m_forward_vec, glm::vec3(0.0f, 1.0f, 0.0f))));

	m_hitboxes.resize(1);
	m_hitboxes[0] = { m_position, {7.0f, 2.5f, 8.0f} };

	m_baseDimensions = { 7.0f, 2.5f, 8.0f };

	m_health = 150;
	m_speed = 120;
	m_damage = 10;
	m_mobility = 10;
	m_shootAngle = 0.2f;
	m_shootDistance = 250.f;

	m_coolDownTime = 0.25f;
	m_currentCoolDown = 0.f;

	m_shootSound = Mix_LoadWAV("assets/sound/laser.mp3");

	if (m_shootSound == nullptr)
	{
		std::cout << "could not load audio file!" << std::endl;
	}
}

Falcon::Falcon(glm::vec3 pos, Player* target, std::vector<std::unique_ptr<Projectile>>* projectiles, Scene* scene)
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
	m_hitboxes[0] = { m_position, {7.0f, 2.5f, 8.0f} };
	m_baseDimensions = { 7.0f, 2.5f, 8.0f };

	m_health = 150;
	m_speed = 120;
	m_damage = 10;
	m_mobility = 10;
	m_shootAngle = 0.2f;
	m_shootDistance = 250.f;

	m_mesh = nullptr;

	m_coolDownTime = 0.25f;
	m_currentCoolDown = 0.f;

	m_shootSound = Mix_LoadWAV("assets/sound/laser.mp3");

	if (m_shootSound == nullptr)
	{
		std::cout << "could not load audio file!" << std::endl;
	}
}


void Falcon::Shoot()
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

void Falcon::DrawMesh(ProgramObject& program, glm::mat4& viewProj)
{
	program.SetTexture("texImage", 0, Falcon::m_static_tex);
	program.SetUniform("MVP", viewProj * m_transforms);
	program.SetUniform("world", m_transforms);
	program.SetUniform("worldIT", glm::inverse(glm::transpose(m_transforms)));

	m_static_mesh->draw();
}

std::unique_ptr<Mesh>& Falcon::GetMesh()
{
	return m_static_mesh;
}