#include "../../headers/scenes/EnemySpawnPoint.h"
#include "../../headers/scenes/Scene.h"

EnemySpawnPoint::EnemySpawnPoint()
{
	m_stage = 1;
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);

	m_coolDown = 20.f;

	m_spawnTimeWindow = 20;

	m_target = nullptr;
	m_projectiles = nullptr;
	m_Scene = nullptr;

	ChangeStage(m_stage);
}

EnemySpawnPoint::EnemySpawnPoint(const glm::vec3& pos, Player* target, std::vector<std::unique_ptr<Projectile>>* projectiles, Scene* scene)
{
	m_stage = 1;
	m_position = pos;

	m_coolDown = 20.f;

	m_spawnTimeWindow = 20;

	m_Scene = scene;
	m_target = target;
	m_projectiles = projectiles;

	ChangeStage(m_stage);
	
	m_Scene->GetEntitiesPtr()->emplace_back(std::make_shared<SaR>(SaR(pos, m_target, m_projectiles, m_Scene)));
	//m_Scene->GetEntitiesPtr()->emplace_back(std::make_shared<Raptor>(Raptor(pos + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Scene)));
	//m_Map->GetEntitiesPtr()->emplace_back(std::make_shared<Falcon>(Falcon(pos + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Map)));
	//m_Map->GetEntitiesPtr()->emplace_back(std::make_shared<Rocketer>(Rocketer(pos + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Map)));
}

void EnemySpawnPoint::Update(const float& delta)
{
	m_coolDown -= delta;

	if (m_coolDown <= 0.f)
	{
		SpawnEnemy();

		if (m_spawnTimeWindow > 5)
		{
			m_spawnTimeWindow--;

			if (m_spawnTimeWindow % 5 == 0)
			{
				ChangeStage(5 - m_spawnTimeWindow / 5);
			}
		}

		m_coolDown = m_spawnTimeWindow;

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
		m_EnemyChances[3] = 0;
		break;
	case 2:
		m_EnemyChances[0] = 60;
		m_EnemyChances[1] = 40;
		m_EnemyChances[2] = 0;
		m_EnemyChances[3] = 0;
		break;
	case 3:
		m_EnemyChances[0] = 30;
		m_EnemyChances[1] = 50;
		m_EnemyChances[2] = 10;
		m_EnemyChances[3] = 10;
		break;
	case 4:
		m_EnemyChances[0] = 5;
		m_EnemyChances[1] = 40;
		m_EnemyChances[2] = 30;
		m_EnemyChances[3] = 25;
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
		m_Scene->GetEntitiesPtr()->emplace_back(std::make_shared<SaR>(SaR(m_position, m_target, m_projectiles, m_Scene)));
		break;
	case 1:
		m_Scene->GetEntitiesPtr()->emplace_back(std::make_shared<Falcon>(Falcon(m_position, m_target, m_projectiles, m_Scene)));
		break;
	case 2:
		m_Scene->GetEntitiesPtr()->emplace_back(std::make_shared<Raptor>(Raptor(m_position, m_target, m_projectiles, m_Scene)));
		break;
	case 3:
		m_Scene->GetEntitiesPtr()->emplace_back(std::make_shared<Rocketer>(Rocketer(m_position, m_target, m_projectiles, m_Scene)));
		break;
	default:
		break;
	}
}
