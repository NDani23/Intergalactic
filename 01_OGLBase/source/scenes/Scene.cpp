#include "../../headers/scenes/Scene.h"
#include "../../headers/scenes/EnemySpawnPoint.h"

Scene::Scene()
{
	m_name = "";
	m_player = nullptr;
	m_projectiles = nullptr;
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

Floor* Scene::GetFloor()
{
	return nullptr;
}

bool Scene::Update(const float& delta, GameState& state)
{
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

	if (state.gameover) return false;
	return CheckForCollision();
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

void Scene::AddEntity(std::shared_ptr<Entity> entity)
{
	m_Entities.emplace_back(entity);
}

void Scene::DrawSkyBox(glm::mat4& viewProj, glm::vec3 eye_pos)
{
	m_skyBox.Draw(viewProj, eye_pos);
}