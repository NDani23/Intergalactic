#include "Mothership.h"

Mothership::Mothership()
{
	m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	m_transforms = glm::mat4();

	HitBox hitbox = { m_position, {0.0f, 0.0f, 0.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/mothership.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/mothership_tex3.png");

	m_lastSpawnTime = std::chrono::system_clock::now();

	m_spawnTimeWindow = 30.0f;

	m_target = nullptr;
	m_projectiles = nullptr;
	world_objects = nullptr;
}

Mothership::Mothership(const glm::vec3& pos, Entity* target, std::vector<Projectile>* projectiles, std::vector<std::shared_ptr<Entity>>* objects)
{
	m_position = pos;
	m_transforms = glm::translate(pos);

	HitBox hitbox = { m_position, {70.0f, 28.0f, 20.0f} };
	m_hitboxes.emplace_back(hitbox);

	hitbox = { m_position + glm::vec3(-50, 7, 0), {30.0f, 10.0f, 15.0f} };
	m_hitboxes.emplace_back(hitbox);

	hitbox = { m_position + glm::vec3(-60, 7, -20), {30.0f, 10.0f, 10.0f} };
	m_hitboxes.emplace_back(hitbox);

	hitbox = { m_position + glm::vec3(-60, 7, 20), {30.0f, 10.0f, 10.0f} };
	m_hitboxes.emplace_back(hitbox);

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/mothership.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/mothership_tex3.png");

	m_lastSpawnTime = std::chrono::system_clock::now();

	m_spawnTimeWindow = 30.0f;

	world_objects = objects;
	m_target = target;
	m_projectiles = projectiles;

	world_objects->emplace_back(std::make_shared<Enemy>(Enemy(pos + glm::vec3(0, -10, -20), m_target, projectiles, world_objects)));
	//outMap.AddEntity(std::make_shared<Enemy>(Enemy(glm::vec3(0, 0, 1000), player, &projectiles, outMap.GetEntitiesPtr())));
}

bool Mothership::Update(const float& delta)
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastSpawnTime;

	/*if (elapsed_seconds.count() >= m_spawnTimeWindow)
	{
		world_objects->emplace_back(std::make_shared<Enemy>(Enemy(m_position + glm::vec3(0, -10, -20), m_target, m_projectiles, world_objects)));

		m_lastSpawnTime = std::chrono::system_clock::now();

		if(m_spawnTimeWindow > 5) m_spawnTimeWindow--;
	}*/

	return false;
}