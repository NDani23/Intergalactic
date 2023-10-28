#include "EnemySpawnPoint.h"
#include "Map.h"

EnemySpawnPoint::EnemySpawnPoint()
{
	m_stage = 1;
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);

	m_lastSpawnTime = std::chrono::system_clock::now();

	m_spawnTimeWindow = 30;

	m_target = nullptr;
	m_projectiles = nullptr;
	m_Map = nullptr;

	ChangeStage(m_stage);
}

EnemySpawnPoint::EnemySpawnPoint(const glm::vec3& pos, Player* target, std::vector<std::unique_ptr<Projectile>>* projectiles, Map* map)
{
	m_stage = 1;
	m_position = pos;

	m_lastSpawnTime = std::chrono::system_clock::now();

	m_spawnTimeWindow = 30;

	m_Map = map;
	m_target = target;
	m_projectiles = projectiles;

	ChangeStage(m_stage);
	
	m_Map->GetEntitiesPtr()->emplace_back(std::make_shared<SaR>(SaR(pos + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Map)));
	//m_Map->GetEntitiesPtr()->emplace_back(std::make_shared<Raptor>(Raptor(pos + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Map)));
	//m_Map->GetEntitiesPtr()->emplace_back(std::make_shared<Falcon>(Falcon(pos + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Map)));
}

void EnemySpawnPoint::Update(const float& delta)
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastSpawnTime;

	if (elapsed_seconds.count() >= m_spawnTimeWindow)
	{
		//m_Map->GetEntitiesPtr()->emplace_back(std::make_shared<SaR>(SaR(m_position + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Map)));
		SpawnEnemy();

		m_lastSpawnTime = std::chrono::system_clock::now();

		if (m_spawnTimeWindow > 5)
		{
			m_spawnTimeWindow--;

			if (m_spawnTimeWindow % 5 == 0)
			{
				ChangeStage(7 - m_spawnTimeWindow / 5);
			}
		}

	}
}

void EnemySpawnPoint::ChangeStage(int stage)
{
	switch (stage)
	{
	case 1:
		m_EnemyChances[0] = 100;
		m_EnemyChances[1] = 0;
		m_EnemyChances[2] = 0;
		break;
	case 2:
		m_EnemyChances[0] = 80;
		m_EnemyChances[1] = 20;
		m_EnemyChances[2] = 0;
		break;
	case 3:
		m_EnemyChances[0] = 50;
		m_EnemyChances[1] = 40;
		m_EnemyChances[2] = 10;
		break;
	case 4:
		m_EnemyChances[0] = 10;
		m_EnemyChances[1] = 60;
		m_EnemyChances[2] = 30;
		break;
	case 5:
		m_EnemyChances[0] = 20;
		m_EnemyChances[1] = 60;
		m_EnemyChances[2] = 20;
		break;
	case 6:
		m_EnemyChances[0] = 10;
		m_EnemyChances[1] = 50;
		m_EnemyChances[2] = 40;
		break;
	default:
		break;
	}
}

void EnemySpawnPoint::SpawnEnemy()
{
	int lucky;
	srand(time(NULL));

	lucky = rand() % 100 + 1;

	int i;
	for (i = 0; i < m_EnemyChances.size(); i++)
	{
		lucky -= m_EnemyChances[i];
		if (lucky <= 0) break;
	}

	switch (i)
	{
	case 0:
		m_Map->GetEntitiesPtr()->emplace_back(std::make_shared<SaR>(SaR(m_position + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Map)));
		break;
	case 1:
		m_Map->GetEntitiesPtr()->emplace_back(std::make_shared<Falcon>(Falcon(m_position + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Map)));
		break;
	case 2:
		m_Map->GetEntitiesPtr()->emplace_back(std::make_shared<Raptor>(Raptor(m_position + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Map)));
		break;
	default:
		break;
	}
}
