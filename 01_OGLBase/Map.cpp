#include <vector>
#include <string>
#include <memory>

#include "includes/ProgramObject.h"
#include "includes/TextureObject.h"

#include "Entity.h"

class Map
{
private:
	TextureCubeMap	m_skyboxTexture;
	std::vector<std::shared_ptr<Entity>> m_Entities;

public:

	std::vector<std::shared_ptr<Entity>>& GetEntities()
	{
		return m_Entities;
	}

	TextureCubeMap& GetSkyBox() 
	{
		return m_skyboxTexture;
	}

	/*void AddEntity(Entity&& entity)
	{
		m_Entities.emplace_back(std::move(entity));	
	}*/

	void AddEntity(std::shared_ptr<Entity> entity)
	{
		m_Entities.emplace_back(entity);
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