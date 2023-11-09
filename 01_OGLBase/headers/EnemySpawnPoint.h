#pragma once

#include "Entity.h"
#include "Projectile.h"
#include "SaR.h"
#include "Falcon.h"
#include "Raptor.h"
#include "Rocketer.h"

#include <chrono>
#include <time.h> 
#include <stdlib.h> 

class Map;

class EnemySpawnPoint
{
private:
	int m_stage;
	std::array<int, 4> m_EnemyChances;
	glm::vec3 m_position;
	int m_spawnTimeWindow;
	std::chrono::time_point<std::chrono::system_clock> m_lastSpawnTime;
	Player* m_target;
	std::vector<std::unique_ptr<Projectile>>* m_projectiles;
	Map* m_Map;

	void ChangeStage(int);
	void SpawnEnemy();
public:
	EnemySpawnPoint();
	EnemySpawnPoint(const glm::vec3&, Player*, std::vector<std::unique_ptr<Projectile>>*, Map*);
	void Update(const float& delta);
};
