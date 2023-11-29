#include "headers/Enemy/Enemy.h"
#include "headers/scenes/Scene.h"

#include <thread>
#include <ppl.h>

Enemy::Enemy()
{
	m_target = nullptr;
	m_projectiles = nullptr;
	m_Scene = nullptr;

	m_mesh = nullptr;
}

void Enemy::UpdateDimensions()
{
}

bool Enemy::Update(const float& delta)
{
	m_position += m_forward_vec * (delta * m_speed);
	if (m_currentCoolDown > 0.f) m_currentCoolDown = std::max(0.f, m_currentCoolDown - delta);

	glm::vec3 temp_dir;
	CalcBaseDir(temp_dir);
	if (CalcAvoidObjectsVec(temp_dir)) return true;
	if (CalcAvoidFloorVec(temp_dir)) return true;

	RegulateTurnDegree(temp_dir, delta);

	CheckIfShoot();

	UpdateDimensions();
	SetTransforms(glm::inverse(glm::lookAt(m_position, m_position + m_forward_vec, m_up_vec)));

	m_tailFire.SetPosition(m_position - m_forward_vec * 5.f);
	m_tailFire.Update(delta);

	float soundVolume = (1 - glm::smoothstep(0.f, 300.0f, glm::distance(m_target->GetPos(), m_position))) * 3;
	int prevVolume = Mix_Volume(4, soundVolume);
	if(soundVolume < prevVolume) Mix_Volume(4, prevVolume);
	

	return false;
}

void Enemy::CalcBaseDir(glm::vec3& temp_dir)
{
	glm::vec3 target_pos = m_target->IsStealth() ? m_target->GetFakePos() : m_target->GetPos();
	bool behind_player = dot(m_position - target_pos, m_target->GetForwardVec()) < 0 ? true : false;
	float distance = glm::length(target_pos - m_position);


	temp_dir = glm::normalize(target_pos - m_position);

	if (behind_player) return;

	glm::vec3 target_dir = m_target->GetForwardVec();
	if (distance < 100.f)
	{
		if (distance < 50.f)
		{
			glm::vec3 to_target = glm::normalize(m_target->GetPos() - m_position);
			glm::vec3 m_cross = glm::cross(m_up_vec, m_forward_vec);
			float dot_up = glm::dot(m_up_vec, to_target);
			int up_sign = dot_up > 0 ? 1 : -1;
			float dot_cross = glm::dot(m_cross, to_target);
			int cross_sign = dot_cross > 0 ? 1 : -1;
			temp_dir = glm::normalize(m_forward_vec + m_up_vec * (-(1/(dot_up + up_sign * 1.5f))) + m_cross * (-(1/(dot_cross + cross_sign * 1.5f))));
			return;
		}


		glm::vec3 cross_vec = m_forward_vec - target_dir;
		temp_dir = glm::normalize(m_forward_vec + cross_vec);

	}

}

bool Enemy::CalcAvoidObjectsVec(glm::vec3& temp_dir)
{
	glm::vec3 target_pos = m_target->IsStealth() ? m_target->GetFakePos() : m_target->GetPos();

	//avoid hitting player:
	if (glm::length(target_pos - m_position) < 200.f)
	{
		bool behind_player = dot(target_pos - m_position, m_forward_vec) > 0 ? true : false;

		glm::vec3 target_dir = m_target->GetForwardVec();
		float angle = acos(dot(target_dir, m_forward_vec));

		if (angle > 3.f)
		{
			glm::vec3 cross_vec = glm::normalize(-target_dir - temp_dir);
			temp_dir += (cross_vec * (float)(angle / M_PI));
			temp_dir = glm::normalize(temp_dir);
		}
		else if (angle < 2.f && behind_player && glm::length(target_pos - m_position) < 50.f)
		{
			glm::vec3 cross_vec = glm::normalize(target_dir - temp_dir);
			temp_dir += (cross_vec * (1.0f / (angle * 30.f)));
			temp_dir = glm::normalize(temp_dir);

		}
	}


	Dimensions enemy_dims = m_hitboxes[0].dimensions;

	for (std::shared_ptr<Entity>& obj : m_Scene->GetEntities())
	{
		if (obj.get() == this)
		{
			continue;
		}

		if (AAB::Collide(m_hitboxes, obj->GetHitboxes())) return true;
		
		AvoidObject(*obj.get(), temp_dir);

	}
	return false;
}

void Enemy::AvoidObject(Entity& obj, glm::vec3& temp_dir)
{

	if (!obj.IsStatic())
	{
		if (glm::distance(obj.GetPos(), m_position) < 40.f)
		{
			glm::vec3 from_obj = glm::normalize(m_position - obj.GetPos());

			temp_dir += from_obj;
			temp_dir = glm::normalize(temp_dir);
		}
		return;
	}

	Ray forward_ray = { m_position, temp_dir };
	for (HitBox& hitbox : obj.GetHitboxes())
	{
		float biggest_side = std::max(std::max(hitbox.dimensions.height, hitbox.dimensions.width), hitbox.dimensions.length);

		if (biggest_side < 100.f && glm::distance(m_position, hitbox.Position) < 200.f)
		{
			glm::vec3 to_point = glm::normalize(hitbox.Position - m_position);
			float dot_to_point = glm::dot(temp_dir, to_point);

			if (dot_to_point > 0.7f)
			{
				glm::vec3 cross_vec = glm::normalize(temp_dir - to_point);
				temp_dir += cross_vec * dot_to_point * 0.5f;
				temp_dir = glm::normalize(temp_dir);
			}

			continue;
		}


		if (glm::distance(m_position, hitbox.Position) > biggest_side * 3.f) continue;

		glm::vec3 normal = AAB::RayIntersection(hitbox, forward_ray);

		if (normal == glm::vec3(0.f, 0.f, 0.f)) continue;
		normal = normal * 0.7f;
		float dot_normal = glm::abs(glm::dot(temp_dir, normal));
		temp_dir += normal * (1 / (dot_normal + 1.f));
		temp_dir = glm::normalize(temp_dir);
		
	}
}

bool Enemy::CalcAvoidFloorVec(glm::vec3& temp_dir)
{
	if (m_Scene->GetFloor() != nullptr)
	{
		if (m_Scene->GetFloor()->DetectCollision(*this))
		{
			return true;
		}

		glm::vec3 future_position = m_position + temp_dir * 100.f;
		glm::vec3 distance_vec = glm::vec3(future_position.x, m_Scene->GetFloor()->GetZCoord(future_position.x, future_position.z), future_position.z) - m_position;
		float distance = glm::length(distance_vec);
		if (distance < 300.0f)
		{
			distance_vec = glm::normalize(distance_vec);
			glm::vec3 cross_vec = glm::normalize(distance_vec - temp_dir);

			float angle = glm::acos(glm::dot(distance_vec, temp_dir));
			float dot_p = glm::dot(distance_vec, temp_dir);

			if (dot_p > 0.f)
			{
				temp_dir += glm::vec3(0.f, 1.0f, 0.0f) * dot_p * (1 / (distance / 100));
				temp_dir = glm::normalize(temp_dir);
			}
		}
	}

	return false;
}

void Enemy::RegulateTurnDegree(glm::vec3& temp_dir, const float delta)
{

	glm::vec3 cross_vec = temp_dir - m_forward_vec;

	glm::vec3 cross_obj = -glm::cross(m_forward_vec, m_up_vec);
	glm::vec3 dot_cross = cross_obj * glm::dot(cross_obj, temp_dir);

	float turn_limit = m_mobility * 0.001 * (delta * 150.f);

	if (isnan(glm::length(cross_vec)))
	{
		m_forward_vec = glm::normalize(m_forward_vec + m_up_vec * turn_limit);
		return;
	}
	else if (glm::length(cross_vec) < turn_limit)
	{
		m_forward_vec = temp_dir;
	}
	else
	{
		m_forward_vec = glm::normalize(m_forward_vec + cross_vec * turn_limit);
	}

	cross_obj = -glm::cross(m_forward_vec, m_up_vec);
	m_up_vec = glm::normalize(glm::cross(m_forward_vec, cross_obj) + dot_cross * (0.020f + turn_limit));
}

void Enemy::CheckIfShoot()
{

	glm::vec3 target_pos = m_target->IsStealth() ? m_target->GetFakePos() : m_target->GetPos() + m_target->GetForwardVec() * 10.f;
	m_shootDir = m_forward_vec;
	glm::vec3 to_target = target_pos - m_position;
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

bool Enemy::IsStatic()
{
	return false;
}

glm::vec3& Enemy::GetForwardVec()
{
	return m_forward_vec;
}

void Enemy::FreeAudio()
{
	if (m_shootSound != nullptr)
	{
		Mix_FreeChunk(m_shootSound);
	}
}