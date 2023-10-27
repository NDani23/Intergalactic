#pragma once

#include "Entity.h"
#include "Projectile.h"
#include "SaR.h"
#include <chrono>

class Map;

class EnemySpawnPoint
{
private:
	glm::vec3 m_position;
	float m_spawnTimeWindow;
	std::chrono::time_point<std::chrono::system_clock> m_lastSpawnTime;
	Player* m_target;
	std::vector<std::unique_ptr<Projectile>>* m_projectiles;
	Map* m_Map;
public:
	EnemySpawnPoint();
	EnemySpawnPoint(const glm::vec3&, Player*, std::vector<std::unique_ptr<Projectile>>*, Map*);
	void Update(const float& delta);
};
