#include "Map.h"


Map::Map()
{
	m_name = "";
	m_player = nullptr;
	m_projectiles = nullptr;
}

std::vector<std::shared_ptr<Entity>>& Map::GetEntities()
{
	return m_Entities;
}

std::vector<std::shared_ptr<Entity>>* Map::GetEntitiesPtr()
{
	return &m_Entities;
}

TextureCubeMap& Map::GetSkyBox()
{
	return m_skyboxTexture;
}

ProgramObject& Map::getProgram()
{
	return m_program;
}

ProgramObject& Map::getSkyBoxProgram()
{
	return m_skyBoxProgram;
}

std::string& Map::getName()
{
	return m_name;
}

void Map::ClearMap()
{
	m_Entities.clear();
}

void Map::AddEntity(std::shared_ptr<Entity> entity)
{
	m_Entities.emplace_back(entity);
}

void Map::SetSkyBox(std::string right,
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