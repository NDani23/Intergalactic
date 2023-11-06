#pragma once

#include <glm/glm.hpp>

#include "includes/ObjParser_OGL3.h"
#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"
#include "HitBox.h"
#include "MeshCollider.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

class Entity
{
protected:
	std::unique_ptr<Mesh> m_mesh;
	MeshCollider m_collider;
	Texture2D m_texture;
	glm::mat4 m_transforms;
	glm::vec3 m_position;
	std::vector<HitBox> m_hitboxes;

public:

	Entity();
	virtual ~Entity() {};
	Entity(const std::string&, glm::vec3, const std::string&);
	Entity(const std::string&, glm::vec3, const std::string&, Dimensions);
	Entity(const std::string&, glm::vec3, glm::mat4, const std::string&);
	Entity(const std::string&, glm::vec3, glm::mat4, const std::string&, Dimensions);
	Entity(const Entity&);
	Entity(Entity&&) noexcept;

	Entity& operator=(const Entity&) = default;
	Entity& operator=(Entity&&) noexcept = default;

	virtual std::unique_ptr<Mesh>& GetMesh();
	glm::vec3 GetPos();
	MeshCollider& GetCollider();
	Texture2D& GetTexture();
	glm::mat4& GetWorldTransform();
	std::vector<HitBox>& GetHitboxes();

	void SetTransforms(glm::mat4);
	void SetPos(glm::vec3);
	void AddHitBox(HitBox);

	virtual void DrawMesh(ProgramObject&, glm::mat4&);
	virtual bool Update(const float&);
	virtual bool Hit(int);
	virtual bool IsTargetable();
	virtual bool IsStatic();
	virtual bool CanCollide();
};
