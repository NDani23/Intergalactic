#include "Entity.h"

Entity::Entity(const Entity& entity)
{
	m_position = entity.m_position;
	m_mesh = std::make_unique<Mesh>(*entity.m_mesh);
}

Entity::Entity(std::unique_ptr<Mesh> mesh, glm::vec3 position, Texture2D texture)
{
	m_mesh = move(mesh);
	m_position = position;
	m_texture = texture;

	m_transforms = glm::translate(m_position);
}

std::unique_ptr<Mesh>& Entity::GetMesh()
{
	return m_mesh;
}

glm::vec3 Entity::GetPosition()
{
	return m_position;
}

Texture2D& Entity::GetTexture()
{
	return m_texture;
}

glm::mat4& Entity::GetWorldTransform()
{
	return m_transforms;
}