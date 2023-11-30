#include "headers/weapons/Mine.h"
#include "headers/scenes/Scene.h"

Mine::Mine()
{
	m_Scene = nullptr;
	m_transforms = glm::mat4();
	m_position = glm::vec3(0.f, 0.f, 0.f);
	m_speed = 0;
	m_direction = glm::vec3(0.0f, 0.0f, 0.0f);
	m_damage = 200;

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/mine.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/mine_tex.png");

	m_explosionProp.ColorBegin = { 254.f / 255.f, 109.f / 255.f, 41 / 255.f, 1.f };
	m_explosionProp.ColorEnd = { 230.f / 255.f, 230.f / 255.f, 230 / 255.f, 1.f };
	m_explosionProp.SizeBegin = 3.0f;
	m_explosionProp.SizeVariation = 0.5f;
	m_explosionProp.SizeEnd = 0.0f;
	m_explosionProp.LifeTime = 1.5f;
	m_explosionProp.Velocity = { 0.0f, 0.0f, 0.0f };
	m_explosionProp.VelocityVariation = { 50.0f, 50.0f, 50.0f };
	m_explosionProp.Position = { 0.0f, 0.0f, 0.0f };
}

Mine::Mine(glm::vec3& pos, Scene* scene)
{

	m_position = pos;
	m_Scene = scene;
	m_transforms = glm::translate(m_position);
	m_speed = 0;
	m_direction = glm::vec3(0.0f, 0.0f, 0.0f);
	m_damage = 200;

	m_mesh = std::unique_ptr<Mesh>(ObjParser::parse("assets/Weapons&Projectiles/mine.obj"));
	m_mesh->initBuffers();

	m_texture.FromFile("assets/Weapons&Projectiles/mine_tex.png");

	m_explosionProp.ColorBegin = { 254.f / 255.f, 109.f / 255.f, 41 / 255.f, 1.f };
	m_explosionProp.ColorEnd = { 230.f / 255.f, 230.f / 255.f, 230 / 255.f, 1.f };
	m_explosionProp.SizeBegin = 1.5f;
	m_explosionProp.SizeVariation = 0.5f;
	m_explosionProp.SizeEnd = 0.0f;
	m_explosionProp.LifeTime = 1.0f;
	m_explosionProp.Velocity = { 0.0f, 0.0f, 0.0f };
	m_explosionProp.VelocityVariation = { 30.0f, 30.0f, 30.0f };
	m_explosionProp.Position = { 0.0f, 0.0f, 0.0f };
}

bool Mine::Update(const float& delta)
{
	return false;
}

bool Mine::CheckHit(Entity* entity)
{
	if (m_Scene == nullptr) return false;
	if (glm::distance(m_position, entity->GetPos()) < 30.f)
	{
		m_explosionProp.Position = m_position;
		for (int i = 0; i < 50; i++)
		{
			m_Scene->GetParticleSystem().Emit(m_explosionProp);
		}
		return true;
	}

	return false;
}