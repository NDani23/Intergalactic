#include "MeshCollider.h"

MeshCollider::MeshCollider()
{
	m_WordPosChanged = false;
	m_WordTrans = glm::mat4();
}

void MeshCollider::setVertices(std::vector<Mesh::Vertex> vertices)
{
	for (Mesh::Vertex vert : vertices)
	{
		m_vertices_initial.emplace_back(vert.position);
		m_vertices.emplace_back(vert.position);
	}
}

void MeshCollider::setWordTrans(const glm::mat4 wordMat)
{
	m_WordTrans = wordMat;
	m_WordPosChanged = true;
}

glm::vec3 MeshCollider::FindFurthestPoint(glm::vec3 direction)
{
	if (m_WordPosChanged) UpdateVertices();

	glm::vec3 maxPoint;
	float maxDistance = -FLT_MAX;

	for (glm::vec3 vertex : m_vertices)
	{
		float distance = glm::dot(vertex, direction);
		if (distance > maxDistance)
		{
			maxDistance = distance;
			maxPoint = vertex;
		}
	}

	return maxPoint;
}

void MeshCollider::UpdateVertices()
{
	//std::cout << "Updated" << std::endl;
	for (int i = 0; i < m_vertices.size(); i++)
	{
		glm::vec4 wordPos = m_WordTrans * glm::vec4(m_vertices_initial[i], 1);
		m_vertices[i] = glm::vec3(wordPos.x, wordPos.y, wordPos.z);
	}

	m_WordPosChanged = false;
}

std::vector<glm::vec3>& MeshCollider::GetVertices()
{
	if (m_WordPosChanged) UpdateVertices();

	return m_vertices;
}