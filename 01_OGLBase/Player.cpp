#include "Directions.h"
#include "Entity.h"

#include <algorithm>

class Player : public Entity
{
private:
	glm::vec3 m_forward_vec;
	glm::vec3 m_up_vec;
	glm::vec3 m_cross_vec;
	horizontal::direction roll_dir = horizontal::none;
	vertical::direction pitch_dir = vertical::none;

public:

	Player()
	{
		m_position = glm::vec3(0, 0, 0);
		m_forward_vec = glm::vec3(0, 0, 1);
		m_up_vec = glm::vec3(0, 1, 0);
		m_cross_vec = glm::vec3(1,0,0);

		HitBox hitbox = { m_position, {8.0, 2.5, 10.0} };

		m_hitboxes.emplace_back(hitbox);

		m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/player_ship.obj"));
		m_mesh->initBuffers();
		
		m_texture.FromFile("assets/player_tex.png");
	}

	void Move(const float& delta)
	{		
		m_position += GetForwardVec() * (delta * 100);

		switch (roll_dir)
		{
		case horizontal::left:
			Roll(-1);
			break;

		case horizontal::right:
			Roll(1);
			break;
		}

		switch (pitch_dir)
		{
		case vertical::up:
			Pitch(-1);
			break;

		case vertical::down:
			Pitch(1);
			break;
		}

		m_hitboxes[0].Position = m_position;

		m_transforms = glm::inverse(glm::lookAt(GetPos(), GetPos()-GetForwardVec(), GetUpVec()));
	}

	void setRollDir(const horizontal::direction& dir)
	{
		roll_dir = dir;
	}

	void setPitchDir(const vertical::direction& dir)
	{
		pitch_dir = dir;
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

private:

	void Roll(const int& dir)
	{
		glm::vec4 new_up_vec = glm::normalize(glm::rotate(dir * 0.02f, m_forward_vec) * glm::vec4(m_up_vec, 0));
		m_up_vec = glm::normalize(glm::vec3(new_up_vec.x, new_up_vec.y, new_up_vec.z));
		
		glm::vec4 new_cross_vec = glm::normalize(glm::rotate(dir * 0.02f, m_forward_vec) * glm::vec4(m_cross_vec, 0));
		m_cross_vec = glm::normalize(glm::vec3(new_cross_vec.x, new_cross_vec.y, new_cross_vec.z));

		updateDimensions();
	}

	void Pitch(const int& dir)
	{
		glm::vec4 new_up_vec = glm::normalize(glm::rotate(dir * 0.01f, m_cross_vec) * glm::vec4(m_up_vec, 0));
		m_up_vec = glm::normalize(glm::vec3(new_up_vec.x, new_up_vec.y, new_up_vec.z));

		glm::vec4 new_forward_vec = glm::normalize(glm::rotate(dir * 0.01f, m_cross_vec) * glm::vec4(m_forward_vec, 0));
		m_forward_vec = glm::normalize(glm::vec3(new_forward_vec.x, new_forward_vec.y, new_forward_vec.z));

		updateDimensions();
	}

	void updateDimensions()
	{
		m_hitboxes[0].dimensions.height = 2.5 + ((abs(m_up_vec.y) - 1) * (8.0 - 2.5)) / -1;
		m_hitboxes[0].dimensions.height = std::max(2.5 + ((abs(m_forward_vec.y) - 0) * (10.0 - 2.5)) / 1, (double)m_hitboxes[0].dimensions.height);

		m_hitboxes[0].dimensions.width = 8.0 + ((abs(m_cross_vec.x) - 1) * (2.5 - 8.0)) / -1;
		m_hitboxes[0].dimensions.width = std::max(2.5 + ((abs(m_forward_vec.x)) * (10.0 - 2.5)) / 1, (double)m_hitboxes[0].dimensions.width);

		m_hitboxes[0].dimensions.length = 2.5 + ((abs(m_forward_vec.z)) * (10.0 - 2.5)) / 1;
		m_hitboxes[0].dimensions.length = std::max(2.5 + ((abs(m_cross_vec.z)) * (8.0 - 2.5)) / 1, (double)m_hitboxes[0].dimensions.length);
	}
};