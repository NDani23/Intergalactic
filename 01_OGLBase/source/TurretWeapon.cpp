#include "../headers/TurretWeapon.h"
#include "../headers/Player.h"
#include "../headers/Enemy.h"
#include "../headers/Map.h"

TurretWeapon::TurretWeapon()
{
	m_ID = 3;
	m_parent = nullptr;
	m_target = nullptr;
	m_shooting = false;
	m_position = glm::vec3(0, 0, 0);
	m_shootDir = glm::vec3(0, 0, 0);
	m_transforms = glm::translate(m_position);
	m_coolDownTime = 60.f;
	m_fireRateCooldown = 0.2f;
	m_durationTime = 15.f;
	m_lastShootTime = std::chrono::system_clock::now();
	m_lastActivateTime = std::chrono::system_clock::now();

	m_side = 0;

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/turret_weapon.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/turret_tex.png");
	m_projectileImage.FromFile("assets/Weapons&Projectiles/turret_weapon.png");
}

TurretWeapon::TurretWeapon(Player* target, int side)
{
	m_ID = 3;
	m_parent = target;
	m_side = side;
	m_shooting = false;
	m_target = nullptr;
	m_shootDir = m_parent->GetForwardVec();
	m_position = m_parent->GetPos() - (float)m_side * (m_parent->GetCrossVec() * 2.5f) - (m_parent->GetForwardVec() * 2.f) - (m_parent->GetUpVec() * 0.5f);
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_shootDir, m_parent->GetUpVec()));
	m_coolDownTime = 60.f;
	m_fireRateCooldown = 0.2f;
	m_durationTime = 15.f;
	m_lastShootTime = std::chrono::system_clock::now();
	m_lastActivateTime = std::chrono::system_clock::now();

	HitBox hitbox = { m_position, {0.0, 0.0, 0.0} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/turret_weapon.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/turret_tex.png");
	m_projectileImage.FromFile("assets/Weapons&Projectiles/turret_weapon.png");
}

void TurretWeapon::Shoot(std::vector<std::unique_ptr<Projectile>>& projectiles)
{
	std::chrono::duration<float> last_active = std::chrono::system_clock::now() - m_lastActivateTime;
	if (last_active.count() >= m_coolDownTime)
	{
		m_shooting = true;
		m_lastActivateTime = std::chrono::system_clock::now();
	}

}

void TurretWeapon::Update()
{
	m_position = m_parent->GetPos() - (float)m_side * (m_parent->GetCrossVec() * 2.5f) - (m_parent->GetForwardVec() * 2.f) - (m_parent->GetUpVec() * 0.5f);
	m_shootDir = m_parent->GetForwardVec();

	if (m_shooting)
	{
		FindClosestEnemy();

		std::chrono::duration<float> last_active = std::chrono::system_clock::now() - m_lastActivateTime;
		if (last_active.count() >= m_durationTime)
		{
			m_shooting = false;
		}
		else
		{
			if (m_target != nullptr)
			{
				SetShootDir();
				std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastShootTime;

				if (elapsed_seconds.count() >= m_fireRateCooldown)
				{
					m_parent->GetProjectiles().emplace_back(std::make_unique<Laser>(m_position + m_shootDir * 5.f, m_shootDir));

					m_lastShootTime = std::chrono::system_clock::now();
				}
			}
		}
	}

	m_transforms = glm::inverse(glm::lookAt(m_position, m_position - m_shootDir, m_parent->GetUpVec()));
}

void TurretWeapon::FindClosestEnemy()
{
	float distance = 201.f;
	int minInd = -1;

	std::vector<std::shared_ptr<Entity>>* entities = m_parent->GetMapPtr()->GetEntitiesPtr();
	for (int i = 0; i < entities->size(); i++)
	{
		if (!entities->at(i).get()->IsTargetable()) continue;

		glm::vec3 to_target = entities->at(i)->GetPos() - m_position;
		if (glm::length(to_target) < distance)
		{
			to_target = glm::normalize(to_target);
			float dot_up = glm::dot(m_parent->GetUpVec(), to_target);
			float dot_cross = glm::dot(m_parent->GetCrossVec(), to_target);

			if (dot_up > 0.27f) continue;
			if ((m_side == -1 && dot_cross < -0.2f) || (m_side == 1) && dot_cross > 0.2f) continue;

			minInd = i;
			distance = glm::length(entities->at(i)->GetPos() - m_position);
		}
	}

	if (distance < 200.f)
	{
		m_target = reinterpret_cast<Enemy*>(entities->at(minInd).get());
	}
	else
	{
		m_target = nullptr;
	}

}

void TurretWeapon::SetShootDir()
{
	if (m_target == nullptr || !m_shooting)
	{
		m_shootDir = m_parent->GetForwardVec();
		return;
	}
	m_shootDir = glm::normalize((m_target->GetPos() + m_target->GetForwardVec() * 10.f) - m_position);
}


