#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

#include "includes/Mesh_OGL3.h"

class MeshCollider
{
private:
	bool m_WordPosChanged;
	glm::mat4 m_WordTrans;
	std::vector<glm::vec3> m_vertices_initial;
	std::vector<glm::vec3> m_vertices;

	void UpdateVertices();

public:

	MeshCollider();
	void setVertices(std::vector<Mesh::Vertex>);
	void setWordTrans(glm::mat4);
	glm::vec3 FindFurthestPoint(glm::vec3);
};
