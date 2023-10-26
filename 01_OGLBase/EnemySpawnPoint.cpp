#include "EnemySpawnPoint.h"
#include "Map.h"

EnemySpawnPoint::EnemySpawnPoint()
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);

	m_lastSpawnTime = std::chrono::system_clock::now();

	m_spawnTimeWindow = 30.0f;

	m_target = nullptr;
	m_projectiles = nullptr;
	m_Map = nullptr;
}

EnemySpawnPoint::EnemySpawnPoint(const glm::vec3& pos, Entity* target, std::vector<std::unique_ptr<Projectile>>* projectiles, Map* map)
{
	m_position = pos;

	m_lastSpawnTime = std::chrono::system_clock::now();

	m_spawnTimeWindow = 30.0f;

	m_Map = map;
	m_target = target;
	m_projectiles = projectiles;

	m_Map->GetEntitiesPtr()->emplace_back(std::make_shared<Enemy>(Enemy(pos + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Map)));
}

void EnemySpawnPoint::Update(const float& delta)
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastSpawnTime;

	if (elapsed_seconds.count() >= m_spawnTimeWindow)
	{
		m_Map->GetEntitiesPtr()->emplace_back(std::make_shared<Enemy>(Enemy(m_position + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Map)));

		m_lastSpawnTime = std::chrono::system_clock::now();

		if (m_spawnTimeWindow > 5) m_spawnTimeWindow--;
	}
}
