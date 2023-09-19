#include "Laser.h"

Laser::Laser(glm::vec3 Pos, glm::vec3 dir)
{
	m_mesh = nullptr;
	m_transforms = glm::mat4();
	m_position = Pos;
	m_speed = 600;
	m_direction = dir;
	m_damage = 10;
}