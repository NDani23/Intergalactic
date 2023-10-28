#include "Enemy.h"
#include "Map.h"

Enemy::Enemy()
{
	m_target = nullptr;
	m_projectiles = nullptr;
	m_Map = nullptr;

	m_mesh = nullptr;
}

HitBox Enemy::UpdateDimensions()
{
	return HitBox();
}

bool Enemy::Update(const float& delta)
{
	m_position += m_forward_vec * (delta * m_speed);

	glm::vec3 temp_dir;
	CalcBaseDir(temp_dir);
	if (CalcAvoidObjectsVec(temp_dir)) return true;
	if (CalcAvoidFloorVec(temp_dir)) return true;

	RegulateTurnDegree(temp_dir);

	CheckIfShoot();

	m_hitboxes[0] = UpdateDimensions();
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_forward_vec, m_up_vec));

	return false;
}

void Enemy::CalcBaseDir(glm::vec3& temp_dir)
{
	glm::vec3 player_behind_pos = m_target->GetPos() - m_target->GetForwardVec() * 100.f;
	temp_dir = glm::normalize(player_behind_pos - m_position);

	if (glm::length(player_behind_pos - m_position) < 50.f) temp_dir = glm::normalize(m_target->GetPos() - m_position);


	bool behind_player = dot(m_target->GetPos() - m_position, m_forward_vec) > 0 ? true : false;

	if (behind_player) return;

	glm::vec3 target_dir = m_target->GetForwardVec();
	float angle = acos(dot(target_dir, m_forward_vec));
	float distance = glm::length(m_target->GetPos() - m_position);
	if (angle < M_PI && distance < 150.f)
	{
		glm::vec3 cross_vec = m_forward_vec - target_dir;
		temp_dir = glm::normalize(m_forward_vec + cross_vec);

	}

}

bool Enemy::CalcAvoidObjectsVec(glm::vec3& temp_dir)
{

	//avoid hitting player:
	if (glm::length(m_target->GetPos() - m_position) < 200.f)
	{
		bool behind_player = dot(m_target->GetPos() - m_position, m_forward_vec) > 0 ? true : false;

		glm::vec3 target_dir = m_target->GetForwardVec();
		float angle = acos(dot(target_dir, m_forward_vec));

		if (angle > 3.f)
		{
			glm::vec3 cross_vec = glm::normalize(-target_dir - temp_dir);
			//std::cout << glm::length(cross_vec) << std::endl;
			temp_dir += (cross_vec * (float)(angle / M_PI));
			temp_dir = glm::normalize(temp_dir);
		}
		else if (angle < 1.8f && behind_player && glm::length(m_target->GetPos() - m_position) < 50.f)
		{
			glm::vec3 cross_vec = glm::normalize(target_dir - temp_dir);
			temp_dir += (cross_vec * (1.0f / (angle * 30.f)));
			temp_dir = glm::normalize(temp_dir);

		}
	}


	Dimensions enemy_dims = m_hitboxes[0].dimensions;

	for (std::shared_ptr<Entity>& obj : m_Map->GetEntities())
	{
		if (obj.get() == this)
		{
			continue;
		}

		for (HitBox& hitbox : obj->GetHitboxes())
		{
			glm::vec3 distance_vec = hitbox.Position - m_position;

			float distance = glm::length(distance_vec);
			if (distance > 200.0f) break;


			//Check collision
			Dimensions hitbox_dims = hitbox.dimensions;

			if (abs(distance_vec.x) < std::max(enemy_dims.width / 2, hitbox_dims.width / 2)
				&& abs(distance_vec.y) < std::max(enemy_dims.height / 2, hitbox_dims.height / 2)
				&& abs(distance_vec.z) < std::max(enemy_dims.length / 2, hitbox_dims.length / 2))
			{
				return true;
			}

			distance_vec = glm::normalize(distance_vec);
			glm::vec3 cross_vec = glm::normalize(distance_vec - temp_dir);

			float angle = glm::acos(glm::dot(distance_vec, temp_dir));

			//if enemy moving in the direction of the object
			if (angle < 1.5f)
			{
				temp_dir += temp_dir - (cross_vec * (1.0f / (angle * 10.f)));
				temp_dir = glm::normalize(temp_dir);
			}

		}

	}
	return false;
}

bool Enemy::CalcAvoidFloorVec(glm::vec3& temp_dir)
{
	if (m_Map->GetFloor() != nullptr)
	{
		if (m_Map->GetFloor()->DetectCollision(*this))
		{
			return true;
		}

		glm::vec3 future_position = m_position + temp_dir * 100.f;
		glm::vec3 distance_vec = glm::vec3(future_position.x, m_Map->GetFloor()->GetZCoord(future_position.x, future_position.z), future_position.z) - m_position;
		float distance = glm::length(distance_vec);
		if (distance < 200.0f)
		{
			distance_vec = glm::normalize(distance_vec);
			glm::vec3 cross_vec = glm::normalize(distance_vec - temp_dir);

			float angle = glm::acos(glm::dot(distance_vec, temp_dir));

			if (angle < 1.5f)
			{
				temp_dir += cross_vec * (1.0f / (angle * 10.f));
				temp_dir = glm::normalize(temp_dir);
			}
		}
	}

	return false;
}

void Enemy::RegulateTurnDegree(glm::vec3& temp_dir)
{

	glm::vec3 cross_vec = temp_dir - m_forward_vec;

	glm::vec3 cross_obj = -glm::cross(m_forward_vec, m_up_vec);
	glm::vec3 dot_cross = cross_obj * glm::dot(cross_obj, temp_dir);

	float turn_limit = m_mobility * 0.001;
	if (glm::length(cross_vec) < turn_limit)
	{
		m_forward_vec = temp_dir;
	}
	else
	{
		m_forward_vec = glm::normalize(m_forward_vec + cross_vec * turn_limit);
	}

	m_up_vec = glm::normalize(glm::cross(m_forward_vec, cross_obj) + dot_cross * (0.020f + turn_limit));
}

void Enemy::CheckIfShoot()
{
	m_shootDir = m_forward_vec;
	glm::vec3 to_target = m_target->GetPos() - m_position;
	float angle = glm::length(glm::normalize(to_target) - m_forward_vec);

	if (glm::length(to_target) < m_shootDistance && angle < m_shootAngle)
	{
		m_shootDir = glm::normalize(to_target);
		Shoot();
	}
}

bool Enemy::Hit(int damage)
{
	m_health -= damage;
	return m_health <= 0;
}

void Enemy::SetTexture()
{
	m_texture.FromFile("assets/enemy_tex.png");
}

bool Enemy::IsTargetable()
{
	return true;
}