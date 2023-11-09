#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <iostream>
#include <execution>
#include <algorithm>
#include <utility>

#include "../includes/Mesh_OGL3.h"

class MeshCollider
{
private:
	bool m_WordPosChanged;
	glm::mat4 m_WordTrans;
	std::vector<std::pair<glm::vec3, int>> m_vertices_initial;
	std::vector<glm::vec3> m_vertices;

	void UpdateVertices();

public:

	MeshCollider();
	void setVertices(std::vector<Mesh::Vertex>);
	void setWordTrans(glm::mat4);
	std::vector<glm::vec3>& GetVertices();
	glm::vec3 FindFurthestPoint(glm::vec3);
};
