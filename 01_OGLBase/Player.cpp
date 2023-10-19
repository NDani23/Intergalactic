#include "Player.h"

Player::Player()
{
	m_health = 100;
	m_max_health = 100;
	m_position = glm::vec3(0, 0, 0);
	m_forward_vec = glm::vec3(0, 0, 1);
	m_up_vec = glm::vec3(0, 1, 0);
	m_cross_vec = glm::vec3(1,0,0);
	m_speed = 90;
	m_max_speed = 90;
	m_slowing = false;
	m_damage = 10;
	m_activeWeaponInd = 1;
	m_target = nullptr;


	m_guns[1] = std::make_unique<LaserGun>(this);
	m_guns[2] = nullptr;
	m_guns[0] = nullptr;

	m_Upgrade = nullptr;

	m_points = 0;
	m_upgradePoints = 20;

	m_stats = {0, 0, 0, 0, 0};

	HitBox hitbox = { m_position, {8.0, 2.5, 10.0} };

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

	m_health = 100 + 20 * m_stats.health;
	m_max_health = 100 + 10 * m_stats.health;
	m_position = glm::vec3(0, 0, 0);
	m_forward_vec = glm::vec3(0, 0, 1);
	m_up_vec = glm::vec3(0, 1, 0);
	m_cross_vec = glm::vec3(1, 0, 0);
	m_speed = 90 + 10*m_stats.speed;
	m_max_speed = 90 + 10 * m_stats.speed;
	m_slowing = false;
	m_damage = 10 + 5*m_stats.damage;
	m_activeWeaponInd = 1;

	m_target = nullptr;

	for (int i = 0; i < 3; i++)
	{
		if (m_guns[i] != nullptr) m_guns[i]->Update();
	}

	if (m_Upgrade != nullptr) m_Upgrade->Update();

	m_points = 0;
	m_upgradePoints = 20;

	HitBox hitbox = { m_position, {8.0, 2.5, 10.0} };


	m_transforms = glm::inverse(glm::lookAt(GetPos(), GetPos() - GetForwardVec(), GetUpVec()));

	m_hitboxes.emplace_back(hitbox);

}

void Player::Move(const float& delta, const glm::vec3& cursor_diff_vec)
{		

	if (m_slowing && m_speed > 80.0f) m_speed = m_speed - 0.5f;
	else if (!m_slowing && m_speed < m_max_speed) m_speed = m_speed + 0.5f;

	m_position += GetForwardVec() * (delta * m_speed);

	glm::vec3 dot_cross = m_cross_vec * glm::dot(m_cross_vec, cursor_diff_vec);
	glm::vec3 dot_up = m_up_vec * glm::dot(m_up_vec, cursor_diff_vec);

	//GO-TO feels good settings
	/*m_forward_vec = glm::normalize(m_forward_vec + dot_up * 0.02f + dot_cross * 0.01f);	
	m_up_vec = glm::normalize(glm::cross(m_forward_vec, m_cross_vec) + dot_cross * 0.05f);
	m_cross_vec = glm::normalize(glm::cross(m_up_vec, m_forward_vec));*/

	//slow settings for starter player
	m_forward_vec = glm::normalize(m_forward_vec + (dot_up * (0.015f + 0.001f * m_stats.mobility)) + (dot_cross * (0.0075f + 0.0005f * m_stats.mobility)));
	m_up_vec = glm::normalize(glm::cross(m_forward_vec, m_cross_vec) + dot_cross * (0.020f + 0.005f * m_stats.mobility));
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


	for (int i = 0; i < 3; i++)
	{
		if (m_guns[i] != nullptr)
		{
			m_guns[i]->Update();
		}
	}

	if(m_Upgrade != nullptr) m_Upgrade->Update();
	

	m_hitboxes[0].Position = m_position;

	m_transforms = glm::inverse(glm::lookAt(GetPos(), GetPos()-GetForwardVec(), GetUpVec()));
}

void Player::Shoot()
{
	if (m_activeWeaponInd == 1)
	{
		m_guns[m_activeWeaponInd]->Shoot(m_projectiles, m_damage);
	}
	else
	{
		m_guns[m_activeWeaponInd]->Shoot(m_projectiles);
	}
}

void Player::ActivateUpgrade()
{
	m_Upgrade->Activate();
}

void Player::RemoveProjectile(std::unique_ptr<Projectile>& proj)
{
	auto position = std::find(m_projectiles.begin(), m_projectiles.end(), proj);
	if (position != m_projectiles.end())
		m_projectiles.erase(position);
}

void Player::UpdateProjectiles(const float& delta)
{
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		if (m_projectiles[i]->Update(delta))
		{	
			m_projectiles.erase(m_projectiles.begin() + i);
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
	m_guns[1]->SetCooldown(0.25f - 0.01f * m_stats.fire_rate);
}

void Player::setActiveWeapon(int ind)
{
	if (ind < 3 && ind >= 0 && m_guns[ind] != nullptr)
	{
		m_activeWeaponInd = ind;
	}
}

void Player::setTarget(Entity* target)
{
	m_target = target;
}

void Player::setSpeed(float speed)
{
	m_speed = speed;
}

Entity* Player::GetTarget()
{
	return m_target;
}

int Player::GetPoints()
{
	return m_points;
}

int Player::GetMaxSpeed()
{
	return m_max_speed;
}

int Player::GetMaxHealth()
{
	return m_max_health;
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

int Player::GetActiveWeaponInd()
{
	return m_activeWeaponInd;
}

std::unique_ptr<Upgrade>& Player::GetUpgrade()
{
	return m_Upgrade;
}

std::unique_ptr<Weapon>* Player::GetWeapons()
{
	return m_guns;
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

std::vector<std::unique_ptr<Projectile>>& Player::GetProjectiles()
{
	return m_projectiles;
}

Weapon& Player::GetActiveWeapon1()
{
	return *m_guns[m_activeWeaponInd];
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