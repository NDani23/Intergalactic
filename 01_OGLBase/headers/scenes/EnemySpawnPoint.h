#pragma once

#include "headers/Entity.h"
#include "headers/weapons/Projectile.h"
#include "headers/enemy/SaR.h"
#include "headers/enemy/Falcon.h"
#include "headers/enemy/Raptor.h"
#include "headers/enemy/Rocketer.h"

#include <chrono>
#include <time.h> 
#include <stdlib.h> 

class Scene;

class EnemySpawnPoint
{
private:
	int m_stage;
	std::array<int, 4> m_EnemyChances;
	glm::vec3 m_position;
	int m_spawnTimeWindow;
	float m_coolDown;
	Player* m_target;
	std::vector<std::unique_ptr<Projectile>>* m_projectiles;
	Scene* m_Scene;

	void ChangeStage(int);
	void SpawnEnemy();
public:
	EnemySpawnPoint();
	EnemySpawnPoint(const glm::vec3&, Player*, std::vector<std::unique_ptr<Projectile>>*, Scene*);
	void Update(const float& delta);
};
