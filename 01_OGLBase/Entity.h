#pragma once

#include <glm/glm.hpp>

#include "includes/ObjParser_OGL3.h"
#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

class Entity
{
protected:
	std::unique_ptr<Mesh> m_mesh;
	glm::vec3 m_position;
	Texture2D m_texture;
	glm::mat4 m_transforms;

public:
	Entity() {};
	Entity(std::unique_ptr<Mesh>, glm::vec3, Texture2D);
	~Entity() = default;

	std::unique_ptr<Mesh>& GetMesh();
	glm::vec3 GetPosition();
	Texture2D& GetTexture();
	glm::mat4& GetWorldTransform();

	Entity(const Entity&);
	Entity& operator=(const Entity&) = default;
};
