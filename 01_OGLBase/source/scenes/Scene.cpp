#include "../../headers/scenes/Scene.h"
#include "../../headers/scenes/EnemySpawnPoint.h"

#include <stack>

Scene::Scene()
{
	m_name = "";
	m_player = nullptr;
	m_floor = nullptr;

	m_explosionProp.ColorBegin = { 254.f / 255.f, 109.f / 255.f, 41 / 255.f, 1.f };
	m_explosionProp.ColorEnd = { 230.f / 255.f, 230.f / 255.f, 230 / 255.f, 1.f };
	m_explosionProp.SizeBegin = 1.5f;
	m_explosionProp.SizeVariation = 0.5f;
	m_explosionProp.SizeEnd = 0.0f;
	m_explosionProp.LifeTime = 1.0f;
	m_explosionProp.Velocity = { 0.0f, 0.0f, 0.0f };
	m_explosionProp.VelocityVariation = { 30.0f, 30.0f, 30.0f };
	m_explosionProp.Position = { 0.0f, 0.0f, 0.0f };

	m_BaseGeometryProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "shaders/particleVert.vert"},
		{ GL_FRAGMENT_SHADER, "shaders/particleFrag.frag"}
		});

	m_BaseGeometryProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },
		});

	m_BaseGeometryProgram.LinkProgram();


	m_LineProgram.Init({
		{GL_VERTEX_SHADER, "shaders/line.vert"},
		{GL_FRAGMENT_SHADER, "shaders/line.frag"}
		});
}

Scene::Scene(Player* player)
{
	m_name = "";
	m_player = player;
	m_floor = nullptr;

	m_explosionProp.ColorBegin = { 254.f / 255.f, 109.f / 255.f, 41 / 255.f, 1.f };
	m_explosionProp.ColorEnd = { 230.f / 255.f, 230.f / 255.f, 230 / 255.f, 1.f };
	m_explosionProp.SizeBegin = 1.5f;
	m_explosionProp.SizeVariation = 0.5f;
	m_explosionProp.SizeEnd = 0.0f;
	m_explosionProp.LifeTime = 1.0f;
	m_explosionProp.Velocity = { 0.0f, 0.0f, 0.0f };
	m_explosionProp.VelocityVariation = { 30.0f, 30.0f, 30.0f };
	m_explosionProp.Position = { 0.0f, 0.0f, 0.0f };

	m_BaseGeometryProgram.AttachShaders({
		{ GL_VERTEX_SHADER, "shaders/particleVert.vert"},
		{ GL_FRAGMENT_SHADER, "shaders/particleFrag.frag"}
		});

	m_BaseGeometryProgram.BindAttribLocations({
		{ 0, "vs_in_pos" },
		});

	m_BaseGeometryProgram.LinkProgram();

	m_LineProgram.Init({
		{GL_VERTEX_SHADER, "shaders/line.vert"},
		{GL_FRAGMENT_SHADER, "shaders/line.frag"}
		});
}

std::vector<std::shared_ptr<Entity>>& Scene::GetEntities()
{
	return m_Entities;
}

std::vector<std::unique_ptr<Projectile>>& Scene::GetProjectiles()
{
	return m_projectiles;
}

std::vector<std::shared_ptr<Entity>>* Scene::GetEntitiesPtr()
{
	return &m_Entities;
}

std::vector<std::unique_ptr<EnemySpawnPoint>>& Scene::GetSpawnPoints()
{
	return m_enemySpawnPoints;
}

void Scene::AddEnemy(glm::vec3 position)
{
	m_Entities.emplace_back(std::make_shared<SaR>(SaR(position, m_player, &m_projectiles, this)));
}

ProgramObject& Scene::getProgram()
{
	return m_program;
}

ParticleSystem& Scene::GetParticleSystem()
{
	return m_particleSystem;
}

std::string& Scene::getName()
{
	return m_name;
}

std::unique_ptr<Floor>& Scene::GetFloor()
{
	return m_floor;
}

bool Scene::Update(const float& delta, GameState& state)
{
	m_player->setTarget(nullptr);

	for (int i = 0; i < m_enemySpawnPoints.size(); i++)
	{
		m_enemySpawnPoints.at(i)->Update(delta);
	}

	UpdateEntities(delta, state);
	UpdateProjectiles(delta);

	if (state.gameover) return false;
	return CheckForCollision() || DetectHits();
}

void Scene::UpdateEntities(const float& delta, GameState& state)
{
	std::stack<int> EntitiesToDelete;

	for (int i = 0; i < m_Entities.size(); i++)
	{

		Entity* entity = m_Entities[i].get();
		if (entity->Update(delta))
		{
			Explosion(entity->GetPos());

			EntitiesToDelete.push(i);

			if (!state.gameover) m_player->setPoints(m_player->GetPoints() + 10);

			continue;
		}

		if (!entity->IsTargetable()) continue;

		glm::vec3 entityPos = entity->GetPos();
		glm::vec3 from_player = entityPos - m_player->GetPos();
		float angle = glm::acos(glm::dot(glm::normalize(from_player), m_player->GetForwardVec()));

		if (glm::length(from_player) < 500.f && !isnan(angle) && !isinf(angle) && angle < 0.3f)
		{
			m_player->setTarget(entity);
		}
	}

	while (!EntitiesToDelete.empty())
	{
		m_Entities.erase(m_Entities.begin() + EntitiesToDelete.top());
		EntitiesToDelete.pop();
	}

	m_particleSystem.Update(delta);
}

void Scene::UpdateProjectiles(const float& delta)
{
	for (int i = m_projectiles.size()-1; i >= 0; i--)
	{
		if (m_projectiles[i]->Update(delta))
		{
			m_projectiles.erase(m_projectiles.begin() + i);
		}
	}
}

bool Scene::CheckForCollision()
{
	glm::vec3 player_pos = m_player->GetPos();

	if (GetFloor() != nullptr)
	{
		if (GetFloor()->DetectCollision(*m_player))
		{
			Explosion(player_pos);
			return true;
		}
	}

	for (std::shared_ptr<Entity>& entity : m_Entities)
	{
		if (AAB::Collide(m_player->GetHitboxes(), entity->GetHitboxes()))
		{
			if (entity->IsStatic())
			{
				if (GJK::Collide(m_player->GetCollider(), entity.get()->GetCollider()))
				{
					Explosion(player_pos);
					return true;
				}
			}
			else
			{
				Explosion(player_pos);

				Explosion(entity->GetPos());
				auto position = std::find(m_Entities.begin(), m_Entities.end(), entity);
				if (position != m_Entities.end())
					m_Entities.erase(position);

				return true;
			}
		}
	}

	return false;
}

bool Scene::DetectHits()
{
	std::vector<std::unique_ptr<Projectile>>& player_projectiles = m_player->GetProjectiles();
	for (int i = 0; i < player_projectiles.size(); i++)
	{
		Projectile* proj = player_projectiles[i].get();
		for (std::shared_ptr<Entity>& entity : m_Entities)
		{

			if (proj->CheckHit(entity.get()))
			{
				if (entity->Hit(proj->GetDamage()))
				{
					if (entity.get() == m_player->GetTarget()) m_player->setTarget(nullptr);

					Explosion(entity->GetPos());

					auto position = std::find(m_Entities.begin(), m_Entities.end(), entity);
					if (position != m_Entities.end())
						m_Entities.erase(position);

					m_player->setPoints(m_player->GetPoints() + 10);
				}

				m_player->RemoveProjectile(player_projectiles[i]);
				break;
			}
		}
	}

	for (int i = 0; i < m_projectiles.size(); i++)
	{
		bool alive = true;
		Projectile* proj = m_projectiles[i].get();
		for (std::shared_ptr<Entity>& entity : m_Entities)
		{
			if (proj->CheckHit(entity.get()))
			{
				if (entity->Hit(proj->GetDamage()))
				{
					auto position = std::find(m_Entities.begin(), m_Entities.end(), entity);
					if (position != m_Entities.end())
						m_Entities.erase(position);

					m_player->setPoints(m_player->GetPoints() + 10);
				}

				m_projectiles.erase(m_projectiles.begin() + i);
				alive = false;
				break;
			}
		}
		if (!alive) break;

		if (proj->CheckHit(m_player))
		{
			if (m_player->Hit(proj->GetDamage()))
			{
				Explosion(m_player->GetPos());
				m_projectiles.erase(m_projectiles.begin() + i);
				return true;
			}

			m_projectiles.erase(m_projectiles.begin() + i);
		}

	}

	return false;
}

void Scene::AddEntity(std::shared_ptr<Entity> entity)
{
	m_Entities.emplace_back(entity);
}

void Scene::DrawScene(glm::mat4& viewproj, GameState& state, glm::vec3 eye_pos)
{
	m_skyBox.Draw(viewproj, eye_pos);
	
	if(m_floor != nullptr) m_floor->DrawFloor(viewproj, m_player);

	if (state.play)
	{
		DrawEntities(viewproj, eye_pos);
		DrawProjectiles(viewproj, m_BaseGeometryProgram);

		if (m_player->GetTarget() != nullptr && m_player->GetWeapons()[m_player->GetActiveWeaponInd()]->requireTarget())
		{
			m_player->GetTarget()->GetHitboxes()[0].Draw(m_LineProgram, viewproj);
		}
	}

	/*m_LineProgram.Use();
	DrawHitBoxes(m_LineProgram, viewproj);
	m_LineProgram.Unuse();*/
}

void Scene::DrawSkyBox(glm::mat4& viewProj, glm::vec3 eye_pos)
{
	m_skyBox.Draw(viewProj, eye_pos);
}


void Scene::DrawEntities(glm::mat4& viewProj, glm::vec3& eye_pos)
{
	m_program.Use();
	for (std::shared_ptr<Entity>& entity : m_Entities)
	{
		m_program.SetUniform("eye_pos", eye_pos);
		m_program.SetUniform("playerPos", m_player->GetPos());
		entity->DrawMesh(m_program, viewProj);
	}

	m_particleSystem.Render(viewProj);

	m_program.Unuse();
}

void Scene::DrawProjectiles(glm::mat4& viewProj, ProgramObject& geometry_program)
{
	for (std::unique_ptr<Projectile>& projectile : m_player->GetProjectiles())
	{
		if (projectile->GetMesh() == nullptr)
		{
			projectile->DrawMesh(geometry_program, viewProj);
		}
		else
		{
			m_program.Use();
			projectile->DrawMesh(m_program, viewProj);
			m_program.Unuse();
		}
	}

	for (std::unique_ptr<Projectile>& projectile : m_projectiles)
	{
		if (projectile->GetMesh() == nullptr)
		{
			projectile->DrawMesh(geometry_program, viewProj);
		}
		else
		{
			m_program.Use();
			projectile->DrawMesh(m_program, viewProj);
			m_program.Unuse();
		}
	}
}

void Scene::DrawHitBoxes(ProgramObject& program, glm::mat4& viewProj)
{
	std::vector<glm::vec3> Points;

	for (std::shared_ptr<Entity>& entity : m_Entities)
	{
		for (HitBox& hitbox : entity->GetHitboxes())
		{
			hitbox.Draw(program, viewProj);
		}
	}

	//m_player.GetHitboxes()[0].Draw(program, viewProj);
}

void Scene::Explosion(glm::vec3& Position)
{
	m_explosionProp.Position = Position;
	for (int i = 0; i < 50; i++)
	{
		m_particleSystem.Emit(m_explosionProp);
	}
}