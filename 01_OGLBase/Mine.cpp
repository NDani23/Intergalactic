#include "Mine.h"

Mine::Mine()
{
	m_transforms = glm::mat4();
	m_position = glm::vec3(0.f, 0.f, 0.f);
	m_speed = 0;
	m_direction = glm::vec3(0.0f, 0.0f, 0.0f);
	m_damage = 100;

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/mine.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/mine_tex.png");
}

Mine::Mine(glm::vec3& pos)
{
	m_position = pos;
	m_transforms = glm::translate(m_position);
	m_speed = 0;
	m_direction = glm::vec3(0.0f, 0.0f, 0.0f);
	m_damage = 100;

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/mine.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/mine_tex.png");
}

bool Mine::Update(const float& delta)
{
	return false;
}

bool Mine::CheckHit(Entity* entity)
{
	glm::vec3 distance_vec = entity->GetPos() - m_position;

	return glm::length(distance_vec) < 20.f;
}