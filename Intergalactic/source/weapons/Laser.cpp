#include "headers/weapons/Laser.h"

GLuint Laser::VaoID = 0;

void Laser::InitGeometry()
{
	std::vector<glm::vec3> vertices;

	//front									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5)});
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5)});
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5)});
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5),});
	//back
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5)});
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5)});
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5)});
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5)});
	//right									 
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5)});
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5)});
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5)});
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5)});
	//left									 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5)});
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5)});
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5)});
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5)});
	//top									 
	vertices.push_back({ glm::vec3(-0.5, +0.5, +0.5)});
	vertices.push_back({ glm::vec3(+0.5, +0.5, +0.5)});
	vertices.push_back({ glm::vec3(-0.5, +0.5, -0.5)});
	vertices.push_back({ glm::vec3(+0.5, +0.5, -0.5)});
	//bottom								 
	vertices.push_back({ glm::vec3(-0.5, -0.5, -0.5)});
	vertices.push_back({ glm::vec3(+0.5, -0.5, -0.5)});
	vertices.push_back({ glm::vec3(-0.5, -0.5, +0.5)});
	vertices.push_back({ glm::vec3(+0.5, -0.5, +0.5)});

	GLushort indices[36];
	int index = 0;
	for (int i = 0; i < 6 * 4; i += 4)
	{
		indices[index + 0] = i + 0;
		indices[index + 1] = i + 1;
		indices[index + 2] = i + 2;
		indices[index + 3] = i + 1;
		indices[index + 4] = i + 3;
		indices[index + 5] = i + 2;
		index += 6;
	}

	GLuint VboID = 0;
	GLuint IbID = 0;

	glGenVertexArrays(1, &VaoID);
	glBindVertexArray(VaoID);

	glGenBuffers(1, &VboID);
	glBindBuffer(GL_ARRAY_BUFFER, VboID);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), &vertices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	glGenBuffers(1, &IbID);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IbID);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	glDeleteBuffers(1, &VboID);
	glDeleteBuffers(1, &IbID);

}

GLuint& Laser::getVaoID()
{
	return Laser::VaoID;
}

Laser::Laser(glm::vec3 Pos, glm::vec3 dir)
{
	m_mesh = nullptr;
	m_transforms = glm::mat4();
	m_position = Pos;
	m_speed = 800;
	m_direction = dir;
	m_damage = 10;
	m_travelDistance = 0.f;
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_direction, glm::vec3(0.0f, 1.0f, 0.0f))) * glm::scale(glm::vec3(0.35f, 0.35f, 2.f));

	if (!Laser::VaoID) InitGeometry();
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
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_direction, glm::vec3(0.0f, 1.0f, 0.0f))) * glm::scale(glm::vec3(0.35f, 0.35f, 2.f));

	if (!Laser::VaoID) InitGeometry();
}

bool Laser::Update(const float& delta)
{
	glm::vec3 newPos = m_position + m_direction * (delta * m_speed);
	m_travelDistance += glm::length(newPos - m_position);
	m_position = newPos;
	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_direction, glm::vec3(0.0f, 1.0f, 0.0f))) * glm::scale(glm::vec3(0.35f, 0.35f, 2.f));

	return m_travelDistance > 500.f;
}

bool Laser::CheckHit(Entity* entity)
{
	return AAB::Collide(entity->GetHitboxes(), m_position);
}

void Laser::DrawMesh(ProgramObject& program, glm::mat4& viewProj)
{
	program.Use();
	program.SetUniform("ViewProj", viewProj);
	program.SetUniform("Transform", m_transforms);
	program.SetUniform("Color", glm::vec4(1.f ,0.f, 0.f, 1.f));
	glBindVertexArray(VaoID);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, nullptr);
	glBindVertexArray(0);
	program.Unuse();
}