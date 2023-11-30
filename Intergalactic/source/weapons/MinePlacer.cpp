#include "headers/weapons/MinePlacer.h"
#include "headers/Player.h"

MinePlacer::MinePlacer()
{
	m_ID = 1;
	m_parent = nullptr;
	m_position = glm::vec3(0, 0, 0);
	m_shootDir = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 15.f;
	m_currentCoolDown = 0.f;

	m_side = 0;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/mine_placer.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/grey_tex.jpg");
	m_projectileImage.FromFile("assets/Weapons&Projectiles/mine.png");

	m_shootSound = Mix_LoadWAV("assets/sound/mineplace.mp3");

	if (m_shootSound == nullptr)
	{
		std::cout << "could not load audio file!" << std::endl;
	}
}

MinePlacer::~MinePlacer()
{
	if (m_shootSound != nullptr)
	{
		Mix_FreeChunk(m_shootSound);
	}
}

MinePlacer::MinePlacer(Player* target, int side)
{
	m_ID = 1;
	m_parent = target;
	m_side = side;
	m_shootDir = glm::vec3(0, 0, 0);
	m_position = m_parent->GetPos() - (float)m_side * (m_parent->GetCrossVec() * 2.5f) - (m_parent->GetForwardVec() * 2.f) - (m_parent->GetUpVec() * 0.2f);
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_parent->GetForwardVec(), m_parent->GetUpVec()));
	m_coolDownTime = 15.f;
	m_currentCoolDown = 0.f;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/mine_placer.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/grey_tex.jpg");
	m_projectileImage.FromFile("assets/Weapons&Projectiles/mine.png");

	m_shootSound = Mix_LoadWAV("assets/sound/mineplace.mp3");

	if (m_shootSound == nullptr)
	{
		std::cout << "could not load audio file!" << std::endl;
	}

}

void MinePlacer::Shoot(std::vector<std::unique_ptr<Projectile>>& projectiles)
{
	if (m_currentCoolDown <= 0.f)
	{
		projectiles.emplace_back(std::make_unique<Mine>(m_position, m_parent->GetParticleSystem()));
		m_currentCoolDown = m_coolDownTime;
		Mix_PlayChannel(0, m_shootSound, 0);
	}
}

void MinePlacer::Update(const float delta)
{
	m_position = m_parent->GetPos() - (float)m_side * (m_parent->GetCrossVec() * 2.5f) - (m_parent->GetForwardVec() * 2.f) - (m_parent->GetUpVec() * 0.2f);
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_parent->GetForwardVec(), m_parent->GetUpVec()));

	if (m_currentCoolDown > 0.f) m_currentCoolDown = std::max(0.f, m_currentCoolDown - delta);
}