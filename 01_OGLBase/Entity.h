#pragma once

#include <glm/glm.hpp>

#include "includes/ObjParser_OGL3.h"
#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"
#include "HitBox.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

class Entity
{
protected:
	std::unique_ptr<Mesh> m_mesh;
	Texture2D m_texture;
	glm::mat4 m_transforms;
	glm::vec3 m_position;
	std::vector<HitBox> m_hitboxes;

public:

	Entity();
	Entity(const std::string&, glm::vec3, const std::string&);
	Entity(const std::string&, glm::vec3, const std::string&, Dimensions);
	Entity(const std::string&, glm::vec3, glm::mat4, const std::string&);
	Entity(const std::string&, glm::vec3, glm::mat4, const std::string&, Dimensions);
	Entity(const Entity&);
	Entity(Entity&&) noexcept;

	Entity& operator=(const Entity&) = default;
	Entity& operator=(Entity&&) noexcept = default;
	~Entity() = default;

	std::unique_ptr<Mesh>& GetMesh();
	glm::vec3 GetPos();
	Texture2D& GetTexture();
	glm::mat4& GetWorldTransform();
	std::vector<HitBox>& GetHitboxes();

	void SetTransforms(glm::mat4);
	void AddHitBox(HitBox);

	void DrawMesh(ProgramObject&, glm::mat4&);
	virtual bool Update(const float& delta);
	virtual bool Hit(int);
	virtual bool IsTargetable();
};
