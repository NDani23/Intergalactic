#include "Directions.h"
#include "Entity.h"
#include "Laser.h"
#include "LaserGun.h"

#include <algorithm>

class Player : public Entity
{
private:
	glm::vec3 m_forward_vec;
	glm::vec3 m_up_vec;
	glm::vec3 m_cross_vec;
	horizontal::direction roll_dir = horizontal::none;
	vertical::direction pitch_dir = vertical::none;
	std::vector<Projectile> m_projectiles;
	LaserGun gun1;
	LaserGun gun2;

public:

	Player()
	{
		m_position = glm::vec3(0, 0, 0);
		m_forward_vec = glm::vec3(0, 0, 1);
		m_up_vec = glm::vec3(0, 1, 0);
		m_cross_vec = glm::vec3(1,0,0);

		HitBox hitbox = { m_position, {8.0, 2.5, 10.0} };

		gun1.SetShootDir(m_forward_vec);
		gun1.SetPosition(m_position - m_cross_vec);

		gun2.SetShootDir(m_forward_vec);
		gun2.SetPosition(m_position + m_cross_vec);

		m_hitboxes.emplace_back(hitbox);

		m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/player_ship.obj"));
		m_mesh->initBuffers();
		
		m_texture.FromFile("assets/player_tex.png");
	}

	void Move(const float& delta)
	{		
		m_position += GetForwardVec() * (delta * 100);

		switch (roll_dir)
		{
		case horizontal::left:
			Roll(-1);
			break;

		case horizontal::right:
			Roll(1);
			break;
		}

		switch (pitch_dir)
		{
		case vertical::up:
			Pitch(-1);
			break;

		case vertical::down:
			Pitch(1);
			break;
		}

		updateDimensions();


		gun1.SetPosition(m_position - m_cross_vec);
		gun1.SetShootDir(m_forward_vec);
		gun1.SetTransforms(glm::inverse(glm::lookAt(gun1.GetPos(), gun1.GetPos() - GetForwardVec(), GetUpVec())));


		gun2.SetPosition(m_position + m_cross_vec);
		gun2.SetShootDir(m_forward_vec);
		gun2.SetTransforms(glm::inverse(glm::lookAt(gun2.GetPos(), gun2.GetPos() - GetForwardVec(), GetUpVec())));

		m_hitboxes[0].Position = m_position;

		m_transforms = glm::inverse(glm::lookAt(GetPos(), GetPos()-GetForwardVec(), GetUpVec()));
	}

	void Shoot()
	{
		gun1.Shoot(m_projectiles);
		gun2.Shoot(m_projectiles);
	}

	void RemoveProjectile(Projectile& proj)
	{
		auto position = std::find(m_projectiles.begin(), m_projectiles.end(), proj);
		if (position != m_projectiles.end())
			m_projectiles.erase(position);
	}

	void UpdateProjectiles(const float& delta)
	{
		for (Projectile& proj : m_projectiles)
		{
			glm::vec3 newPos = proj.GetPos() + proj.GetDirection() * (delta * proj.GetSpeed());
			proj.SetPosition(newPos);

			glm::vec3 dist_vec = proj.GetPos() - m_position;
			if (glm::length(dist_vec) > 2000.0f)
			{
				auto position = std::find(m_projectiles.begin(), m_projectiles.end(), proj);
				if (position != m_projectiles.end())
					m_projectiles.erase(position);
			}
		}
	}

	void setRollDir(const horizontal::direction& dir)
	{
		roll_dir = dir;
	}

	void setPitchDir(const vertical::direction& dir)
	{
		pitch_dir = dir;
	}

	glm::vec3 GetForwardVec()
	{
		return m_forward_vec;
	}

	glm::vec3 GetUpVec()
	{
		return m_up_vec;
	}

	glm::vec3 GetCrossVec()
	{
		return m_cross_vec;
	}

	std::vector<Projectile>& GetProjectiles()
	{
		return m_projectiles;
	}

	Weapon& GetActiveWeapon1() 
	{
		return gun1;
	}

	Weapon& GetActiveWeapon2()
	{
		return gun2;
	}

private:

	void Roll(const int& dir)
	{
		glm::vec4 new_up_vec = glm::normalize(glm::rotate(dir * 0.02f, m_forward_vec) * glm::vec4(m_up_vec, 0));
		m_up_vec = glm::normalize(glm::vec3(new_up_vec.x, new_up_vec.y, new_up_vec.z));
		
		glm::vec4 new_cross_vec = glm::normalize(glm::rotate(dir * 0.02f, m_forward_vec) * glm::vec4(m_cross_vec, 0));
		m_cross_vec = glm::normalize(glm::vec3(new_cross_vec.x, new_cross_vec.y, new_cross_vec.z));

	}

	void Pitch(const int& dir)
	{
		glm::vec4 new_up_vec = glm::normalize(glm::rotate(dir * 0.01f, m_cross_vec) * glm::vec4(m_up_vec, 0));
		m_up_vec = glm::normalize(glm::vec3(new_up_vec.x, new_up_vec.y, new_up_vec.z));

		glm::vec4 new_forward_vec = glm::normalize(glm::rotate(dir * 0.01f, m_cross_vec) * glm::vec4(m_forward_vec, 0));
		m_forward_vec = glm::normalize(glm::vec3(new_forward_vec.x, new_forward_vec.y, new_forward_vec.z));
	}

	void updateDimensions()
	{
		m_hitboxes[0].dimensions.height = 2.5 + ((abs(m_up_vec.y) - 1) * (8.0 - 2.5)) / -1;
		m_hitboxes[0].dimensions.height = std::max(2.5 + ((abs(m_forward_vec.y) - 0) * (10.0 - 2.5)) / 1, (double)m_hitboxes[0].dimensions.height);

		m_hitboxes[0].dimensions.width = 8.0 + ((abs(m_cross_vec.x) - 1) * (2.5 - 8.0)) / -1;
		m_hitboxes[0].dimensions.width = std::max(2.5 + ((abs(m_forward_vec.x)) * (10.0 - 2.5)) / 1, (double)m_hitboxes[0].dimensions.width);

		m_hitboxes[0].dimensions.length = 2.5 + ((abs(m_forward_vec.z)) * (10.0 - 2.5)) / 1;
		m_hitboxes[0].dimensions.length = std::max(2.5 + ((abs(m_cross_vec.z)) * (8.0 - 2.5)) / 1, (double)m_hitboxes[0].dimensions.length);
	}
};