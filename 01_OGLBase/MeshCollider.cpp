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
		m_vertices.emplace_back(vert.position);
	}
}

void MeshCollider::setWordTrans(const glm::mat4 wordMat)
{
	m_WordTrans = wordMat;
	m_WordPosChanged = true;
}

glm::vec3 MeshCollider::FindFurthestPoint(glm::vec3& direction)
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
	for (glm::vec3& vert : m_vertices)
	{
		glm::vec4 wordPos = m_WordTrans * glm::vec4(vert, 1);
		vert = glm::vec3(wordPos.x, wordPos.y, wordPos.z);
	}
}