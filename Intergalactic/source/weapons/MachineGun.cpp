#include "headers/weapons/MachineGun.h"
#include "headers/Player.h"

MachineGun::MachineGun()
{
	m_ID = 2;
	m_parent = nullptr;
	m_position = glm::vec3(0, 0, 0);
	m_shootDir = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 0.1f;
	m_currentCoolDown = 0.f;

	m_side = 0;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/machine_gun.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/machine_gun_tex.png");
	m_projectileImage.FromFile("assets/Weapons&Projectiles/machine_gun.png");

	m_shootSound = Mix_LoadWAV("assets/sound/laser.mp3");

	if (m_shootSound == nullptr)
	{
		std::cout << "could not load audio file!" << std::endl;
	}
}

MachineGun::~MachineGun()
{
	if (m_shootSound != nullptr)
	{
		Mix_FreeChunk(m_shootSound);
	}
}

MachineGun::MachineGun(Player* target, int side)
{
	m_ID = 2;
	m_parent = target;
	m_side = side;
	m_shootDir = m_parent->GetForwardVec();
	m_position = m_parent->GetPos() - (float)m_side * (m_parent->GetCrossVec() * 2.5f) - (m_parent->GetForwardVec() * 2.f) - (m_parent->GetUpVec() * 0.35f);
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_shootDir, m_parent->GetUpVec()));
	m_coolDownTime = 0.1f;
	m_currentCoolDown = 0.f;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/machine_gun.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/machine_gun_tex.png");
	m_projectileImage.FromFile("assets/Weapons&Projectiles/machine_gun.png");

	m_shootSound = Mix_LoadWAV("assets/sound/laser.mp3");

	if (m_shootSound == nullptr)
	{
		std::cout << "could not load audio file!" << std::endl;
	}
}

void MachineGun::Shoot(std::vector<std::unique_ptr<Projectile>>& projectiles)
{
	if (m_currentCoolDown <= 0.f)
	{
		projectiles.emplace_back(std::make_unique<Laser>(m_position + m_shootDir * 5.f, m_shootDir));

		m_currentCoolDown = m_coolDownTime;
		Mix_PlayChannel(0, m_shootSound, 0);
	}
}

void MachineGun::Update(const float delta)
{
	m_position = m_parent->GetPos() - (float)m_side * (m_parent->GetCrossVec() * 2.5f) - (m_parent->GetForwardVec() * 2.f) - (m_parent->GetUpVec() * 0.35f);

	if (m_active)
	{
		glm::vec3 m_tempDir = glm::normalize(m_parent->GetCursorVec());
		float dot_up = glm::dot(m_parent->GetUpVec(), m_tempDir);
		float dot_cross = glm::dot(m_parent->GetCrossVec(), m_tempDir);
		//forward
		if (!m_parent->IsLookingBack())
		{
			dot_cross = m_side == -1 ? std::max(dot_cross, -0.15f) : std::min(dot_cross, 0.15f);
			m_tempDir = glm::normalize(m_parent->GetForwardVec() + m_parent->GetCrossVec() * dot_cross + m_parent->GetUpVec() * std::min(dot_up, 0.27f));
		}
		else
		{
			dot_cross = m_side == 1 ? std::max(dot_cross, -0.4f) : std::min(dot_cross, 0.4f);
			m_tempDir = glm::normalize(-m_parent->GetForwardVec() - m_parent->GetCrossVec() * dot_cross + m_parent->GetUpVec() * std::min(dot_up, 0.085f) * 2.f);
		}

		m_shootDir = m_tempDir;

	}
	else
	{
		m_shootDir = m_parent->GetForwardVec();
	}

	if (m_currentCoolDown > 0.f) m_currentCoolDown = std::max(0.f, m_currentCoolDown - delta);
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_shootDir, m_parent->GetUpVec()));
}
