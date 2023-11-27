#include "headers/collision_detection/MeshCollider.h"

MeshCollider::MeshCollider()
{
	m_WordPosChanged = false;
	m_WordTrans = glm::mat4();
}

void MeshCollider::setVertices(std::vector<Mesh::Vertex> vertices)
{
	int i = 0;
	for (Mesh::Vertex vert : vertices)
	{
		m_vertices_initial.emplace_back(std::make_pair(vert.position, i));
		m_vertices.emplace_back(vert.position);
		i++;
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
	std::for_each(std::execution::par, m_vertices_initial.begin(), m_vertices_initial.end(),
		[&WordTrans = m_WordTrans, &vertices = m_vertices](std::pair<glm::vec3, int> vertex_init) -> void {

			glm::vec4 wordPos = WordTrans * glm::vec4(vertex_init.first, 1);
			vertices[vertex_init.second] = glm::vec3(wordPos.x, wordPos.y, wordPos.z);
		});

	m_WordPosChanged = false;
}

std::vector<glm::vec3>& MeshCollider::GetVertices()
{
	if (m_WordPosChanged) UpdateVertices();

	return m_vertices;
}