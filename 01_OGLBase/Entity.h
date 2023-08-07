#pragma once

#include <glm/glm.hpp>

#include "includes/ObjParser_OGL3.h"
#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"

class Entity
{
protected:
	std::unique_ptr<Mesh> m_mesh;
	glm::vec3 m_position;
	Texture2D m_texture;

public:
	Entity() {};
	Entity(std::unique_ptr<Mesh>, glm::vec3, Texture2D);
	~Entity() = default;

	std::unique_ptr<Mesh>& GetMesh();
	glm::vec3 GetPosition();
	Texture2D& GetTexture();

	Entity(const Entity&);
	Entity& operator=(const Entity&) = default;
};
