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

TextureCubeMap& Scene::GetSkyBox()
{
	return m_skyboxTexture;
}

ProgramObject& Scene::getProgram()
{
	return m_program;
}

ProgramObject& Scene::getSkyBoxProgram()
{
	return m_skyBoxProgram;
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

void Scene::SetSkyBox(std::string right,
			   std::string left,
			   std::string top,
			   std::string bottom,
			   std::string front,
			   std::string back)
{
	m_skyboxTexture.AttachFromFile(right, false, GL_TEXTURE_CUBE_MAP_POSITIVE_X);
	m_skyboxTexture.AttachFromFile(left, false, GL_TEXTURE_CUBE_MAP_NEGATIVE_X);
	m_skyboxTexture.AttachFromFile(top, false, GL_TEXTURE_CUBE_MAP_POSITIVE_Y);
	m_skyboxTexture.AttachFromFile(bottom, false, GL_TEXTURE_CUBE_MAP_NEGATIVE_Y);
	m_skyboxTexture.AttachFromFile(front, false, GL_TEXTURE_CUBE_MAP_POSITIVE_Z);
	m_skyboxTexture.AttachFromFile(back, true, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z);
}