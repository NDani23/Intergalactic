#include "Entity.h"

Entity::Entity(const Entity& entity)
{
	m_position = entity.m_position;
	m_mesh = std::make_unique<Mesh>(*entity.m_mesh);
	m_texture = entity.m_texture;
	m_position = entity.m_position;
}

Entity::Entity(Entity&& other) noexcept
{
	m_position = other.m_position;
	m_mesh = std::move(other.m_mesh);
	m_texture = std::move(other.m_texture);
	m_transforms = other.m_transforms;

	other.m_mesh = nullptr;
	other.m_texture.Clean();
}

Entity::Entity(const std::string& model_path, glm::vec3 position, const std::string& texture_path)
{

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse(model_path.data()));
	m_mesh->initBuffers();
	m_position = position;
	m_texture.FromFile(texture_path);

	m_transforms = glm::translate(m_position);
}

Entity::Entity(const std::string& model_path, glm::vec3 position, glm::mat4 transform, const std::string& texture_path)
{

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse(model_path.data()));
	m_mesh->initBuffers();
	m_position = position;
	m_texture.FromFile(texture_path);

	m_transforms = glm::translate(m_position) * transform;
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

void Entity::SetTransforms(glm::mat4 transforms)
{
	m_transforms = transforms;
}