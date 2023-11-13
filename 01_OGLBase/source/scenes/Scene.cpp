#include "../../headers/scenes/Scene.h"
#include "../../headers/scenes/EnemySpawnPoint.h"

Scene::Scene()
{
	m_name = "";
	m_player = nullptr;
	m_floor = nullptr;
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

void Scene::ClearScene()
{
	m_Entities.clear();
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

	for (int i = m_Entities.size() - 1; i >= 0; i--)
	{

		Entity* entity = m_Entities[i].get();
		if (entity->Update(delta))
		{
			m_Entities.erase(m_Entities.begin() + i);
			
			if(!state.gameover) m_player->setPoints(m_player->GetPoints() + 10);

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
	UpdateProjectiles(delta);

	if (state.gameover) return false;
	return CheckForCollision() || DetectHits();
}

bool Scene::CheckForCollision()
{
	glm::vec3 player_pos = m_player->GetPos();
	Dimensions player_dims = m_player->GetHitboxes()[0].dimensions;

	if (GetFloor() != nullptr)
	{
		if (GetFloor()->DetectCollision(*m_player))
		{
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
						return true;
					}
				}
				else
				{
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
				m_projectiles.erase(m_projectiles.begin() + i);
				return true;
			}

			m_projectiles.erase(m_projectiles.begin() + i);
		}

	}

	return false;
}

void Scene::UpdateProjectiles(const float&delta)
{
	for (int i = 0; i < m_projectiles.size(); i++)
	{
		if (m_projectiles[i]->Update(delta))
		{
			m_projectiles.erase(m_projectiles.begin() + i);
		}
	}
}

void Scene::AddEntity(std::shared_ptr<Entity> entity)
{
	m_Entities.emplace_back(entity);
}

void Scene::DrawSkyBox(glm::mat4& viewProj, glm::vec3 eye_pos)
{
	m_skyBox.Draw(viewProj, eye_pos);
}

void Scene::DrawScene(glm::mat4& viewproj, GameState& state, glm::vec3 eye_pos, ProgramObject& laser_program)
{
	m_skyBox.Draw(viewproj, eye_pos);
	
	if(m_floor != nullptr) m_floor->DrawFloor(viewproj, m_player);

	m_program.Use();
	if (state.play)
	{
		for (std::shared_ptr<Entity>& entity : m_Entities)
		{
			m_program.SetUniform("playerPos", m_player->GetPos());
			entity->DrawMesh(m_program, viewproj);
		}
	}
	m_program.Unuse();

	ProgramObject& BaseProgram = m_program;
	for (std::unique_ptr<Projectile>& projectile : m_player->GetProjectiles())
	{
		if (projectile->GetMesh() == nullptr)
		{
			laser_program.Use();
			projectile->DrawMesh(laser_program, viewproj);
			laser_program.Unuse();
		}
		else
		{
			BaseProgram.Use();
			projectile->DrawMesh(BaseProgram, viewproj);
			BaseProgram.Unuse();
		}
	}

	for (std::unique_ptr<Projectile>& projectile : m_projectiles)
	{
		if (projectile->GetMesh() == nullptr)
		{
			laser_program.Use();
			projectile->DrawMesh(laser_program, viewproj);
			laser_program.Unuse();
		}
		else
		{
			BaseProgram.Use();
			projectile->DrawMesh(BaseProgram, viewproj);
			BaseProgram.Unuse();
		}
	}
}