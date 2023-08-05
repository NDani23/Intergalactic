#include <vector>
#include <string>
#include <memory>

#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"

#include "Entity.h"

class Map
{
private:
	std::vector<Entity> m_Entities;
	TextureCubeMap	m_skyboxTexture;

public:

	std::vector<Entity>& GetEntities()
	{
		return m_Entities;
	}

	TextureCubeMap& GetSkyBox() 
	{
		return m_skyboxTexture;
	}

	void AddEntity(const Entity& entity)
	{
		m_Entities.push_back(entity);	
	}

	void SetSkyBox(std::string right,
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
};