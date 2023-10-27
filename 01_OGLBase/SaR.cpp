#include "SaR.h"
#include "Map.h"

std::unique_ptr<Mesh> SaR::m_static_mesh;
Texture2D SaR::m_static_tex;

int SaR::FirstInit()
{
	SaR::m_static_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/enemy_ship.obj"));
	SaR::m_static_mesh->initBuffers();
	SaR::m_static_tex.FromFile("assets/enemy_tex.png");

	return 1;
}

SaR::SaR()
{
	static int onFirstCall = FirstInit();
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_forward_vec = glm::vec3(1.0f, 0.0f, 0.0f);
	m_shootDir = m_forward_vec;
	m_up_vec = glm::vec3(0.0f, 1.0f, 0.0f);

	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_shootDir, glm::vec3(0.0f, 1.0f, 0.0f)));

	HitBox hitbox = { m_position, {10.0f, 3.0f, 11.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_health = 100;
	m_speed = 90;

	m_coolDownTime = 0.25f;
	m_lastShootTime = std::chrono::system_clock::now();
}

SaR::SaR(glm::vec3 pos, Player* target, std::vector<std::unique_ptr<Projectile>>* projectiles, Map* map)
{
	static int onFirstCall = FirstInit();
	m_position = pos;
	m_target = target;
	m_projectiles = projectiles;
	m_Map = map;

	m_forward_vec = glm::normalize(m_target->GetPos() - m_position);
	m_shootDir = m_forward_vec;
	m_up_vec = glm::vec3(0.0f, 1.0f, 0.0f);

	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_shootDir, glm::vec3(0.0f, 1.0f, 0.0f)));

	HitBox hitbox = { m_position, {10.0f, 3.0f, 11.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_health = 100;
	m_speed = 100;

	m_mesh = nullptr;

	m_coolDownTime = 0.25f;
	m_lastShootTime = std::chrono::system_clock::now();
}

bool SaR::Update(const float& delta)
{
	m_position += m_forward_vec * (delta * m_speed);
	//std::cout << m_position.x << " " << m_position.y << " " << m_position.z << std::endl;
	glm::vec3 temp_dir;
	CalcBaseDir(temp_dir);
	if (CalcAvoidObjectsVec(temp_dir)) return true;
	if (CalcAvoidFloorVec(temp_dir)) return true;

	RegulateTurnDegree(temp_dir);

	m_shootDir = m_forward_vec;

	//Check if shoot
	glm::vec3 to_target = m_target->GetPos() - m_position;
	float angle = glm::length(glm::normalize(to_target) - m_forward_vec);
	if (glm::length(to_target) < 200.0f && angle < 0.1f)
	{
		Shoot();
	}

	m_hitboxes[0] = UpdateDimensions();
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_forward_vec, m_up_vec));

	return false;
}

void SaR::CalcBaseDir(glm::vec3& temp_dir)
{
	temp_dir = glm::normalize(m_target->GetPos() - m_position);


	bool behind_player = dot(m_target->GetPos() - m_position, m_forward_vec) > 0 ? true : false;
	
	if (behind_player) return;
	
	glm::vec3 target_dir = m_target->GetForwardVec();
	float angle = acos(dot(target_dir, m_forward_vec));
	if (angle < M_PI/2 && glm::length(m_target->GetPos() - m_position) < 200.f)
	{
		glm::vec3 cross_vec = glm::normalize(m_forward_vec - (-temp_dir));
		temp_dir = glm::normalize(m_forward_vec - cross_vec);
	}

}

bool SaR::CalcAvoidObjectsVec(glm::vec3& temp_dir)
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
		else if (angle < 1.5f && behind_player && glm::length(m_target->GetPos() - m_position) < 50.f)
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

bool SaR::CalcAvoidFloorVec(glm::vec3& temp_dir)
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
				temp_dir += cross_vec * (1.0f / (angle*10.f));
				temp_dir = glm::normalize(temp_dir);
			}
		}
	}

	return false;
}

void SaR::Shoot()
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastShootTime;

	if (elapsed_seconds.count() >= m_coolDownTime)
	{
		m_projectiles->emplace_back(std::make_unique<Laser>(m_position, m_shootDir));

		m_lastShootTime = std::chrono::system_clock::now();
	}
}

void SaR::DrawMesh(ProgramObject& program, glm::mat4& viewProj)
{
	program.SetTexture("texImage", 0, m_static_tex);
	program.SetUniform("MVP", viewProj * m_transforms);
	program.SetUniform("world", m_transforms);
	program.SetUniform("worldIT", glm::inverse(glm::transpose(m_transforms)));

	m_static_mesh->draw();
}

HitBox SaR::UpdateDimensions()
{
	HitBox newHitBox = { m_position, {10.0f, 3.0f, 11.0f} };
	glm::vec3 cross_vec = glm::normalize(glm::cross(m_forward_vec, m_up_vec));

	newHitBox.dimensions.height = 3.0f + ((abs(m_up_vec.y) - 1) * (10.0f - 3.0f)) / -1;
	newHitBox.dimensions.height = std::max(2.5 + ((abs(m_forward_vec.y) - 0) * (11.0f - 3.0f)) / 1, (double)newHitBox.dimensions.height);

	newHitBox.dimensions.width = 10.0f + ((abs(cross_vec.x) - 1) * (3.0f - 10.0f)) / -1;
	newHitBox.dimensions.width = std::max(2.5 + ((abs(m_forward_vec.x)) * (11.0f - 3.0f)) / 1, (double)newHitBox.dimensions.width);

	newHitBox.dimensions.length = 3.0f + ((abs(m_forward_vec.z)) * (11.0f - 3.0f)) / 1;
	newHitBox.dimensions.length = std::max(3.0f + ((abs(cross_vec.z)) * (10.0f - 3.0)) / 1, (double)newHitBox.dimensions.length);

	return newHitBox;
}

void SaR::RegulateTurnDegree(glm::vec3& temp_dir)
{

	glm::vec3 cross_vec = temp_dir - m_forward_vec;

	if (glm::length(cross_vec) < 0.008f)
	{
		m_forward_vec = temp_dir;
		m_up_vec = glm::normalize(m_up_vec + cross_vec);
	}
	else
	{
		m_forward_vec = glm::normalize(m_forward_vec + cross_vec * 0.008f);
		m_up_vec = glm::normalize(m_up_vec + cross_vec * 0.008f);
	}
}