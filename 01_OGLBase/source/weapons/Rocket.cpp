#include "../../headers/weapons/Rocket.h"

Rocket::Rocket()
{
	m_target = nullptr;
	m_transforms = glm::mat4();
	m_position = glm::vec3(0.f, 0.f, 0.f);
	m_speed = 210;
	m_direction = glm::vec3(1.0f, 0.0f, 0.0f);
	m_damage = 100;
	m_travelDistance = 0.f;

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/rocket.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/rocket_tex.png");
}

Rocket::Rocket(glm::vec3 pos, Entity* target)
{
	m_target = target;
	m_position = pos;
	m_speed = 210;
	m_direction = glm::normalize(target->GetPos() - m_position);
	m_damage = 100;
	m_travelDistance = 0.f;
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_direction, glm::vec3(0.0f, 1.0f, 0.0f)));

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/rocket.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/rocket_tex.png");
}

bool Rocket::Update(const float& delta)
{

	glm::vec3 temp_dir = glm::normalize(m_target->GetPos() - m_position);

	glm::vec3 cross_vec = temp_dir - m_direction;

	if (glm::length(cross_vec) * (delta * 150.f) < 0.015f)
	{
		m_direction = temp_dir;
	}
	else
	{
		m_direction = glm::normalize(m_direction + cross_vec * 0.015f * (delta * 150.f));
	}
	glm::vec3 newPos = m_position + m_direction * (delta * m_speed);
	m_travelDistance += glm::length(newPos - m_position);
	m_position = newPos;
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_direction, glm::vec3(0.0f, 1.0f, 0.0f)));

	return m_travelDistance > 800.f;
}

bool Rocket::CheckHit(Entity* entity)
{

	glm::vec3 distance_vec = entity->GetPos() - m_position;
	Dimensions hitbox_dims = entity->GetHitboxes()[0].dimensions;

	if (abs(distance_vec.x) < hitbox_dims.width / 2
		&& abs(distance_vec.y) < hitbox_dims.height / 2
		&& abs(distance_vec.z) < hitbox_dims.length / 2)
	{
		return true;
	}

	return false;
}