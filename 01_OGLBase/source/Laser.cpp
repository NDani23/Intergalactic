#include "../headers/Laser.h"

Laser::Laser(glm::vec3 Pos, glm::vec3 dir)
{
	m_mesh = nullptr;
	m_transforms = glm::mat4();
	m_position = Pos;
	m_speed = 800;
	m_direction = dir;
	m_damage = 10;
	m_travelDistance = 0.f;
}

Laser::Laser(glm::vec3 Pos, glm::vec3 dir, int damage)
{
	m_mesh = nullptr;
	m_transforms = glm::mat4();
	m_position = Pos;
	m_speed = 800;
	m_direction = dir;
	m_damage = damage;
	m_travelDistance = 0.f;
}

bool Laser::Update(const float& delta)
{
	glm::vec3 newPos = m_position + m_direction * (delta * m_speed);
	m_travelDistance += glm::length(newPos - m_position);
	m_position = newPos;

	return m_travelDistance > 500.f;
}

bool Laser::CheckHit(Entity* entity)
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

void Laser::DrawMesh(ProgramObject& program, glm::mat4& viewproj)
{
	glm::vec3 Points[2];
	Points[0] = (m_position + (m_direction * 2.0f));
	Points[1] = (m_position - (m_direction * 2.0f));

	program.SetUniform("mvp", viewproj);
	program.SetUniform("points", Points);
	glDrawArrays(GL_LINES, 0, (GLsizei)Points->length());
}