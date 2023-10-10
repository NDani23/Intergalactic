#include "Player.h"

Player::Player()
{
	m_health = 100;
	m_position = glm::vec3(0, 0, 0);
	m_forward_vec = glm::vec3(0, 0, 1);
	m_up_vec = glm::vec3(0, 1, 0);
	m_cross_vec = glm::vec3(1,0,0);
	m_speed = 100;
	m_slowing = false;

	m_points = 0;
	m_upgradePoints = 20;

	m_stats = {1, 1, 1, 1, 1};

	HitBox hitbox = { m_position, {8.0, 2.5, 10.0} };

	gun1.SetShootDir(m_forward_vec);
	gun1.SetPosition(m_position - m_cross_vec);
	gun1.SetTransforms(glm::inverse(glm::lookAt(gun1.GetPos(), gun1.GetPos() - GetForwardVec(), GetUpVec())));

	gun2.SetShootDir(m_forward_vec);
	gun2.SetPosition(m_position + m_cross_vec);
	gun2.SetTransforms(glm::inverse(glm::lookAt(gun2.GetPos(), gun2.GetPos() - GetForwardVec(), GetUpVec())));

	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/player_ship.obj"));
	m_mesh->initBuffers();
	
	m_texture.FromFile("assets/player_tex.png");

	m_transforms = glm::inverse(glm::lookAt(GetPos(), GetPos() - GetForwardVec(), GetUpVec()));
}

void Player::Reset()
{

	m_hitboxes.clear();
	m_projectiles.clear();

	m_health = 100;
	m_position = glm::vec3(0, 0, 0);
	m_forward_vec = glm::vec3(0, 0, 1);
	m_up_vec = glm::vec3(0, 1, 0);
	m_cross_vec = glm::vec3(1, 0, 0);
	m_speed = 100;
	m_slowing = false;

	m_points = 0;
	m_upgradePoints = 20;

	HitBox hitbox = { m_position, {8.0, 2.5, 10.0} };

	gun1.SetShootDir(m_forward_vec);
	gun1.SetPosition(m_position - m_cross_vec);
	gun1.SetTransforms(glm::inverse(glm::lookAt(gun1.GetPos(), gun1.GetPos() - GetForwardVec(), GetUpVec())));

	gun2.SetShootDir(m_forward_vec);
	gun2.SetPosition(m_position + m_cross_vec);
	gun2.SetTransforms(glm::inverse(glm::lookAt(gun2.GetPos(), gun2.GetPos() - GetForwardVec(), GetUpVec())));

	m_transforms = glm::inverse(glm::lookAt(GetPos(), GetPos() - GetForwardVec(), GetUpVec()));

	m_hitboxes.emplace_back(hitbox);

}

void Player::Move(const float& delta, const glm::vec3& cursor_diff_vec)
{		

	if (m_slowing && m_speed > 80.0f) m_speed = m_speed - 0.5f;
	else if (!m_slowing && m_speed != 100.0f) m_speed = m_speed + 0.5f;

	m_position += GetForwardVec() * (delta * m_speed);


	glm::vec3 dot_cross = m_cross_vec * glm::dot(m_cross_vec, cursor_diff_vec);
	glm::vec3 dot_up = m_up_vec * glm::dot(m_up_vec, cursor_diff_vec);

	m_forward_vec = glm::normalize(m_forward_vec + dot_up * 0.02f + dot_cross * 0.01f);	
	m_up_vec = glm::normalize(glm::cross(m_forward_vec, m_cross_vec) + dot_cross * 0.05f);
	m_cross_vec = glm::normalize(glm::cross(m_up_vec, m_forward_vec));

	switch (roll_dir)
	{
	case horizontal::left:
		Roll(-1, delta);
		break;

	case horizontal::right:
		Roll(1, delta);
		break;
	}

	switch (pitch_dir)
	{
	case vertical::up:
		Pitch(-1, delta);
		break;

	case vertical::down:
		Pitch(1, delta);
		break;
	}

	updateDimensions();


	gun1.SetPosition(m_position - m_cross_vec);
	gun1.SetShootDir(m_forward_vec);
	gun1.SetTransforms(glm::inverse(glm::lookAt(gun1.GetPos(), gun1.GetPos() - GetForwardVec(), GetUpVec())));


	gun2.SetPosition(m_position + m_cross_vec);
	gun2.SetShootDir(m_forward_vec);
	gun2.SetTransforms(glm::inverse(glm::lookAt(gun2.GetPos(), gun2.GetPos() - GetForwardVec(), GetUpVec())));

	m_hitboxes[0].Position = m_position;

	m_transforms = glm::inverse(glm::lookAt(GetPos(), GetPos()-GetForwardVec(), GetUpVec()));
}

void Player::Shoot()
{
	gun1.Shoot(m_projectiles);
	gun2.Shoot(m_projectiles);
}

void Player::RemoveProjectile(Projectile& proj)
{
	auto position = std::find(m_projectiles.begin(), m_projectiles.end(), proj);
	if (position != m_projectiles.end())
		m_projectiles.erase(position);
}

void Player::UpdateProjectiles(const float& delta)
{
	for (Projectile& proj : m_projectiles)
	{
		glm::vec3 newPos = proj.GetPos() + proj.GetDirection() * (delta * proj.GetSpeed());
		proj.SetPosition(newPos);

		glm::vec3 dist_vec = proj.GetPos() - m_position;
		if (glm::length(dist_vec) > 500.0f)
		{
			auto position = std::find(m_projectiles.begin(), m_projectiles.end(), proj);
			if (position != m_projectiles.end())
				m_projectiles.erase(position);
		}
	}
}

void Player::setRollDir(const horizontal::direction& dir)
{
	roll_dir = dir;
}

void Player::setPitchDir(const vertical::direction& dir)
{
	pitch_dir = dir;
}

void Player::setHealth(int health)
{
	m_health = health;
}

void Player::setPoints(int point)
{
	m_points = point;
}

void Player::setUpgradePoints(int points)
{
	m_upgradePoints = points;
}

void Player::setStats(Stats stat)
{
	m_stats = stat;
}

int Player::GetPoints()
{
	return m_points;
}

int Player::GetUpgradePoints()
{
	return m_upgradePoints;
}

int Player::GetHealth()
{
	return m_health;
}

int Player::GetSpeed()
{
	return m_speed;
}

Stats& Player::GetStats() 
{
	return m_stats;
}

glm::vec3 Player::GetForwardVec()
{
	return m_forward_vec;
}

glm::vec3 Player::GetUpVec()
{
	return m_up_vec;
}

glm::vec3 Player::GetCrossVec()
{
	return m_cross_vec;
}

std::vector<Projectile>& Player::GetProjectiles()
{
	return m_projectiles;
}

Weapon& Player::GetActiveWeapon1()
{
	return gun1;
}

Weapon& Player::GetActiveWeapon2()
{
	return gun2;
}

void Player::Decelerate(bool activated)
{

	activated ? m_slowing = true : m_slowing = false;
}

bool Player::Hit(int damage)
{
	m_health -= damage;
	return m_health <= 0;
}

void Player::Roll(const int& dir, const float& delta)
{
	glm::vec4 new_up_vec = glm::normalize(glm::rotate(dir * (3 * delta), m_forward_vec) * glm::vec4(m_up_vec, 0));
	m_up_vec = glm::normalize(glm::vec3(new_up_vec.x, new_up_vec.y, new_up_vec.z));
	
	glm::vec4 new_cross_vec = glm::normalize(glm::rotate(dir * (3 * delta), m_forward_vec) * glm::vec4(m_cross_vec, 0));
	m_cross_vec = glm::normalize(glm::vec3(new_cross_vec.x, new_cross_vec.y, new_cross_vec.z));

}

void Player::Pitch(const int& dir, const float& delta)
{
	glm::vec4 new_up_vec = glm::normalize(glm::rotate(dir * (1.5f * delta / (m_speed * 0.01f)), m_cross_vec) * glm::vec4(m_up_vec, 0));
	m_up_vec = glm::normalize(glm::vec3(new_up_vec.x, new_up_vec.y, new_up_vec.z));

	glm::vec4 new_forward_vec = glm::normalize(glm::rotate(dir * (1.5f * delta / (m_speed * 0.01f)), m_cross_vec) * glm::vec4(m_forward_vec, 0));
	m_forward_vec = glm::normalize(glm::vec3(new_forward_vec.x, new_forward_vec.y, new_forward_vec.z));
}

void Player::updateDimensions()
{
	m_hitboxes[0].dimensions.height = 2.5 + ((abs(m_up_vec.y) - 1) * (8.0 - 2.5)) / -1;
	m_hitboxes[0].dimensions.height = std::max(2.5 + ((abs(m_forward_vec.y) - 0) * (10.0 - 2.5)) / 1, (double)m_hitboxes[0].dimensions.height);

	m_hitboxes[0].dimensions.width = 8.0 + ((abs(m_cross_vec.x) - 1) * (2.5 - 8.0)) / -1;
	m_hitboxes[0].dimensions.width = std::max(2.5 + ((abs(m_forward_vec.x)) * (10.0 - 2.5)) / 1, (double)m_hitboxes[0].dimensions.width);

	m_hitboxes[0].dimensions.length = 2.5 + ((abs(m_forward_vec.z)) * (10.0 - 2.5)) / 1;
	m_hitboxes[0].dimensions.length = std::max(2.5 + ((abs(m_cross_vec.z)) * (8.0 - 2.5)) / 1, (double)m_hitboxes[0].dimensions.length);
}