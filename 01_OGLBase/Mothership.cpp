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
	m_Map = nullptr;
}

Mothership::Mothership(const glm::vec3& pos, Entity* target, std::vector<std::unique_ptr<Projectile>>* projectiles, Map* map)
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

	m_Map = map;
	m_target = target;
	m_projectiles = projectiles;

	m_Map->GetEntitiesPtr()->emplace_back(std::make_shared<SaR>(SaR(pos + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Map)));
}

bool Mothership::Update(const float& delta)
{
	std::chrono::duration<float> elapsed_seconds = std::chrono::system_clock::now() - m_lastSpawnTime;

	if (elapsed_seconds.count() >= m_spawnTimeWindow)
	{
		m_Map->GetEntitiesPtr()->emplace_back(std::make_shared<SaR>(SaR(m_position + glm::vec3(0, -10, -20), m_target, m_projectiles, m_Map)));

		m_lastSpawnTime = std::chrono::system_clock::now();

		if(m_spawnTimeWindow > 5) m_spawnTimeWindow--;
	}

	return false;
}