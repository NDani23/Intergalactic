#include "Entity.h"

Entity::Entity(const Entity& entity)
{
	m_position = entity.m_position;
	m_mesh = std::make_unique<Mesh>(*entity.m_mesh);
}