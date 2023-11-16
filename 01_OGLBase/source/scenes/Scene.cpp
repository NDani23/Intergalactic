#include "../../headers/scenes/Scene.h"
#include "../../headers/scenes/EnemySpawnPoint.h"

Scene::Scene()
{
	m_name = "";
	m_player = nullptr;
	m_floor = nullptr;
	//m_particlePool.resize(50);
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

std::vector<std::shared_ptr<Entity>>& Scene::GetEntities()
{
	return m_Entities;
}

std::vector<std::shared_ptr<Entity>>* Scene::GetEntitiesPtr()
{
	return &m_Entities;
}

std::vector<std::unique_ptr<EnemySpawnPoint>>& Scene::GetSpawnPoints()
{
	return m_enemySpawnPoints;
}

ProgramObject& Scene::getProgram()
{
	return m_program;
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
	for (int i = m_Entities.size() - 1; i >= 0; i--)
	{

		Entity* entity = m_Entities[i].get();
		if (entity->Update(delta))
		{
			Explosion(entity->GetPos());

			m_Entities.erase(m_Entities.begin() + i);

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

	m_particleSystem.Update(delta);
}

void Scene::UpdateProjectiles(const float& delta)
{
	for (int i = 0; i < m_projectiles.size(); i++)
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
	Dimensions player_dims = m_player->GetHitboxes()[0].dimensions;

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
		if (!entity->CanCollide()) continue;

		for (HitBox& hitbox : entity->GetHitboxes())
		{
			glm::vec3 distance_vec = player_pos - hitbox.Position;

			Dimensions hitbox_dims = hitbox.dimensions;

			if (abs(distance_vec.x) < player_dims.width / 2 + hitbox_dims.width / 2
				&& abs(distance_vec.y) < player_dims.height / 2 + hitbox_dims.height / 2
				&& abs(distance_vec.z) < player_dims.length / 2 + hitbox_dims.length / 2)
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
					return true;
				}
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
			if (!entity->CanCollide()) continue;

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

void Scene::DrawScene(glm::mat4& viewproj, GameState& state, glm::vec3 eye_pos, ProgramObject& laser_program)
{
	m_skyBox.Draw(viewproj, eye_pos);
	
	if(m_floor != nullptr) m_floor->DrawFloor(viewproj, m_player);

	if (state.play)
	{
		DrawEntities(viewproj);
		DrawProjectiles(viewproj, laser_program);

		m_program.Use();

		if (m_player->GetTarget() != nullptr && m_player->GetWeapons()[m_player->GetActiveWeaponInd()]->requireTarget())
		{
			m_player->GetTarget()->GetHitboxes()[0].Draw(laser_program, viewproj);
		}

		m_program.Unuse();

	}
}

void Scene::DrawSkyBox(glm::mat4& viewProj, glm::vec3 eye_pos)
{
	m_skyBox.Draw(viewProj, eye_pos);
}


void Scene::DrawEntities(glm::mat4& viewProj)
{
	m_program.Use();

	for (std::shared_ptr<Entity>& entity : m_Entities)
	{
		m_program.SetUniform("playerPos", m_player->GetPos());
		entity->DrawMesh(m_program, viewProj);
	}

	m_particleSystem.Render(viewProj);

	m_program.Unuse();
}

void Scene::DrawProjectiles(glm::mat4& viewProj, ProgramObject& laser_program)
{
	for (std::unique_ptr<Projectile>& projectile : m_player->GetProjectiles())
	{
		if (projectile->GetMesh() == nullptr)
		{
			laser_program.Use();
			projectile->DrawMesh(laser_program, viewProj);
			laser_program.Unuse();
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
			laser_program.Use();
			projectile->DrawMesh(laser_program, viewProj);
			laser_program.Unuse();
		}
		else
		{
			m_program.Use();
			projectile->DrawMesh(m_program, viewProj);
			m_program.Unuse();
		}
	}
}

void Scene::Explosion(glm::vec3& Position)
{
	m_explosionProp.Position = Position;
	for (int i = 0; i < 50; i++)
	{
		m_particleSystem.Emit(m_explosionProp);
	}
}