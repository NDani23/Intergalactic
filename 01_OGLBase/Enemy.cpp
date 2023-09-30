#include "Enemy.h"


Enemy::Enemy()
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_shootDir = glm::vec3(1.0f, 0.0f, 0.0f);

	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_shootDir, glm::vec3(0.0f, 1.0f, 0.0f)));

	HitBox hitbox = { m_position, {10.0f, 3.0f, 11.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_health = 100;
	m_speed = 90;
	
	m_target = nullptr;
	m_projectiles = nullptr;

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/enemy_ship.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/enemy_tex.png");

	m_coolDownTime = 0.25f;
	m_lastShootTime = std::chrono::system_clock::now();
}

Enemy::Enemy(glm::vec3 pos, Entity* target, std::vector<Projectile>* projectiles)
{
	m_position = pos;
	m_target = target;
	m_projectiles = projectiles;

	m_shootDir = glm::normalize(m_target->GetPos() - m_position);

	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_shootDir, glm::vec3(0.0f, 1.0f, 0.0f)));

	HitBox hitbox = { m_position, {10.0f, 3.0f, 11.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_health = 100;
	m_speed = 90;

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/enemy_ship.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/enemy_tex.png");

	m_coolDownTime = 0.25f;
	m_lastShootTime = std::chrono::system_clock::now();
}

void Enemy::Update(const float& delta)
{
	m_position += m_shootDir * (delta * m_speed);

	glm::vec3 temp_dir = glm::normalize(m_target->GetPos() - m_position);

	float angle = glm::acos(glm::dot(m_shootDir, temp_dir));

	//m_shootDir = temp_dir;

	if (angle < 0.01f)
	{
		m_shootDir = temp_dir;
	}
	else
	{	
		glm::vec3 cross_vec = temp_dir - m_shootDir;

		//std::cout << " bemegy" << std::endl;
		m_shootDir = glm::normalize(m_shootDir + cross_vec * 0.01f);
		//m_shootDir = glm::normalize(m_shootDir + cross_vec);
		
	}
	//std::cout << m_shootDir.x << " " << m_shootDir.y << " " << m_shootDir.z << std::endl;
	//std::cout << angle << std::endl;


	HitBox hitbox = { m_position, {10.0f, 3.0f, 11.0f} };
	m_hitboxes[0] = hitbox;

	m_transforms = glm::inverse(glm::lookAt(m_position, m_position + m_shootDir, glm::vec3(0.0f, 1.0f, 0.0f)));
}