#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

class Player
{
private:
	glm::vec3 m_position;
	glm::vec3 m_forward_vec;
	glm::vec3 m_up_vec;
	glm::vec3 m_cross_vec;

public:

	Player()
	{
		m_position = glm::vec3(0, 0, 0);
		m_forward_vec = glm::vec3(0, 0, 1);
		m_up_vec = glm::vec3(0, 1, 0);
		m_cross_vec = glm::vec3(1,0,0);
	}

	void Move(const glm::vec3& dir)
	{		
		m_position += dir;
	}

	void Roll(const int& dir)
	{
		glm::vec4 new_up_vec = glm::normalize(glm::rotate(dir * 0.1f, m_forward_vec) * glm::vec4(m_up_vec, 0));
		m_up_vec = glm::normalize(glm::vec3(new_up_vec.x, new_up_vec.y, new_up_vec.z));

		glm::vec4 new_cross_vec = glm::normalize(glm::rotate(dir * 0.1f, m_forward_vec) * glm::vec4(m_cross_vec, 0));
		m_cross_vec = glm::normalize(glm::vec3(new_cross_vec.x, new_cross_vec.y, new_cross_vec.z));
	}

	void Pitch(const int& dir)
	{
		glm::vec4 new_up_vec = glm::normalize(glm::rotate(dir * 0.1f, m_cross_vec) * glm::vec4(m_up_vec, 0));
		m_up_vec = glm::normalize(glm::vec3(new_up_vec.x, new_up_vec.y, new_up_vec.z));

		glm::vec4 new_forward_vec = glm::normalize(glm::rotate(dir * 0.1f, m_cross_vec) * glm::vec4(m_forward_vec, 0));
		m_forward_vec = glm::normalize(glm::vec3(new_forward_vec.x, new_forward_vec.y, new_forward_vec.z));
	}

	glm::vec3 GetPos()
	{
		return m_position;
	}

	glm::vec3 GetForwardVec()
	{
		return m_forward_vec;
	}

	glm::vec3 GetUpVec()
	{
		return m_up_vec;
	}

	glm::vec3 GetCrossVec()
	{
		return m_cross_vec;
	}
};