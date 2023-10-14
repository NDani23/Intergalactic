#pragma once

#include "Entity.h"
#include "Projectile.h"
#include "Enemy.h"
#include <chrono> 

class Mothership : public Entity
{
private:
	std::vector<std::shared_ptr<Entity>>* world_objects;
	float m_spawnTimeWindow;
	std::chrono::time_point<std::chrono::system_clock> m_lastSpawnTime;
	Entity* m_target;
	std::vector<std::unique_ptr<Projectile>>* m_projectiles;

public:
	Mothership();
	Mothership(const glm::vec3&, Entity*, std::vector<std::unique_ptr<Projectile>>*, std::vector<std::shared_ptr<Entity>>*);
	bool Update(const float& delta) override;
};
