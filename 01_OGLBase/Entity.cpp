#include "Entity.h"


Entity::Entity()
{
	m_mesh = nullptr;
	m_position = glm::vec3(0, 0, 0);
	m_transforms = glm::mat4();
}

Entity::Entity(const std::string& model_path, glm::vec3 position, const std::string& texture_path)
{
	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse(model_path.data()));
	m_mesh->initBuffers();
	m_position = position;
	m_texture.FromFile(texture_path);
	m_transforms = glm::translate(m_position);

	HitBox hitbox = { position, {0.0f, 0.0f, 0.0f} };
	m_hitboxes.emplace_back(hitbox);
}

Entity::Entity(const std::string& model_path, glm::vec3 position, const std::string& texture_path, Dimensions dims)
{
	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse(model_path.data()));
	m_mesh->initBuffers();
	m_position = position;
	m_texture.FromFile(texture_path);
	m_transforms = glm::translate(m_position);

	HitBox hitbox = { position, dims };
	m_hitboxes.emplace_back(hitbox);
}

Entity::Entity(const std::string& model_path, glm::vec3 position, glm::mat4 transform, const std::string& texture_path)
{
	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse(model_path.data()));
	m_mesh->initBuffers();
	m_position = position;
	m_texture.FromFile(texture_path);
	m_transforms = glm::translate(m_position) * transform;
}

Entity::Entity(const std::string& model_path, glm::vec3 position, glm::mat4 transform, const std::string& texture_path, Dimensions dims)
{
	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse(model_path.data()));
	m_mesh->initBuffers();
	m_position = position;
	m_texture.FromFile(texture_path);
	m_transforms = glm::translate(m_position) * transform;

	HitBox hitbox = { position, dims };
	m_hitboxes.emplace_back(hitbox);
}

Entity::Entity(const Entity& entity)
{
	m_position = entity.m_position;

	if (m_mesh != nullptr)
	{
		m_mesh = std::make_unique<Mesh>(*entity.m_mesh);
	}
	m_texture = entity.m_texture;
	m_position = entity.m_position;
	m_hitboxes = entity.m_hitboxes;
}

Entity::Entity(Entity&& other) noexcept
{
	m_position = other.m_position;
	m_mesh = std::move(other.m_mesh);
	m_texture = std::move(other.m_texture);
	m_transforms = other.m_transforms;
	m_hitboxes = std::move(other.m_hitboxes);

	other.m_mesh = nullptr;
	other.m_texture.Clean();
}

std::unique_ptr<Mesh>& Entity::GetMesh()
{
	return m_mesh;
}

glm::vec3 Entity::GetPos()
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

std::vector<HitBox>& Entity::GetHitboxes() 
{
	return m_hitboxes;
}

void Entity::SetTransforms(glm::mat4 transforms)
{
	m_transforms = transforms;
}

void Entity::AddHitBox(HitBox hitbox)
{
	m_hitboxes.emplace_back(hitbox);
}

void Entity::DrawMesh(ProgramObject& program, glm::mat4& viewProj)
{
	program.SetTexture("texImage", 0, m_texture);
	program.SetUniform("MVP", viewProj * m_transforms);
	program.SetUniform("world", m_transforms);
	program.SetUniform("worldIT", glm::inverse(glm::transpose(m_transforms)));

	m_mesh->draw();
}

void Entity::Update()
{

}