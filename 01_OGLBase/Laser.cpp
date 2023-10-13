#include "Laser.h"

Laser::Laser(glm::vec3 Pos, glm::vec3 dir)
{
	m_mesh = nullptr;
	m_transforms = glm::mat4();
	m_position = Pos;
	m_speed = 600;
	m_direction = dir;
	m_damage = 10;
	m_travelDistance = 0.f;
}

Laser::Laser(glm::vec3 Pos, glm::vec3 dir, int damage)
{
	m_mesh = nullptr;
	m_transforms = glm::mat4();
	m_position = Pos;
	m_speed = 600;
	m_direction = dir;
	m_damage = damage;
	m_travelDistance = 0.f;
}

bool Laser::Update(const float& delta)
{
	std::cout << "bemegy" << std::endl;
	glm::vec3 newPos = m_position + m_direction * (delta * m_speed);
	m_travelDistance += glm::length(newPos - m_position);
	m_position = newPos;
	
	return false;
	//return m_travelDistance > 500.f;
}