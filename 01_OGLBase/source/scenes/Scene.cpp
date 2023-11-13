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

void Scene::AddEntity(std::shared_ptr<Entity> entity)
{
	m_Entities.emplace_back(entity);
}

void Scene::DrawSkyBox(glm::mat4& viewProj, glm::vec3 eye_pos)
{
	m_skyBox.Draw(viewProj, eye_pos);
}