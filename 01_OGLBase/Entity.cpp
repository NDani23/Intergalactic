#include "Entity.h"

Entity::Entity(const Entity& entity)
{
	m_position = entity.m_position;
	m_mesh = std::make_unique<Mesh>(*entity.m_mesh);
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